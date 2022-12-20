/*
 * Copyright © 2012 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2 or 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Kevin DuBois <kevin.dubois@canonical.com>
 */
#ifndef MIR_CLIENT_CLIENT_PLATFORM_H_
#define MIR_CLIENT_CLIENT_PLATFORM_H_

#include "mir/graphics/native_buffer.h"
#include "mir/geometry/size.h"
#include "mir_toolkit/client_types.h"
#include "mir_toolkit/mir_native_buffer.h"

#include <EGL/eglplatform.h>
#include <EGL/egl.h>  // for EGLConfig
#include <memory>

namespace mir
{
namespace client
{
class ClientBufferFactory;
class EGLNativeSurface;
class ClientContext;

/**
 * Interface to client-side platform specific support for graphics operations.
 * \ingroup platform_enablement
 */
class ClientPlatform
{
public:
    ClientPlatform() = default;
    ClientPlatform(const ClientPlatform& p) = delete;
    ClientPlatform& operator=(const ClientPlatform& p) = delete;

    virtual ~ClientPlatform() = default;

    virtual MirPlatformType platform_type() const = 0;
    virtual void populate(MirPlatformPackage& package) const = 0;
    /**
     * Perform a platform operation.
     *
     * The returned platform message is owned by the caller and should be
     * released with mir_platform_message_release().
     *
     *   \param [in] request      The platform operation request
     *   \return                  The platform operation reply, or a nullptr if the
     *                            requested operation is not supported
     */
    virtual MirPlatformMessage* platform_operation(MirPlatformMessage const* request) = 0;
    virtual std::shared_ptr<ClientBufferFactory> create_buffer_factory() = 0;

    virtual void* request_interface(char const* name, int version) = 0;

    // EGLNativeWindowType differs between platforms, so can't reasonably be used in the
    // platform-independent interface.
    // The following will be deprecated soon in favor of request_interface
    virtual std::shared_ptr<void> create_egl_native_window(EGLNativeSurface* surface) = 0;
    virtual void use_egl_native_window(std::shared_ptr<void> native_window, EGLNativeSurface* surface) = 0;
    virtual std::shared_ptr<EGLNativeDisplayType> create_egl_native_display() = 0;
    virtual MirNativeBuffer* convert_native_buffer(graphics::NativeBuffer*) const = 0;
    virtual MirPixelFormat get_egl_pixel_format(EGLDisplay, EGLConfig) const = 0;
    virtual uint32_t native_format_for(MirPixelFormat) const = 0;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    virtual uint32_t native_flags_for(MirBufferUsage, mir::geometry::Size) const = 0;
#pragma GCC diagnostic pop
};

}
}

#endif /* MIR_CLIENT_CLIENT_PLATFORM_H_ */
