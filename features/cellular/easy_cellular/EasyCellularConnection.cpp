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

#include "CellularTargets.h"
#ifdef CELLULAR_DEVICE

#if NSAPI_PPP_AVAILABLE
#include "nsapi_ppp.h"
#endif

#include "CellularConnectionUtil.h"
#include "CellularUtil.h"

#include "EasyCellularConnection.h"
#include "CellularLog.h"

#if MBED_CONF_APP_CELLULAR_USE_APN_LOOKUP
#include "APN_db.h"
#endif //MBED_CONF_APP_CELLULAR_USE_APN_LOOKUP

namespace mbed {

bool EasyCellularConnection::cellular_status(int state, int next_state)
{
    tr_info("cellular_status %d=>%d", state, next_state);
    if (_target_state == state) {
        if (state == CellularConnectionUtil::STATE_CONNECTED) {
            _is_connected = true;
        } else {
            _is_connected = false;
        }
        MBED_ASSERT(_cellularSemaphore.release() == osOK);
        return false;
    }
    else {
        _is_connected = false;
    }
    return true;
}

EasyCellularConnection::EasyCellularConnection() :  _is_connected(false), _is_initialized(false),
        _credentials_set(false), _target_state(CellularConnectionUtil::STATE_POWER_ON),
        _cellularSerial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE),
        _cellularSemaphore(0), _cellularConnectionUtil(), _credentials_err(NSAPI_ERROR_OK)
{
    tr_info("EasyCellularConnection()");
}

EasyCellularConnection::~EasyCellularConnection()
{
    _cellularConnectionUtil.stop();
}

nsapi_error_t EasyCellularConnection::init()
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    if (!_is_initialized) {
#if defined (MDMRTS) && defined (MDMCTS)
        _cellularSerial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
        _cellularConnectionUtil.set_serial(&_cellularSerial);
        _cellularConnectionUtil.set_callback(callback(this, &EasyCellularConnection::cellular_status));

       err = _cellularConnectionUtil.init();

        if (err == NSAPI_ERROR_OK) {
            err = _cellularConnectionUtil.start_dispatch();
        }
        _is_initialized = true;
    }

    return err;
}

void EasyCellularConnection::set_credentials(const char *apn, const char *uname, const char *pwd)
{
    if (apn && strlen(apn) > 0) {
        _credentials_err = init();

        if (_credentials_err) {
            return;
        }
        CellularNetwork * network = _cellularConnectionUtil.get_network();
        if (network) {
            _credentials_err = network->set_credentials(apn, uname, pwd);
            if (_credentials_err == NSAPI_ERROR_OK) {
                _credentials_set = true;
            }
        } else {
            tr_error("NO Network...");
        }
    }
}

void EasyCellularConnection::set_sim_pin(const char *sim_pin)
{
    if (sim_pin) {
        _cellularConnectionUtil.set_sim_pin(sim_pin);
    }
}

nsapi_error_t EasyCellularConnection::connect(const char *sim_pin, const char *apn, const char *uname, const char *pwd)
{
    if (_is_connected) {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    set_credentials(apn, uname, pwd);
    if (_credentials_err) {
        return _credentials_err;
    }

    if (sim_pin) {
        _cellularConnectionUtil.set_sim_pin(sim_pin);
    }

    return connect();
}

nsapi_error_t EasyCellularConnection::check_connect()
{
    if (_is_connected) {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    // there was an error while setting credentials but it's a void function so check error here...
    if (_credentials_err) {
        return _credentials_err;
    }

    nsapi_error_t err = init();
    if (err) {
        return err;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t EasyCellularConnection::connect()
{
    nsapi_error_t err = check_connect();
    if (err) {
        return err;
    }
#if MBED_CONF_APP_CELLULAR_USE_APN_LOOKUP
    if (!_credentials_set) {
        _target_state = CellularConnectionUtil::STATE_SIM_PIN;
        err = _cellularConnectionUtil.continue_to_state(_target_state);
        if (err == NSAPI_ERROR_OK) {
            int sim_wait = _cellularSemaphore.wait(6*1000); // reserve 6 seconds to access to SIM
            if (sim_wait != 1) {
                tr_error("NO SIM ACCESS");
                err = NSAPI_ERROR_NO_CONNECTION;
            } else {
                char imsi[MAX_IMSI_LENGTH+1];
                err = _cellularConnectionUtil.get_sim()->get_imsi(imsi);
                if (err == NSAPI_ERROR_OK) {
                    const char *apn_config = apnconfig(imsi);
                    if (apn_config) {
                        const char* apn = _APN_GET(apn_config);
                        const char* uname = _APN_GET(apn_config);
                        const char* pwd = _APN_GET(apn_config);
                        tr_info("Looked up APN %s", apn);
                        err = _cellularConnectionUtil.get_network()->set_credentials(apn, uname, pwd);
                    }
                }
            }
        }
        if (err) {
            return err;
        }
    }
#endif // MBED_CONF_APP_CELLULAR_USE_APN_LOOKUP

    _target_state = CellularConnectionUtil::STATE_CONNECTED;
    err = _cellularConnectionUtil.continue_to_state(_target_state);
    if (err == NSAPI_ERROR_OK) {
        int ret_wait = _cellularSemaphore.wait(10*60*1000); // cellular network searching may take several minutes
        if (ret_wait != 1) {
            tr_info("No cellular connection");
            err = NSAPI_ERROR_NO_CONNECTION;
        }
    }

    return err;
}

nsapi_error_t EasyCellularConnection::disconnect()
{
    _credentials_err = NSAPI_ERROR_OK;
    _is_connected = false;
    _credentials_set = false;
    if (!_cellularConnectionUtil.get_network()) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    return _cellularConnectionUtil.get_network()->disconnect();
}

bool EasyCellularConnection::is_connected()
{
    return _is_connected;
}

const char *EasyCellularConnection::get_ip_address()
{
    CellularNetwork *network = _cellularConnectionUtil.get_network();
    if (!network) {
        return NULL;
    }
    return _cellularConnectionUtil.get_network()->get_ip_address();
}

const char *EasyCellularConnection::get_netmask()
{
    CellularNetwork *network = _cellularConnectionUtil.get_network();
    if (!network) {
        return NULL;
    }

    return network->get_netmask();
}

const char *EasyCellularConnection::get_gateway()
{
    CellularNetwork *network = _cellularConnectionUtil.get_network();
    if (!network) {
        return NULL;
    }

    return network->get_gateway();
}

void EasyCellularConnection::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    CellularNetwork *network = _cellularConnectionUtil.get_network();
    if (network) {
        network->attach(status_cb);
    }
}

NetworkStack *EasyCellularConnection::get_stack()
{
    return _cellularConnectionUtil.get_stack();
}

} // namespace

#endif // CELLULAR_DEVICE
