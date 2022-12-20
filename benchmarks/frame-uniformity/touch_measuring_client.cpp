/*
 * Copyright © 2014 Canonical Ltd.
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
 * Authored by: Robert Carr <robert.carr@canonical.com>
 */

#include "touch_measuring_client.h"

#include "mir/time/clock.h"

#include "mir_toolkit/mir_client_library.h"

#include <iostream>

namespace mt = mir::test;

namespace
{

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
MirWindow *create_window(MirConnection *connection)
{
    MirPixelFormat pixel_format;
    unsigned int valid_formats;
    mir_connection_get_available_surface_formats(connection, &pixel_format, 1, &valid_formats);

    auto const spec = mir_create_normal_window_spec(connection, 1024, 1024);
    mir_window_spec_set_pixel_format(spec, pixel_format);
    mir_window_spec_set_name(spec, "frame-uniformity-test");
    mir_window_spec_set_buffer_usage(spec, mir_buffer_usage_hardware);

    auto window = mir_create_window_sync(spec);
    mir_window_spec_release(spec);
    
    if (!mir_window_is_valid(window))
    {
        std::cerr << "Window creation failed: " << mir_window_get_error_message(window) << std::endl;
        exit(1);
    }

    return window;
}

void input_callback(MirWindow * /* surface */, MirEvent const* event, void* context)
{
    auto results = static_cast<TouchSamples*>(context);
    
    results->record_pointer_coordinates(std::chrono::high_resolution_clock::now(), *event);
}

void collect_input_and_frame_timing(MirWindow *surface, mt::Barrier& client_ready, std::chrono::high_resolution_clock::duration duration, std::shared_ptr<TouchSamples> const& results)
{
    mir_window_set_event_handler(surface, input_callback, results.get());
    
    client_ready.ready();

    // May be better if end time were relative to the first input event
    auto end_time = std::chrono::high_resolution_clock::now() + duration;
    while (std::chrono::high_resolution_clock::now() < end_time)
    {
        mir_buffer_stream_swap_buffers_sync(mir_window_get_buffer_stream(surface));
        results->record_frame_time(std::chrono::high_resolution_clock::now());
    }
}
#pragma GCC diagnostic pop

}

TouchMeasuringClient::TouchMeasuringClient(mt::Barrier& client_ready,
    std::chrono::high_resolution_clock::duration const& touch_duration)
    : client_ready(client_ready),
      touch_duration(touch_duration),
      results_(std::make_shared<TouchSamples>())
{
}

namespace
{
void null_lifecycle_callback(MirConnection*, MirLifecycleState, void*)
{
}
}

void TouchMeasuringClient::run(std::string const& connect_string)
{
    auto connection = mir_connect_sync(connect_string.c_str(), "frame-uniformity-test");
    if (!mir_connection_is_valid(connection))
    {
        std::cerr << "Connection to Mir failed: " << mir_connection_get_error_message(connection) << std::endl;
        exit(1);
    }
    
    /*
     * Set a null callback to avoid killing the process
     * (default callback raises SIGHUP).
     */
    mir_connection_set_lifecycle_event_callback(connection, null_lifecycle_callback, nullptr);
    
    auto window = create_window(connection);

    collect_input_and_frame_timing(window, client_ready, touch_duration, results_);
    
    mir_window_release_sync(window);
    mir_connection_release(connection);
}

std::shared_ptr<TouchSamples> TouchMeasuringClient::results()
{
    return results_;
}
