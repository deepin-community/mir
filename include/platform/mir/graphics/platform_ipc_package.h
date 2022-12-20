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
 * Authored by: Alexandros Frantzis <alexandros.frantzis@canonical.com>
 */

#ifndef MIR_GRAPHICS_PLATFORM_IPC_PACKAGE_H_
#define MIR_GRAPHICS_PLATFORM_IPC_PACKAGE_H_

#include <vector>
#include <cinttypes>

namespace mir
{
struct ModuleProperties;

namespace graphics
{

/**
 * Platform data to be sent to the clients over IPC.
 */
struct PlatformIPCPackage
{
    PlatformIPCPackage() : graphics_module(nullptr) {}
    explicit PlatformIPCPackage(ModuleProperties const* graphics_module) : graphics_module(graphics_module) {}

    std::vector<int32_t> ipc_data;
    std::vector<int32_t> ipc_fds;

    ModuleProperties const* graphics_module;
};

}
}

#endif /* MIR_GRAPHICS_PLATFORM_IPC_PACKAGE_H_ */

