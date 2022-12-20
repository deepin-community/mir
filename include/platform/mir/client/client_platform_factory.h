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
#ifndef MIR_CLIENT_CLIENT_PLATFORM_FACTORY_H_
#define MIR_CLIENT_CLIENT_PLATFORM_FACTORY_H_

#include "mir/module_deleter.h"
#include <memory>

namespace mir
{
namespace logging { class Logger; }
namespace client
{

class ClientPlatform;
class ClientContext;

class ClientPlatformFactory
{
public:
    virtual ~ClientPlatformFactory() = default;

    virtual std::shared_ptr<ClientPlatform> create_client_platform(ClientContext* context) = 0;

protected:
    ClientPlatformFactory() = default;
    ClientPlatformFactory(ClientPlatformFactory const& p) = delete;
    ClientPlatformFactory& operator=(ClientPlatformFactory const& p) = delete;
};

extern "C" typedef mir::UniqueModulePtr<ClientPlatform>(*CreateClientPlatform)(
               ClientContext* context,
               std::shared_ptr<mir::logging::Logger> const& logger);
extern "C" typedef bool (*ClientPlatformProbe)(ClientContext* context);
}
}

#if defined(__clang__)
#pragma clang diagnostic push
// These functions are given "C" linkage to avoid name-mangling, not for C compatibility.
// (We don't want a warning for doing this intentionally.)
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif
extern "C" mir::UniqueModulePtr<mir::client::ClientPlatform> create_client_platform(
               mir::client::ClientContext* context,
               std::shared_ptr<mir::logging::Logger> const& logger);
extern "C" bool is_appropriate_module(mir::client::ClientContext* context);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif /* MIR_CLIENT_CLIENT_PLATFORM_FACTORY_H_ */
