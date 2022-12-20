/*
 * Copyright © 2016 Canonical Ltd.
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
 * Authored by: Daniel van Vugt <daniel.van.vugt@canonical.com>
 */

#ifndef MIR_GRAPHICS_ATOMIC_FRAME_H_
#define MIR_GRAPHICS_ATOMIC_FRAME_H_

#include "mir/graphics/frame.h"
#include <mutex>

namespace mir { namespace graphics {

class AtomicFrame
{
public:
    Frame load() const;
    // Preferably use this and provide all fields from the driver:
    void store(Frame const&);
    // Or if your driver is limited these will suffice:
    void increment_now();
    void increment_with_timestamp(Frame::Timestamp t);
private:
    mutable std::mutex mutex;
    Frame frame;
};

}} // namespace mir::graphics

#endif // MIR_GRAPHICS_ATOMIC_FRAME_H_
