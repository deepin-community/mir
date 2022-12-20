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

#ifndef MIR_CLIENT_EGL_NATIVE_SURFACE_H_
#define MIR_CLIENT_EGL_NATIVE_SURFACE_H_

#include "mir_toolkit/client_types.h"
#include "mir/geometry/size.h"
#include <memory>

namespace mir
{
namespace client
{
class ClientBuffer;
class EGLNativeSurface
{
  public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    virtual MirWindowParameters get_parameters() const = 0;
#pragma GCC diagnostic pop
    virtual std::shared_ptr<ClientBuffer> get_current_buffer() = 0;
    virtual void swap_buffers_sync() = 0;
    virtual void request_and_wait_for_configure(MirWindowAttrib a, int value) = 0;
    virtual void set_buffer_cache_size(unsigned int) = 0;
    virtual void set_size(geometry::Size) = 0;

  protected:
    EGLNativeSurface() = default;
    virtual ~EGLNativeSurface() = default;
    EGLNativeSurface(const EGLNativeSurface&) = delete;
    EGLNativeSurface& operator=(const EGLNativeSurface&) = delete;
};

}
}

#endif /* MIR_CLIENT_EGL_NATIVE_SURFACE_H_ */
