/*
 * Copyright © 2016 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2 or 3 as
 * published by the Free Software Foundation.
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

#ifndef MIR_CLIENT_MIR_BUFFER_H
#define MIR_CLIENT_MIR_BUFFER_H

#include "mir_toolkit/mir_buffer.h"
#include "mir_toolkit/mir_native_buffer.h"
#include "mir/geometry/size.h"
#include "mir/fd.h"
#include <memory>
#include <chrono>

namespace mir
{
namespace client
{
class ClientBuffer;
class MemoryRegion;

class MirBuffer
{
public:
    virtual ~MirBuffer() = default;
    virtual int rpc_id() const = 0;
    virtual void submitted() = 0;
    virtual void received() = 0;
    virtual void received(MirBufferPackage const& update_message) = 0;
    virtual std::shared_ptr<ClientBuffer> client_buffer() const = 0;
    virtual MirGraphicsRegion map_region() = 0;
    virtual void unmap_region() = 0;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    virtual MirBufferUsage buffer_usage() const = 0;
#pragma GCC diagnostic pop
    virtual MirPixelFormat pixel_format() const = 0;
    virtual geometry::Size size() const = 0;
    virtual MirConnection* allocating_connection() const = 0;
    virtual void increment_age() = 0;
    virtual bool valid() const = 0;
    virtual char const* error_message() const = 0;
    virtual void set_callback(MirBufferCallback callback, void* context) = 0;
protected:
    MirBuffer() = default;
    MirBuffer(MirBuffer const&) = delete;
    MirBuffer& operator=(MirBuffer const&) = delete;
};
}
}
#endif /* MIR_CLIENT_BUFFER_H_ */
