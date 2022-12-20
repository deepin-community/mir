/*
 * Copyright © 2015 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 or 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Cemil Azizoglu <cemil.azizoglu@canonical.com>
 */

#ifndef TEST_DISPLAY_H_
#define TEST_DISPLAY_H_

#include "mir/test/doubles/mock_display_configuration.h"
#include "mir/test/display_config_matchers.h"
#include "mir/renderer/gl/context.h"
#include "mir/renderer/gl/context_source.h"

namespace
{
auto as_context_source(mg::Display* display)
{
    auto const ctx = dynamic_cast<mir::renderer::gl::ContextSource*>(display->native_display());
    if (!ctx)
        BOOST_THROW_EXCEPTION(std::logic_error("Display does not support GL rendering"));
    return ctx;
}
}

TEST_F(DisplayTestGeneric, configure_disallows_invalid_configuration)
{
    using namespace testing;
    auto display = create_display();
    mir::test::doubles::MockDisplayConfiguration config;

    EXPECT_CALL(config, valid())
        .WillOnce(Return(false));

    EXPECT_THROW({display->configure(config);}, std::logic_error);

    // Determining what counts as a valid configuration is a much trickier
    // platform-dependent exercise, so won't be tested here.
}

#ifdef MIR_DISABLE_TESTS_ON_X11
TEST_F(DisplayTestGeneric, DISABLED_gl_context_make_current_uses_shared_context)
#else
TEST_F(DisplayTestGeneric, gl_context_make_current_uses_shared_context)
#endif
{
    using namespace testing;
    EGLContext const shared_context{reinterpret_cast<EGLContext>(0x111)};
    EGLContext const display_buffer_context{reinterpret_cast<EGLContext>(0x222)};
    EGLContext const new_context{reinterpret_cast<EGLContext>(0x333)};

    EXPECT_CALL(mock_egl, eglCreateContext(_,_,EGL_NO_CONTEXT,_))
        .WillOnce(Return(shared_context));
    EXPECT_CALL(mock_egl, eglCreateContext(_,_,shared_context,_))
        .WillOnce(Return(display_buffer_context));

    auto display = create_display();

    Mock::VerifyAndClearExpectations(&mock_egl);

    {
        InSequence s;
        EXPECT_CALL(mock_egl, eglCreateContext(_,_,shared_context,_))
            .WillOnce(Return(new_context));
        EXPECT_CALL(mock_egl, eglMakeCurrent(_,_,_,new_context));
        EXPECT_CALL(mock_egl, eglGetCurrentContext())
           .WillOnce(Return(new_context));
        EXPECT_CALL(mock_egl, eglMakeCurrent(_,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT));

        auto const gl_ctx = as_context_source(display.get())->create_gl_context();

        ASSERT_NE(nullptr, gl_ctx);

        gl_ctx->make_current();
    }

    Mock::VerifyAndClearExpectations(&mock_egl);

    /* Possible display shutdown sequence, depending on the platform */
    EXPECT_CALL(mock_egl, eglGetCurrentContext())
        .Times(AtLeast(0));
    EXPECT_CALL(mock_egl, eglMakeCurrent(_,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT))
        .Times(AtLeast(0));
}

TEST_F(DisplayTestGeneric, gl_context_releases_context)
{
    using namespace testing;

    auto display = create_display();

    {
        InSequence s;
        EXPECT_CALL(mock_egl, eglMakeCurrent(_,_,_,Ne(EGL_NO_CONTEXT)));
        EXPECT_CALL(mock_egl, eglMakeCurrent(_,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT));

        auto const gl_ctx = as_context_source(display.get())->create_gl_context();

        ASSERT_NE(nullptr, gl_ctx);

        gl_ctx->make_current();
        gl_ctx->release_current();

        Mock::VerifyAndClearExpectations(&mock_egl);
    }

    /* Possible display shutdown sequence, depending on the platform */
    EXPECT_CALL(mock_egl, eglMakeCurrent(_,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT))
        .Times(AtLeast(0));
}

#ifdef MIR_DISABLE_TESTS_ON_X11
TEST_F(DisplayTestGeneric, DISABLED_does_not_expose_display_buffer_for_output_with_power_mode_off)
#else
TEST_F(DisplayTestGeneric, does_not_expose_display_buffer_for_output_with_power_mode_off)
#endif
{
    using namespace testing;
    auto display = create_display();
    int db_count{0};

    display->for_each_display_sync_group([&](mg::DisplaySyncGroup& group) {
        group.for_each_display_buffer([&] (mg::DisplayBuffer&) { ++db_count; });
    });
    EXPECT_THAT(db_count, Eq(1));

    auto conf = display->configuration();
    conf->for_each_output(
        [] (mg::UserDisplayConfigurationOutput& output)
        {
            output.power_mode = mir_power_mode_off;
        });

    display->configure(*conf);

    db_count = 0;
    display->for_each_display_sync_group([&](mg::DisplaySyncGroup& group) {
        group.for_each_display_buffer([&] (mg::DisplayBuffer&) { ++db_count; });
    });
    EXPECT_THAT(db_count, Eq(0));
}

TEST_F(DisplayTestGeneric,
       returns_configuration_whose_clone_matches_original_configuration)
{
    using namespace testing;

    auto display = create_display();

    auto config = display->configuration();
    auto cloned_config = config->clone();

    EXPECT_THAT(*cloned_config, mir::test::DisplayConfigMatches(std::cref(*config)));
}

TEST_F(DisplayTestGeneric,
       returns_configuration_whose_clone_is_independent_of_original_configuration)
{
    using namespace testing;

    auto display = create_display();

    auto config = display->configuration();
    auto cloned_config = config->clone();

    config->for_each_output(
        [] (mg::UserDisplayConfigurationOutput& output)
        {
            output.power_mode = mir_power_mode_off;
        });

    cloned_config->for_each_output(
        [] (mg::UserDisplayConfigurationOutput& output)
        {
            output.power_mode = mir_power_mode_on;
        });

    // Check that changes to cloned_config haven't affected original config
    config->for_each_output(
        [] (mg::DisplayConfigurationOutput const& output)
        {
            EXPECT_THAT(output.power_mode, Eq(mir_power_mode_off));
        });
}

#endif // TEST_DISPLAY_H_
