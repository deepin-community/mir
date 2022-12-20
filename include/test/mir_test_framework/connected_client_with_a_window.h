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
 * Authored By: Alan Griffiths <alan@octopull.co.uk>
 */

#ifndef MIR_TEST_FRAMEWORK_CONNECTED_CLIENT_WITH_A_WINDOW_H_
#define MIR_TEST_FRAMEWORK_CONNECTED_CLIENT_WITH_A_WINDOW_H_

#include "mir_test_framework/connected_client_headless_server.h"
#include "mir/geometry/size.h"

namespace mir_test_framework
{
struct ConnectedClientWithAWindow : ConnectedClientHeadlessServer
{
    MirWindow* window{nullptr};

    void SetUp() override;

    void TearDown() override;

    mir::geometry::Size const surface_size {640, 480};
};
}

#endif /* MIR_TEST_FRAMEWORK_CONNECTED_CLIENT_WITH_A_WINDOW_H_ */
