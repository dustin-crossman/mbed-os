/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "onboard_modem_api.h"

#include "SARA4_PPP.h"
#include "SARA4_PPP_CellularNetwork.h"

using namespace mbed;
using namespace events;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
};

SARA4_PPP::SARA4_PPP(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

SARA4_PPP::~SARA4_PPP()
{
}

AT_CellularNetwork *SARA4_PPP::open_network_impl(ATHandler &at)
{
    return new SARA4_PPP_CellularNetwork(at);
}

nsapi_error_t SARA4_PPP::power_on()
{
#if MODEM_ON_BOARD
    ::onboard_modem_init();
    ::onboard_modem_power_up();
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t SARA4_PPP::power_off()
{
#if MODEM_ON_BOARD
    ::onboard_modem_power_down();
#endif
    return NSAPI_ERROR_OK;
}
