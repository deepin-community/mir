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
 * Authored by: Andreas Pokorny <andreas.pokorny@canonical.com>
 */

#ifndef MIR_COMMON_INPUT_DEVICE_STATE_EVENT_H_
#define MIR_COMMON_INPUT_DEVICE_STATE_EVENT_H_

#include <chrono>
#include <limits>
#include <unordered_map>

#include "mir/events/event.h"
#include "mir/events/input_device_state_event.h"

struct MirInputDeviceStateEvent : MirEvent
{
    MirInputDeviceStateEvent();

    MirPointerButtons pointer_buttons() const;
    void set_pointer_buttons(MirPointerButtons buttons);

    float pointer_axis(MirPointerAxis axis) const;
    void set_pointer_axis(MirPointerButtons axis, float value);

    std::chrono::nanoseconds when() const;
    void set_when(std::chrono::nanoseconds const& when);

    MirInputEventModifiers modifiers() const;
    void set_modifiers(MirInputEventModifiers modifiers);

    uint32_t device_count() const;
    MirInputDeviceId device_id(size_t index) const;
    MirPointerButtons device_pointer_buttons(size_t index) const;

    uint32_t device_pressed_keys_for_index(size_t index, size_t pressed_index) const;
    uint32_t device_pressed_keys_count(size_t index) const;

    void set_device_states(std::vector<mir::events::InputDeviceState> const& device_states);
    void set_window_id(int id);
    int window_id() const;
};

#endif /* MIR_COMMON_INPUT_DEVICE_STATE_EVENT_H_*/
