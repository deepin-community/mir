/*
 * Copyright © 2015 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 or 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#include "mir/shell/system_compositor_window_manager.h"
#include "mir/shell/surface_specification.h"
#include "mir_toolkit/mir_client_library.h"

#include "mir/geometry/rectangle.h"
#include "mir_test_framework/headless_test.h"
#include "mir/test/signal.h"
#include "mir/test/spin_wait.h"

#include "gmock/gmock.h"

#include <mutex>

namespace msh = mir::shell;
namespace mt = mir::test;
namespace mtf = mir_test_framework;

using namespace testing;
using namespace std::chrono_literals;

namespace
{
std::chrono::seconds const max_wait{4};

class SurfaceHandle
{
public:
    explicit SurfaceHandle(MirWindow* window) : window{window} {}
    ~SurfaceHandle() { if (window) mir_window_release_sync(window); }

    operator MirWindow*() const { return window; }

    void post_buffer()
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        mir_buffer_stream_swap_buffers_sync(mir_window_get_buffer_stream(window));
#pragma GCC diagnostic pop
    }

    SurfaceHandle(SurfaceHandle const&& that) : window{that.window} { window = nullptr; }
private:
    SurfaceHandle(SurfaceHandle const&) = delete;
    MirWindow* window;
};

struct MockClient
{
    explicit MockClient(char const* connect_string) :
        connection_{mir_connect_sync(connect_string, __PRETTY_FUNCTION__)}
    {
    }

    MockClient(MockClient&& source) :
        connection_{nullptr}
    {
        std::swap(connection_, source.connection_);
    }

    auto create_surface(int output_id) -> SurfaceHandle
    {
        auto const spec = mir_create_normal_window_spec(connection_, 800, 600);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        mir_window_spec_set_pixel_format(spec, mir_pixel_format_bgr_888);
#pragma GCC diagnostic pop
        mir_window_spec_set_fullscreen_on_output(spec, output_id);
        auto const window = mir_create_window_sync(spec);
        mir_window_spec_release(spec);

        mir_window_set_event_handler(window, on_surface_event, this);

        return SurfaceHandle{window};
    };

    void disconnect()
    {
        if (connection_)
            mir_connection_release(connection_);

        connection_ = nullptr;
    }

    MirConnection* connection()
    {
        return connection_;
    }

    ~MockClient() noexcept
    {
        disconnect();
    }

    MOCK_METHOD2(surface_event, void(MirWindow* window, const MirEvent* event));

private:
    MirConnection* connection_{nullptr};

    static void on_surface_event(MirWindow* window, const MirEvent* event, void* client_ptr)
    {
        static_cast<MockClient*>(client_ptr)->surface_event(window, event);
    }
};

struct OverridenSystemCompositorWindowManager : msh::SystemCompositorWindowManager
{
    OverridenSystemCompositorWindowManager(msh::FocusController* focus_controller,
            std::shared_ptr<msh::DisplayLayout> const& display_layout,
            std::shared_ptr<mir::scene::SessionCoordinator> const& session_coordinator) :
        SystemCompositorWindowManager(focus_controller, display_layout, session_coordinator)
    {
    }

    void modify_surface(
        std::shared_ptr<mir::scene::Session> const& /*session*/,
        std::shared_ptr<mir::scene::Surface> const& /*window*/,
        mir::shell::SurfaceSpecification const& modifications) override
    {
        std::unique_lock<decltype(mutex)> lock(mutex);
        last_mod = modifications;
    }

    std::mutex mutex;
    mir::shell::SurfaceSpecification last_mod;
};

struct SystemCompositorWindowManager : mtf::HeadlessTest
{
    void SetUp() override
    {
        add_to_environment("MIR_SERVER_NO_FILE", "");

        initial_display_layout({{{0, 0}, { 640,  480}}, {{480, 0}, {1920, 1080}}});

        server.override_the_window_manager_builder(
            [this](msh::FocusController* focus_controller)
            {
                wm = std::make_shared<OverridenSystemCompositorWindowManager>(
                         focus_controller,
                         server.the_shell_display_layout(),
                         server.the_session_coordinator());

                return wm;
            });

        start_server();
    }

    void TearDown() override
    {
        stop_server();
    }

    MockClient connect_client()
    {
        return MockClient(new_connection().c_str());
    }

    std::shared_ptr<OverridenSystemCompositorWindowManager> wm;
};

MATCHER_P(MirFocusEvent, expected, "")
{
    if (mir_event_get_type(arg) != mir_event_type_window)
        return false;

    auto window_event = mir_event_get_window_event(arg);
    auto attrib = mir_window_event_get_attribute(window_event);
    auto value = mir_window_event_get_attribute_value(window_event);

    return (attrib == mir_window_attrib_focus)
        && (value == expected);
}
}

TEST_F(SystemCompositorWindowManager, when_output_is_valid_surfaces_creation_succeeds)
{
    auto client = connect_client();

    auto surface1 = client.create_surface(1);
    auto surface2 = client.create_surface(2);

    EXPECT_TRUE(mir_window_is_valid(surface1));
    EXPECT_TRUE(mir_window_is_valid(surface2));
}

TEST_F(SystemCompositorWindowManager, when_output_ID_not_specified_surfaces_creation_fails)
{
    auto client = connect_client();

    auto window = client.create_surface(0);

    EXPECT_FALSE(mir_window_is_valid(window));
    EXPECT_THAT(mir_window_get_error_message(window), HasSubstr("An output ID must be specified"));
}

TEST_F(SystemCompositorWindowManager, if_a_surface_posts_client_gets_focus)
{
    auto client = connect_client();

    // Throw away all uninteresting window events
    EXPECT_CALL(client, surface_event(_, Not(MirFocusEvent(mir_window_focus_state_focused)))).Times(AnyNumber());

    mt::Signal signal;

    EXPECT_CALL(client, surface_event(_, MirFocusEvent(mir_window_focus_state_focused))).Times(1)
            .WillOnce(InvokeWithoutArgs([&] { signal.raise(); }));

    auto window = client.create_surface(1);
    window.post_buffer();

    signal.wait_for(1s);
}

TEST_F(SystemCompositorWindowManager, if_no_surface_posts_client_never_gets_focus)
{
    auto client = connect_client();

    // Throw away all uninteresting window events
    EXPECT_CALL(client, surface_event(_, Not(MirFocusEvent(mir_window_focus_state_focused)))).Times(AnyNumber());

    mt::Signal signal;

    ON_CALL(client, surface_event(_, MirFocusEvent(mir_window_focus_state_focused)))
            .WillByDefault(InvokeWithoutArgs([&] { signal.raise(); }));

    auto window = client.create_surface(1);

    EXPECT_FALSE(signal.wait_for(100ms)) << "Unexpected surface_focused event received";
}

TEST_F(SystemCompositorWindowManager, surface_gets_confine_pointer_set)
{
    auto client = connect_client();

    auto window = client.create_surface(1);

    MirWindowSpec* spec = mir_create_window_spec(client.connection());
    mir_window_spec_set_pointer_confinement(spec, mir_pointer_confined_to_window);

    mir_window_apply_spec(window, spec);
    mir_window_spec_release(spec);

    mt::spin_wait_for_condition_or_timeout([this]
    {
        std::unique_lock<decltype(wm->mutex)> lock(wm->mutex);
        return wm->last_mod.confine_pointer.is_set();
    }, std::chrono::seconds{max_wait});
}
