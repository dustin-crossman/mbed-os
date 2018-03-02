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

#include <stdlib.h>
#include "AT_CellularNetwork.h"
#include "nsapi_ppp.h"
#include "CellularUtil.h"
#include "CellularLog.h"

using namespace std;
using namespace mbed_cellular_util;
using namespace mbed;

AT_CellularNetwork::AT_CellularNetwork(ATHandler &atHandler) : AT_CellularBase(atHandler),
    _stack(NULL), _uname(NULL), _pwd(NULL), _ip_stack_type_requested(DEFAULT_STACK), _ip_stack_type(DEFAULT_STACK), _cid(-1),
    _op_act(operator_t::RAT_UNKNOWN), _authentication_type(CHAP), _last_reg_type(C_REG)
{

    _at.set_urc_handler("NO CARRIER", callback(this, &AT_CellularNetwork::urc_no_carrier));

    memset(_apn, 0, MAX_APN_LENGTH);

#ifdef MBED_CONF_APP_CELLULAR_APN
    strncpy(_apn, MBED_CONF_APP_CELLULAR_APN, MAX_APN_LENGTH);
    log_debug("Using APN [%s] from json", _apn);
#endif

}

AT_CellularNetwork::~AT_CellularNetwork()
{
}

void AT_CellularNetwork::urc_no_carrier()
{
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_ERROR_CONNECTION_LOST);
    }
}

