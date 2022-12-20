/* Generated by wayland-scanner 1.16.0 */

#ifndef WL_EGLSTREAM_CONTROLLER_SERVER_PROTOCOL_H
#define WL_EGLSTREAM_CONTROLLER_SERVER_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include "wayland-server.h"

#ifdef  __cplusplus
extern "C" {
#endif

struct wl_client;
struct wl_resource;

/**
 * @page page_wl_eglstream_controller The wl_eglstream_controller protocol
 * @section page_ifaces_wl_eglstream_controller Interfaces
 * - @subpage page_iface_wl_eglstream_controller - 
 * @section page_copyright_wl_eglstream_controller Copyright
 * <pre>
 *
 * Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * </pre>
 */
struct wl_buffer;
struct wl_eglstream_controller;
struct wl_surface;

/**
 * @page page_iface_wl_eglstream_controller wl_eglstream_controller
 * @section page_iface_wl_eglstream_controller_api API
 * See @ref iface_wl_eglstream_controller.
 */
/**
 * @defgroup iface_wl_eglstream_controller The wl_eglstream_controller interface
 */
extern const struct wl_interface wl_eglstream_controller_interface;

/**
 * @ingroup iface_wl_eglstream_controller
 * @struct wl_eglstream_controller_interface
 */
struct wl_eglstream_controller_interface {
	/**
	 * Create server stream and attach consumer
	 *
	 * Creates the corresponding server side EGLStream from the given
	 * wl_buffer and attaches a consumer to it.
	 * @param wl_surface wl_surface corresponds to the client surface associated with         newly created eglstream
	 * @param wl_resource wl_resource corresponding to an EGLStream
	 */
	void (*attach_eglstream_consumer)(struct wl_client *client,
					  struct wl_resource *resource,
					  struct wl_resource *wl_surface,
					  struct wl_resource *wl_resource);
};


/**
 * @ingroup iface_wl_eglstream_controller
 */
#define WL_EGLSTREAM_CONTROLLER_ATTACH_EGLSTREAM_CONSUMER_SINCE_VERSION 1

#ifdef  __cplusplus
}
#endif

#endif
