/*
 * Copyright © 2014 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 or 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Christopher James Halse Rogers <christopher.halse.rogers@canonical.com>>
 */

#include "mir/client/client_platform_factory.h"
#include "mir/client/client_context.h"

#include "mir_test_framework/stub_client_platform_factory.h"
#include "mir_test_framework/stub_platform_helpers.h"
#include "mir_test_framework/stub_client_platform_options.h"
#include "mir/assert_module_entry_point.h"

#include <memory>
#include <unordered_map>
#include <exception>
#include <gmock/gmock.h>

namespace mtf = mir_test_framework;
namespace mcl = mir::client;

namespace
{
std::unordered_map<mtf::FailurePoint, std::exception_ptr, std::hash<int>> next_error_set;
}

extern "C" void add_client_platform_error(mtf::FailurePoint where, std::exception_ptr what)
{
    next_error_set[where] = what;
}

mir::UniqueModulePtr<mcl::ClientPlatform> create_client_platform(
    mcl::ClientContext* context,
    std::shared_ptr<mir::logging::Logger> const& /*logger*/)
{
    mir::assert_entry_point_signature<mcl::CreateClientPlatform>(&create_client_platform);

    auto error_set = std::move(next_error_set);
    next_error_set = {};

    return mir::make_module_ptr<mtf::StubClientPlatform>(context, std::move(error_set));
}

bool is_appropriate_module(mcl::ClientContext* context)
{
    using namespace testing;
    mir::assert_entry_point_signature<mcl::ClientPlatformProbe>(&is_appropriate_module);
    MirPlatformPackage package;
    context->populate_server_package(package);
    MirModuleProperties module_properties;
    context->populate_graphics_module(module_properties);
    return
        Matches(mtf::IsStubPlatformPackage())(package) &&
        (strcmp(module_properties.name, "mir:stub-graphics") == 0);
}