nsapi_error_t AT_CellularNetwork::set_credentials(const char *apn,
        const char *username, const char *password)
{
    strncpy(_apn, apn, MAX_APN_LENGTH);
    _uname = username;
    _pwd = password;

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_credentials(const char *apn,
     AuthenticationType type, const char *username, const char *password)
{
    strncpy(_apn, apn, MAX_APN_LENGTH);
    _uname = username;
    _pwd = password;
    _authentication_type = type;

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::connect(const char *apn,
        const char *username, const char *password)
{
    strncpy(_apn, apn, MAX_APN_LENGTH);
    _uname = username;
    _pwd = password;

    return connect();
}

nsapi_error_t AT_CellularNetwork::connect()
{
    _at.lock();

    nsapi_error_t err = set_context_to_be_activated();
    if (err != NSAPI_ERROR_OK) {
        _at.unlock();
        log_error("Failed to activate network context!");
        return err;
    }

    err = open_data_channel();
    if (err != NSAPI_ERROR_OK) {
        _at.unlock();
        log_error("Failed to open data channel!");
        return err;
    }

    _at.unlock();

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::open_data_channel()
{
    //old way: _at.send("ATD*99***%d#", _cid) && _at.recv("CONNECT");
    nsapi_error_t err = NSAPI_ERROR_NO_CONNECTION;
#if NSAPI_PPP_AVAILABLE
    log_info("Open data channel in PPP mode");
    _at.cmd_start("AT+CGDATA=\"PPP\",");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("CONNECT", true);
    if (_at.get_last_error()) {
        log_warn("Failed to CONNECT");
    }
    /* Initialize PPP
     * mbed_ppp_init() is a blocking call, it will block until
     * connected, or timeout after 30 seconds*/
    err = nsapi_ppp_connect(_at.get_file_handle(), _connection_status_cb, _uname, _pwd, _ip_stack_type);
#else
    // do check for stack to validate that we have support for stack
    _stack = get_stack();
    if (!_stack) {
        return err;
    }

    bool is_context_active = false;
    _at.cmd_start("AT+CGACT?");
    _at.cmd_stop();
    _at.resp_start("+CGACT:");
    while (_at.info_resp()) {
        int context_id = _at.read_int();
        int context_activation_state = _at.read_int();
        if (context_id == _cid && context_activation_state == 1) {
            is_context_active = true;
        }
    }
    _at.resp_stop();

    if (!is_context_active) {
        log_info("Activate PDP context");
        _at.cmd_start("AT+CGACT=1,");
        _at.write_int(_cid);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    err = (_at.get_last_error() == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_CONNECTION;
#endif
    return err;
}

/**
 * User initiated disconnect
 *
 * Disconnects from PPP connection only and brings down the underlying network
 * interface
 */
nsapi_error_t AT_CellularNetwork::disconnect()
{
#if NSAPI_PPP_AVAILABLE
    return nsapi_ppp_disconnect(_at.get_file_handle());
#else
    return NSAPI_ERROR_OK;
#endif
}

void AT_CellularNetwork::connection_status_cb(Callback<void(nsapi_error_t)> cb)
{
    _connection_status_cb = cb;
}

nsapi_error_t AT_CellularNetwork::set_context_to_be_activated()
{
    // try to find or create context with suitable stack
    if (!get_context(_ip_stack_type_requested)) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    // if user has defined user name and password we need to call CGAUTH before activating or modifying context
    if (_pwd && _uname) {
        _at.cmd_start("AT+CGAUTH=");
        _at.write_int(_cid);
        _at.write_int(_authentication_type);
        _at.write_string(_uname);
        _at.write_string(_pwd);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return _at.get_last_error();
}

bool AT_CellularNetwork::set_new_context(nsapi_ip_stack_t stack, int cid)
{
    nsapi_ip_stack_t tmp_stack = stack;
    char pdp_type[8+1] = {0};

    switch (stack) {
        case IPV4_STACK:
            strncpy(pdp_type, "IP", sizeof(pdp_type));
            break;
        case IPV6_STACK:
            strncpy(pdp_type, "IPV6", sizeof(pdp_type));
            break;
        case IPV4V6_STACK:
            strncpy(pdp_type, "IPV4V6", sizeof(pdp_type));
            break;
        default:
            strncpy(pdp_type, "", sizeof(pdp_type));
            break;
    }

    //apn: "If the value is null or omitted, then the subscription value will be requested."
    bool success = false;
    _at.cmd_start("AT+CGDCONT=");
    _at.write_int(cid);
    _at.write_string(pdp_type);
    _at.write_string(_apn);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    success = (_at.get_last_error() == NSAPI_ERROR_OK);

    // Fall back to ipv4
    if (!success && tmp_stack == IPV4V6_STACK) {
        tmp_stack = IPV4_STACK;
        _at.cmd_start("AT+FCLASS=0;+CGDCONT=");
        _at.write_int(cid);
        _at.write_string("IP");
        _at.write_string(_apn);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
        success = (_at.get_last_error() == NSAPI_ERROR_OK);
    }

    if (success) {
        _ip_stack_type = tmp_stack;
        _cid = cid;
    }

  return success;
}

bool AT_CellularNetwork::get_context(nsapi_ip_stack_t requested_stack)
{
    _at.cmd_start("AT+CGDCONT?");
    _at.cmd_stop();
    _at.resp_start("+CGDCONT:");
    _cid = -1;
    int cid_max = 0; // needed when creating new context
    char apn[MAX_APN_LENGTH] = {0};
    int apn_len = 0;

    while (_at.info_resp()) {
        int cid = _at.read_int();
        if (cid > cid_max) {
            cid_max = cid;
        }
        char pdp_type_from_context[10];
        int pdp_type_len = _at.read_string(pdp_type_from_context, sizeof(pdp_type_from_context) - 1);
        if (pdp_type_len > 0) {
            apn_len = _at.read_string(apn, sizeof(apn) - 1);
            if (apn_len >= 0) {
                if (strlen(_apn) && strcmp(apn, _apn) != 0 ) {
                    continue;
                }
                nsapi_ip_stack_t pdp_stack = string_to_stack_type(pdp_type_from_context);
                if (pdp_stack != DEFAULT_STACK) {
                    if (get_modem_stack_type(pdp_stack)) {
                        if (requested_stack == IPV4_STACK) {
                            if (pdp_stack == IPV4_STACK || pdp_stack == IPV4V6_STACK) {
                                _ip_stack_type = requested_stack;
                                _cid = cid;
                                break;
                            }
                        } else if (requested_stack == IPV6_STACK) {
                            if (pdp_stack == IPV6_STACK || pdp_stack == IPV4V6_STACK) {
                                _ip_stack_type = requested_stack;
                                _cid = cid;
                                break;
                            }
                        } else { // accept any but prefer to IPv6
                            if (pdp_stack == IPV6_STACK || pdp_stack == IPV4V6_STACK) {
                                _ip_stack_type = requested_stack;
                                _cid = cid;
                                break;
                            }
                            if (_ip_stack_type == DEFAULT_STACK) {
                                _ip_stack_type = pdp_stack;
                                _cid = cid;
                            }
                        }
                    }
                }
            }
        }
    }
    _at.resp_stop();
    if (_cid == -1) { // no suitable context was found so create a new one
        if (!set_new_context(requested_stack, cid_max+1)) {
            return false;
        }
    }

    // save the apn
    if (apn_len > 0 && !strlen(_apn)) {
        strncpy(_apn, apn, MAX_APN_LENGTH);
    }

    log_debug("Context id %d", _cid);
    return true;
}

nsapi_ip_stack_t AT_CellularNetwork::string_to_stack_type(const char* pdp_type)
{
    nsapi_ip_stack_t stack = DEFAULT_STACK;
    int len = strlen(pdp_type);

    if (len == 6 && memcmp(pdp_type, "IPV4V6", len) == 0) {
        stack = IPV4V6_STACK;
    } else if (len == 4 && memcmp(pdp_type, "IPV6", len) == 0) {
        stack = IPV6_STACK;
    } else if (len == 2 && memcmp(pdp_type, "IP", len) == 0) {
        stack = IPV4_STACK;
    }
    return stack;
}

nsapi_error_t AT_CellularNetwork::set_registration_urc(bool urc_on)
{
    RegistrationType reg_types[] = {C_EREG,        C_GREG,       C_REG};
    const char *cmd_on[]  =       {"AT+CEREG=2",   "AT+CGREG=2",    "AT+CREG=2"};
    const char *cmd_off[] =       {"AT+CEREG=0",   "AT+CGREG=0",    "AT+CREG=0"};
    for (uint8_t i=0; i<sizeof(reg_types)/sizeof(reg_types[0]); i++) {
        if (has_registration(reg_types[i])) {
            _last_reg_type = reg_types[i];
            if (urc_on) {
                _at.cmd_start(cmd_on[i]);
                _at.cmd_stop();
            } else {
                _at.cmd_start(cmd_off[i]);
                _at.cmd_stop();
            }

            _at.resp_start();
            _at.resp_stop();
        }
    }
    return _at.get_last_error();
}

nsapi_error_t AT_CellularNetwork::set_registration(char *plmn)
{
    _at.lock();

    nsapi_error_t ret = set_registration_urc(false);
    if (ret) {
        log_error("Setting registration URC failed!");
        _at.clear_error(); // allow temporary failures here
    }

    if (!plmn) {
        log_debug("Automatic network registration");
        _at.cmd_start("AT+COPS=0");
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    } else {
        log_debug("Manual network registration to %s", plmn);
        _at.cmd_start("AT+COPS=4,2,");
        _at.write_string(plmn);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_registration_status(RegistrationType type, RegistrationStatus &status)
{
    int i = (int)type;
    MBED_ASSERT(i >= 0 && i < C_MAX);

    RegistrationType reg_types[] = { C_EREG,       C_GREG,       C_REG};
    const char *cmd[] =            { "AT+CEREG",   "AT+CGREG",    "AT+CREG"};
    const char *rsp[] =            { "+CEREG: ",    "+CGREG: ",     "+CREG: "};

    const int LAC_LENGTH = 5, CELL_ID_LENGTH = 9;
    char lac_string[LAC_LENGTH] = {0}, cell_id_string[CELL_ID_LENGTH] = {0};
    bool lac_read = false, cell_id_read = false;

    _cell_id = -1;
    _lac = -1;

     _at.lock();

    if (!has_registration(reg_types[i])) {
        _at.unlock();
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.cmd_start(cmd[i]);
    _at.write_string("=2", false);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start(cmd[i]);
    _at.write_string("?", false);

    _at.cmd_stop();
    _at.resp_start(rsp[i]);
    _at.read_int(); // ignore urc mode subparam
    status = (RegistrationStatus)_at.read_int();

    int len = _at.read_string(lac_string, LAC_LENGTH);
    if (memcmp(lac_string, "ffff", LAC_LENGTH-1) && len >= 0) {
        lac_read = true;
    }

    len = _at.read_string(cell_id_string, CELL_ID_LENGTH);
    if (memcmp(cell_id_string, "ffffffff", CELL_ID_LENGTH-1) && len >= 0) {
        cell_id_read = true;
    }

    _AcT = (operator_t::RadioAccessTechnology)_at.read_int();

    _at.resp_stop();

    _at.cmd_start(cmd[i]);
    _at.write_string("=0", false);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    nsapi_error_t ret = _at.get_last_error();
    _at.unlock();

    if (lac_read) {
        _lac = hex_str_to_int(lac_string, LAC_LENGTH);
        log_debug("lac %s %d", lac_string, _lac );
    }

    if (cell_id_read) {
        _cell_id = hex_str_to_int(cell_id_string, CELL_ID_LENGTH);
        log_debug("cell_id %s %d", cell_id_string, _cell_id );
    }

    return ret;
}

nsapi_error_t AT_CellularNetwork::get_cell_id(int &cell_id)
{
    RegistrationStatus tmp;

    nsapi_error_t error = get_registration_status(_last_reg_type, tmp);

    cell_id = _cell_id;

    return error;
}

bool AT_CellularNetwork::has_registration(RegistrationType reg_type)
{
    (void)reg_type;
    return true;
}

nsapi_error_t AT_CellularNetwork::set_attach(int timeout)
{
    _at.lock();

    _at.cmd_start("AT+CGATT?");
    _at.cmd_stop();
    _at.resp_start("+CGATT:");
    int attached_state = _at.read_int();
    _at.resp_stop();
    if (attached_state != 1) {
        log_debug("Network attach");
        _at.cmd_start("AT+CGATT=1");
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_attach(AttachStatus &status)
{
    _at.lock();

    _at.cmd_start("AT+CGATT?");
    _at.cmd_stop();

    _at.resp_start("+CGATT:");
    if (_at.info_resp()) {
        int attach_status = _at.read_int();
        status = (attach_status == 1) ? Attached : Detached;
    }
    _at.resp_stop();

    return _at.unlock_return_error();
}


nsapi_error_t AT_CellularNetwork::get_backoff_time(int &backoffTime)
{
    _at.lock();

    // If apn is set
    if (strlen(_apn)) {
        _at.cmd_start("AT+CABTRDP=");
        _at.write_string(_apn);
        _at.cmd_stop();
        _at.resp_start("+CABTRDP:");
        if (_at.info_resp()) {
            _at.skip_param();
            backoffTime = _at.read_int();
        }
        _at.resp_stop();
    }

    return _at.unlock_return_error();
}

NetworkStack *AT_CellularNetwork::get_stack()
{
    // use lwIP/PPP if modem does not have IP stack
#if NSAPI_PPP_AVAILABLE
    _stack = nsapi_ppp_get_stack();
#else
    _stack = NULL;
#endif
    return _stack;
}

const char *AT_CellularNetwork::get_ip_address()
{
#if NSAPI_PPP_AVAILABLE
    return nsapi_ppp_get_ip_addr(_at.get_file_handle());
#else
    if (!_stack) {
        _stack = get_stack();
    }
    if (_stack) {
        return _stack->get_ip_address();
    }
    return NULL;
#endif
}

nsapi_error_t AT_CellularNetwork::set_stack_type(nsapi_ip_stack_t stack_type)
{

    if (get_modem_stack_type(stack_type)) {
        _ip_stack_type_requested = stack_type;
        return NSAPI_ERROR_OK;
    } else {
        return NSAPI_ERROR_PARAMETER;
    }

}

nsapi_ip_stack_t AT_CellularNetwork::get_stack_type()
{
    return _ip_stack_type;
}

bool AT_CellularNetwork::get_modem_stack_type(nsapi_ip_stack_t requested_stack)
{
    if (requested_stack == _ip_stack_type) {
        return true;
    } else {
        return false;
    }
}

nsapi_error_t AT_CellularNetwork::set_access_technology_impl(operator_t::RadioAccessTechnology opsAct)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularNetwork::set_access_technology(operator_t::RadioAccessTechnology opAct)
{
    if (opAct == operator_t::RAT_UNKNOWN) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _op_act = opAct;

    return set_access_technology_impl(opAct);
}

nsapi_error_t AT_CellularNetwork::scan_plmn(operList_t &operators, int &opsCount)
{
    int idx = 0;

    _at.lock();

    _at.cmd_start("AT+COPS=?");
    _at.cmd_stop();

    _at.resp_start("+COPS:");

    int ret, error_code = -1;
    operator_t *op = NULL;

    while (_at.info_elem('(')) {

        op = operators.add_new();

        op->op_status = (operator_t::Status)_at.read_int();
        _at.read_string(op->op_long, sizeof(op->op_long));
        _at.read_string(op->op_short, sizeof(op->op_short));
        _at.read_string(op->op_num, sizeof(op->op_num));

        // Optional - try read an int
        ret = _at.read_int();
        op->op_rat = (ret == error_code) ? operator_t::RAT_UNKNOWN:(operator_t::RadioAccessTechnology)ret;

        if ((_op_act == operator_t::RAT_UNKNOWN) ||
           ((op->op_rat != operator_t::RAT_UNKNOWN) && (op->op_rat == _op_act))) {
            idx++;
        } else {
            operators.delete_last();
        }
    }

    _at.resp_stop();

    opsCount = idx;

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                               Preferred_UE_Opt preferred_opt)
{
    _at.lock();

    _at.cmd_start("AT+CCIOTOPT=");
    _at.write_int(_cid);
    _at.write_int(supported_opt);
    _at.write_int(preferred_opt);
    _at.cmd_stop();

    _at.resp_start();
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_ciot_optimization_config(Supported_UE_Opt& supported_opt,
                                                               Preferred_UE_Opt& preferred_opt)
{
    _at.lock();

    _at.cmd_start("AT+CCIOTOPT?");
    _at.cmd_stop();

    _at.resp_start("+CCIOTOPT:");
    _at.read_int();
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        supported_opt = (Supported_UE_Opt)_at.read_int();
        preferred_opt = (Preferred_UE_Opt)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_rate_control(
        CellularNetwork::RateControlExceptionReports &reports,
        CellularNetwork::RateControlUplinkTimeUnit &timeUnit, int &uplinkRate)
{

    _at.lock();

    _at.cmd_start("AT+CGAPNRC=");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("+CGAPNRC:");
    _at.read_int();
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        bool comma_found = true;
        int next_element = _at.read_int();
        if (next_element >= 0) {
            reports = (RateControlExceptionReports)next_element;
            log_debug("reports %d",reports);
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            timeUnit = (RateControlUplinkTimeUnit)next_element;
            log_debug("time %d",timeUnit);
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            uplinkRate = next_element;
            log_debug("rate %d",uplinkRate);
        }
    }
    _at.resp_stop();
    nsapi_error_t ret = _at.get_last_error();
    _at.unlock();

    return (ret == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_PARAMETER;
}

nsapi_error_t AT_CellularNetwork::get_pdpcontext_params(pdpContextList_t& params_list)
{
    const int ipv6_subnet_size = 128;
    const int max_ipv6_size = 64;
    char* ipv6_and_subnetmask = (char*)malloc(ipv6_subnet_size);
    if (!ipv6_and_subnetmask) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    char* temp = (char*)malloc(max_ipv6_size);
    if (!temp) {
        free(ipv6_and_subnetmask);
        return NSAPI_ERROR_NO_MEMORY;
    }

    _at.lock();

    _at.cmd_start("AT+CGCONTRDP=");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("+CGCONTRDP:");
    pdpcontext_params_t *params = NULL;
    while (_at.info_resp()) { // response can be zero or many +CGDCONT lines
        params = params_list.add_new();
        if (!params) {
            log_warn("Could not allocate new pdpcontext_params_t");
            params_list.delete_all();
            _at.resp_stop();
            free(temp);
            free(ipv6_and_subnetmask);
            return NSAPI_ERROR_NO_MEMORY;
        }

        params->cid = _at.read_int();
        params->bearer_id = _at.read_int();
        _at.read_string(params->apn, sizeof(params->apn));

        // rest are optional params
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';
        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->local_addr, sizeof(params->local_addr), params->local_subnet_mask, sizeof(params->local_subnet_mask));
        ipv6_and_subnetmask[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->gateway_addr, sizeof(params->gateway_addr), temp, max_ipv6_size);
        prefer_ipv6(params->gateway_addr, sizeof(params->gateway_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->dns_primary_addr, sizeof(params->dns_primary_addr), temp, max_ipv6_size);
        prefer_ipv6(params->dns_primary_addr, sizeof(params->dns_primary_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->dns_secondary_addr, sizeof(params->dns_secondary_addr), temp, max_ipv6_size);
        prefer_ipv6(params->dns_secondary_addr, sizeof(params->dns_secondary_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->p_cscf_prim_addr, sizeof(params->p_cscf_prim_addr), temp, max_ipv6_size);
        prefer_ipv6(params->p_cscf_prim_addr, sizeof(params->p_cscf_prim_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->p_cscf_sec_addr, sizeof(params->p_cscf_sec_addr), temp, max_ipv6_size);
        prefer_ipv6(params->p_cscf_sec_addr, sizeof(params->p_cscf_sec_addr), temp, max_ipv6_size);

        params->im_signalling_flag = _at.read_int();
        params->lipa_indication = _at.read_int();
        params->ipv4_mtu = _at.read_int();
        params->wlan_offload = _at.read_int();
        params->local_addr_ind = _at.read_int();
        params->non_ip_mtu = _at.read_int();
        params->serving_plmn_rate_control_value = _at.read_int();
    }
    _at.resp_stop();

    free(temp);
    free(ipv6_and_subnetmask);

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp)
{
    _at.lock();

    _at.cmd_start("AT+CESQ");
    _at.cmd_stop();

    _at.resp_start("+CESQ:");
    rxlev = _at.read_int();
    ber = _at.read_int();
    rscp = _at.read_int();
    ecno = _at.read_int();
    rsrq = _at.read_int();
    rsrp = _at.read_int();
    _at.resp_stop();
    if (rxlev < 0 || ber < 0 || rscp < 0 || ecno < 0 || rsrq < 0 || rsrp < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_signal_quality(int &rssi, int &ber)
{
    _at.lock();

    _at.cmd_start("AT+CSQ");
    _at.cmd_stop();

    _at.resp_start("+CSQ:");
    rssi = _at.read_int();
    ber = _at.read_int();
    _at.resp_stop();
    if (rssi < 0 || ber < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return _at.unlock_return_error();
}

/** Get the last 3GPP error code
 *  @return see 3GPP TS 27.007 error codes
 */
uint8_t AT_CellularNetwork::get_3gpp_error()
{
    return _at.get_3gpp_error();
}


nsapi_error_t AT_CellularNetwork::get_operator_params(int &format, operator_t &operator_params)
{
    _at.lock();

    _at.cmd_start("AT+COPS?");
    _at.cmd_stop();

    _at.resp_start("+COPS: ");
    _at.read_int(); //ignore mode
    format = _at.read_int();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {

        switch (format) {
            case 0:
                _at.read_string(operator_params.op_long, 16+9);
                break;

            case 1:
                _at.read_string(operator_params.op_short, 8+4);
                break;

            default:
                _at.read_string(operator_params.op_num, 8+4);
                break;
        }

        operator_params.op_rat = (operator_t::RadioAccessTechnology)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}
