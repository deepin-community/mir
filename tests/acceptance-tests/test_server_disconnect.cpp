/*
 * Copyright © 2013 Canonical Ltd.
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

#include "mir_toolkit/mir_client_library.h"

#include "mir_test_framework/interprocess_client_server_test.h"
#include "mir/test/cross_process_sync.h"
#include "mir_test_framework/process.h"
#include "mir_test_framework/any_surface.h"
#include "mir/test/cross_process_action.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <atomic>
#include <future>

namespace mtf = mir_test_framework;
namespace mt = mir::test;
using namespace testing;

namespace
{
struct ServerDisconnect : mtf::InterprocessClientServerTest
{
    void SetUp() override
    {
        mtf::InterprocessClientServerTest::SetUp();
        run_in_server([]{});
    }
};

struct MockEventHandler
{
    MOCK_METHOD1(handle, void(MirLifecycleState transition));

    static void handle(MirConnection*, MirLifecycleState transition, void* event_handler)
    {
        static_cast<MockEventHandler*>(event_handler)->handle(transition);
    }
};

void null_lifecycle_callback(MirConnection*, MirLifecycleState, void*)
{
}
}

TEST_F(ServerDisconnect, is_detected_by_client)
{
    if (is_test_process())
    {
        MockEventHandler mock_event_handler;

        auto connection = mir_connect_sync(mtf::test_socket_file().c_str() , __PRETTY_FUNCTION__);
        mir_connection_set_lifecycle_event_callback(connection, &MockEventHandler::handle, &mock_event_handler);
        auto window = mtf::make_any_surface(connection);

        std::atomic<bool> signalled(false);

        EXPECT_CALL(mock_event_handler, handle(mir_lifecycle_connection_lost)).Times(1).
            WillOnce(testing::InvokeWithoutArgs([&] { signalled.store(true); }));

        using clock = std::chrono::high_resolution_clock;

        auto const time_limit = clock::now() + std::chrono::seconds(2);

        auto const server_stopper = std::async(std::launch::async, [this] { stop_server(); });

        while (!signalled.load() && clock::now() < time_limit)
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            mir_buffer_stream_swap_buffers_sync(
                mir_window_get_buffer_stream(window));
#pragma GCC diagnostic pop
        }

        mir_window_release_sync(window);
        mir_connection_release(connection);
    }
}

TEST_F(ServerDisconnect, doesnt_stop_client_calling_API_functions)
{
    mt::CrossProcessAction connect;
    mt::CrossProcessAction create_surface;
    mt::CrossProcessAction configure_display;
    mt::CrossProcessAction disconnect;

    auto const client = new_client_process([&]
        {
            MirConnection* connection{nullptr};

            connect.exec([&] {
                connection = mir_connect_sync(mtf::test_socket_file().c_str() , __PRETTY_FUNCTION__);
                EXPECT_TRUE(mir_connection_is_valid(connection));
                /*
                 * Set a null callback to avoid killing the process
                 * (default callback raises SIGHUP).
                 */
                mir_connection_set_lifecycle_event_callback(connection,
                                                            null_lifecycle_callback,
                                                            nullptr);
            });

            MirWindow* window;

            create_surface.exec([&] {
                auto spec = mir_create_normal_window_spec(connection, 800, 600);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
                mir_window_spec_set_pixel_format(spec, mir_pixel_format_xbgr_8888);
#pragma GCC diagnostic pop
                window = mir_create_window_sync(spec);
                mir_window_spec_release(spec);
            });

            configure_display.exec([&] {
                auto config = mir_connection_create_display_configuration(connection);
                mir_connection_apply_session_display_config(connection, config);
                mir_display_config_release(config);
            });

            disconnect.exec([&] {
                mir_window_release_sync(window);
                mir_connection_release(connection);
            });
        });

    if (is_test_process())
    {
        connect();
        stop_server();
        /* While trying to create a window the connection break will be detected */
        create_surface();

        /* Trying to configure the display shouldn't block */
        configure_display();
        /* Trying to disconnect at this point shouldn't block */
        disconnect();

        EXPECT_THAT(client->wait_for_termination().exit_code, Eq(EXIT_SUCCESS));
    }
}

using ServerDisconnectDeathTest = ServerDisconnect;

TEST_F(ServerDisconnectDeathTest, causes_client_to_terminate_by_default)
{
    mt::CrossProcessAction connect;
    mt::CrossProcessSync create_surface_sync;

    auto const client = new_client_process([&]
        {
            MirConnection* connection{nullptr};

            connect.exec([&] {
                connection = mir_connect_sync(mtf::test_socket_file().c_str() , __PRETTY_FUNCTION__);
                EXPECT_TRUE(mir_connection_is_valid(connection));
            });

            create_surface_sync.wait_for_signal_ready_for();

            mtf::make_any_surface(connection);

            mir_connection_release(connection);
        });

    if (is_test_process())
    {
        connect();
        stop_server();

        /*
         * While trying to create a window the connection break will be detected
         * and the client should self-terminate.
         */
        create_surface_sync.signal_ready();

        auto const client_result = client->wait_for_termination();
        EXPECT_EQ(mtf::TerminationReason::child_terminated_by_signal,
                  client_result.reason);
        int sig = client_result.signal;
        EXPECT_TRUE(sig == SIGHUP || sig == SIGKILL /* (Valgrind) */);
    }
}
