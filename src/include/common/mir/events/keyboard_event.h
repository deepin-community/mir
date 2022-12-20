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
 * Authored by: Brandon Schaefer <brandon.schaefer@canonical.com>
 */

#ifndef MIR_COMMON_KEYBOARD_EVENT_H_
#define MIR_COMMON_KEYBOARD_EVENT_H_

#include <chrono>
#include <cstdint>
#include <vector>

#include "mir/events/input_event.h"

struct MirKeyboardEvent : MirInputEvent
{
    MirKeyboardEvent();

    MirKeyboardAction action() const;
    void set_action(MirKeyboardAction action);

    int32_t key_code() const;
    void set_key_code(int32_t key_code);

    int32_t scan_code() const;
    void set_scan_code(int32_t scan_code);

    char const* text() const;
    void set_text(char const* str);
};

#endif /* MIR_COMMON_KEYBOARD_EVENT_H_ */
