/*
 * Copyright © 2012 Canonical Ltd.
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
 * Authored by: Kevin DuBois   <kevin.dubois@canonical.com>
 */

#include "mir_toolkit/mir_client_library.h"
#include "graphics.h"

#ifdef NDEBUG
// We have no error handling so rely on assert() always being present.
#undef NDEBUG
#endif
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <thread>

static char const *socket_file = NULL;
static EGLDisplay disp;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
void create_and_run_scroll_surface(MirConnection *connection)
{
    /* egl setup */
    int major, minor, n, rc;
    EGLContext context;
    EGLSurface egl_surface;
    EGLConfig egl_config;
    EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE };
    EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

    rc = eglInitialize(disp, &major, &minor);
    assert(rc == EGL_TRUE);
    assert(major == 1);
    assert(minor == 4);

    rc = eglChooseConfig(disp, attribs, &egl_config, 1, &n);
    assert(rc == EGL_TRUE);
    assert(n == 1);

    MirPixelFormat pixel_format =
        mir_connection_get_egl_pixel_format(connection, disp, egl_config);

    printf("Mir chose pixel format %d\n", pixel_format);

    auto deleter = [](MirWindowSpec *spec) { mir_window_spec_release(spec); };
    std::unique_ptr<MirWindowSpec, decltype(deleter)> spec{
        mir_create_normal_window_spec(connection, 640, 480),
        deleter
    };

    assert(spec != nullptr);

    mir_window_spec_set_pixel_format(spec.get(), pixel_format);
    mir_window_spec_set_name(spec.get(), __PRETTY_FUNCTION__);
    mir_window_spec_set_buffer_usage(spec.get(), mir_buffer_usage_hardware);

    MirWindow *window = mir_create_window_sync(spec.get());
    spec.reset();

    assert(window != NULL);
    assert(mir_window_is_valid(window));
    assert(strcmp(mir_window_get_error_message(window), "") == 0);
    puts("Window created");

    EGLNativeWindowType native_window =
        (EGLNativeWindowType)mir_buffer_stream_get_egl_native_window(
            mir_window_get_buffer_stream(window));
    assert(native_window != (EGLNativeWindowType)NULL);

    egl_surface = eglCreateWindowSurface(disp, egl_config, native_window, NULL);
    assert(egl_surface != EGL_NO_SURFACE);

    context = eglCreateContext(disp, egl_config, EGL_NO_CONTEXT, context_attribs);
    assert(context != EGL_NO_CONTEXT);

    rc = eglMakeCurrent(disp, egl_surface, egl_surface, context);
    assert(rc == EGL_TRUE);

    mir::draw::glAnimationBasic gl_animation;
    gl_animation.init_gl();

    for(;;)
    {
        gl_animation.render_gl();
        rc = eglSwapBuffers(disp, egl_surface);
        assert(rc == EGL_TRUE);
        gl_animation.step();
    }

    eglDestroySurface(disp, egl_surface);
    eglDestroyContext(disp, context);
    
    mir_window_release_sync(window);
    puts("Window released");
}

static void shutdown(int signum)
{
    // Exiting without closing surfaces and connection is impolite but should
    // not kill the server. It actually did kill the server prior to -r 3385.
    // Running this example in CI acts as a smoke test for that failure.
    printf("Signal %d received. Good night.\n", signum);
    quick_exit(EXIT_SUCCESS); // quick_exit() doesn't seem to hit lp:1560943
}

int main(int argc, char* argv[])
{
    MirConnection *connection = 0;
    unsigned num_windows = 1;
    int arg;
    opterr = 0;
    while ((arg = getopt (argc, argv, "hm:w:")) != -1)
    {
        switch (arg)
        {
        case 'm':
            socket_file = optarg;
            break;
        case 'w':
            num_windows = atoi(optarg);
            break;
        case '?':
        case 'h':
        default:
            puts(argv[0]);
            puts("Usage:");
            puts("    -m <Mir server socket>");
            puts("    -w <Number of windows to create>:");
            puts("    -h: this help text");
            return -1;
        }
    }

    puts("Starting");

    signal(SIGINT, shutdown);
    signal(SIGTERM, shutdown);
    signal(SIGHUP, shutdown);

    connection = mir_connect_sync(socket_file, __PRETTY_FUNCTION__);
    assert(connection != NULL);
    assert(mir_connection_is_valid(connection));
    assert(strcmp(mir_connection_get_error_message(connection), "") == 0);
    puts("Connected");

    EGLNativeDisplayType native_display = (EGLNativeDisplayType) mir_connection_get_egl_native_display(connection);
    disp = eglGetDisplay(native_display);
    assert(disp != EGL_NO_DISPLAY);

    if (num_windows == 1)
    {
        create_and_run_scroll_surface(connection);
    }
    else
    {
        for (unsigned i = 0; i < num_windows; i++) std::thread(create_and_run_scroll_surface, connection).detach();
        for(;;) {}
    }
    

    eglTerminate(disp);

    mir_connection_release(connection);
    puts("Connection released");

    return 0;
}
#pragma GCC diagnostic pop


