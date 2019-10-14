/*
 * Copyright (c) 2017 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "system_network_internal.h"

namespace particle {

namespace {

bool networkOff(network_interface_index iface) {
    if (network_ready(iface, NETWORK_READY_TYPE_ANY, nullptr)) {
        network_off(iface, 0, 0, nullptr);
        return true;
    }
    return false;
}

void networkOn(network_interface_index iface) {
    network_on(iface, 0, 0, nullptr);
}

} // namespace

void resetNetworkInterfaces() {
    // TODO: There's no cross-platform API to enumerate available network interfaces
#if HAL_PLATFORM_MESH
    const bool resetMesh = networkOff(NETWORK_INTERFACE_MESH);
#endif // HAL_PLATFORM_MESH
#if HAL_PLATFORM_ETHERNET
    const bool resetEthernet = networkOff(NETWORK_INTERFACE_ETHERNET);
#endif // HAL_PLATFORM_ETHERNET
#if HAL_PLATFORM_CELLULAR
    const bool resetCellular = networkOff(NETWORK_INTERFACE_CELLULAR);
#endif // HAL_PLATFORM_CELLULAR
#if HAL_PLATFORM_WIFI
    const bool resetWifi = networkOff(NETWORK_INTERFACE_WIFI_STA);
#endif // HAL_PLATFORM_WIFI
#if HAL_PLATFORM_MESH
    if (resetMesh) {
        networkOn(NETWORK_INTERFACE_MESH);
    }
#endif // HAL_PLATFORM_MESH
#if HAL_PLATFORM_ETHERNET
    if (resetEthernet) {
        networkOn(NETWORK_INTERFACE_ETHERNET);
    }
#endif // HAL_PLATFORM_ETHERNET
#if HAL_PLATFORM_CELLULAR
    if (resetCellular) {
        networkOn(NETWORK_INTERFACE_CELLULAR);
    }
#endif // HAL_PLATFORM_CELLULAR
#if HAL_PLATFORM_WIFI
    if (resetWifi) {
        networkOn(NETWORK_INTERFACE_WIFI_STA);
    }
#endif // HAL_PLATFORM_WIFI
}

} // namespace particle

/* FIXME: there should be a define that tells whether there is NetworkManager available
 * or not */
#if !HAL_PLATFORM_IFAPI

namespace {

using namespace particle;

NetworkDiagnostics g_networkDiagnostics;

} // namespace

particle::NetworkDiagnostics* particle::NetworkDiagnostics::instance() {
    return &g_networkDiagnostics;
}

#endif /* !HAL_PLATFORM_IFAPI */
