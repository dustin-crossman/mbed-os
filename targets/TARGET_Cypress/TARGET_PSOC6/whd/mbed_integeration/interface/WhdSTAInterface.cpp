/* WHD STAION implementation of NetworkInterfaceAPI
 * Copyright (c) 2017 ARM Limited
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

#include <cstring>
#include "WhdSTAInterface.h"
#include "nsapi.h"
#include "lwipopts.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "rtos.h"
#include "whd_emac.h"
#include "whd_wifi_api.h"

extern "C" void whd_emac_wifi_link_state_changed(bool state_up, whd_interface_t ifp);

#define SCAN_RESULT_BUFF_SIZE     (40)
#define CMP_MAC( a, b )  (((((unsigned char*)a)[0])==(((unsigned char*)b)[0]))&& \
                          ((((unsigned char*)a)[1])==(((unsigned char*)b)[1]))&& \
                          ((((unsigned char*)a)[2])==(((unsigned char*)b)[2]))&& \
                          ((((unsigned char*)a)[3])==(((unsigned char*)b)[3]))&& \
                          ((((unsigned char*)a)[4])==(((unsigned char*)b)[4]))&& \
                          ((((unsigned char*)a)[5])==(((unsigned char*)b)[5])))

//static whd_scan_result_t     result_buff[SCAN_RESULT_BUFF_SIZE];
//static uint16_t                result_buff_write_pos = 0;
//static uint16_t                result_buff_read_pos  = 0;

static int whd_toerror(whd_result_t res) {
    switch (res) {
        case WHD_NETWORK_NOT_FOUND:             return NSAPI_ERROR_NO_SSID;
        case WHD_NOT_AUTHENTICATED:
        case WHD_EAPOL_KEY_PACKET_M3_TIMEOUT:   return NSAPI_ERROR_AUTH_FAILURE;
        default:                                return -res;
    }
}

static nsapi_security_t whd_tosecurity(whd_security_t sec) {
    switch (sec & (WEP_ENABLED | WPA_SECURITY | WPA2_SECURITY)) {
        case WEP_ENABLED:   return NSAPI_SECURITY_WEP;
        case WPA_SECURITY:  return NSAPI_SECURITY_WPA;
        case WPA2_SECURITY: return NSAPI_SECURITY_WPA2;
        default:
            if (sec == WHD_SECURITY_OPEN) return NSAPI_SECURITY_NONE;
            else                            return NSAPI_SECURITY_UNKNOWN;
    }
}

static whd_security_t whd_fromsecurity(nsapi_security_t sec) {
    switch (sec) {
        case NSAPI_SECURITY_NONE:       return WHD_SECURITY_OPEN;
        case NSAPI_SECURITY_WEP:        return WHD_SECURITY_WEP_PSK;
        case NSAPI_SECURITY_WPA:        return WHD_SECURITY_WPA_MIXED_PSK;
        case NSAPI_SECURITY_WPA2:       return WHD_SECURITY_WPA2_AES_PSK;
        case NSAPI_SECURITY_WPA_WPA2:   return WHD_SECURITY_WPA2_MIXED_PSK;
        default:                        return WHD_SECURITY_UNKNOWN;
    }
}

WhdSTAInterface::WhdSTAInterface(WHD_EMAC &emac, OnboardNetworkStack &stack)
    : EMACInterface(emac, stack),
      _ssid("\0"),
      _pass("\0"),
      _security(NSAPI_SECURITY_NONE),
      _whd_emac(emac)
{
}

nsapi_error_t WhdSTAInterface::connect(
        const char *ssid, const char *pass,
        nsapi_security_t security,
        uint8_t channel)
{
    int err = set_channel(channel);
    if (err) {
        return err;
    }

    err = set_credentials(ssid, pass, security);
    if (err) {
        return err;
    }

    return connect();
}

nsapi_error_t WhdSTAInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    if ((ssid == NULL) ||
        (strlen(ssid) == 0) ||
        (pass == NULL && security != NSAPI_SECURITY_NONE) ||
        (strlen(pass) == 0 && security != NSAPI_SECURITY_NONE) ||
        (strlen(pass) > 63 && (security == NSAPI_SECURITY_WPA2 || security == NSAPI_SECURITY_WPA || security == NSAPI_SECURITY_WPA_WPA2))
		)
    {
        return NSAPI_ERROR_PARAMETER;
    }

    memset(_ssid, 0, sizeof(_ssid));
    strncpy(_ssid, ssid, sizeof(_ssid));

    memset(_pass, 0, sizeof(_pass));
    strncpy(_pass, pass, sizeof(_pass));

    _security = security;

    return NSAPI_ERROR_OK;
}

nsapi_error_t WhdSTAInterface::connect()
{

#define MAX_RETRY_COUNT    ( 5 )
    int i;

    // initialize wiced, this is noop if already init
        if (!_whd_emac.powered_up)
            _whd_emac.power_up();

    if (!_interface) {
        nsapi_error_t err = _stack.add_ethernet_interface(_emac, true, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
    }

    if ((_ssid == NULL) ||
        (strlen(_ssid) == 0)) {
        return NSAPI_ERROR_PARAMETER;
    }

    // setup ssid
    whd_ssid_t ssid;
    strncpy((char*)ssid.value, _ssid, SSID_NAME_SIZE);
    ssid.value[SSID_NAME_SIZE-1] = '\0';
    ssid.length = strlen((char*)ssid.value);

    // choose network security
    whd_security_t security = whd_fromsecurity(_security);
#if 0
    if (security == WHD_SECURITY_OPEN) {
        // none actually indicates we need to find out the security ourselves
        // if ssid isn't being broadcasted we just continue with security none
        whd_scan_security_userdata data;
        data.ssid = _ssid;
        data.ssidlen = strlen(_ssid);
        data.security = WHD_SECURITY_OPEN;

        res = whd_wifi_scan_networks(whd_scan_security_handler, &data);
        if (res != WHD_SUCCESS) {
            return whd_toerror(res);
        }

        int tok = data.sema.wait();
        if (tok < 1) {
            return NSAPI_ERROR_WOULD_BLOCK;
        }

        security = data.security;
    }
#endif
    // join the network
    whd_result_t res;
    for ( i = 0; i < MAX_RETRY_COUNT; i++ )
    {
       res = (whd_result_t)whd_wifi_join(_whd_emac.ifp,
            &ssid,
            security,
            (const uint8_t *)_pass, strlen(_pass));
       if (res == WHD_SUCCESS) {
           break;
       }
    }

    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    /* Use DHCP to get IP address? */
    set_dhcp( (_ip_address[0] ? false : true) );

    if(whd_wifi_is_ready_to_transceive(_whd_emac.ifp) == WHD_SUCCESS)
    {
    		whd_emac_wifi_link_state_changed(true, _whd_emac.ifp);
    }

    // bring up
    return _interface->bringup(_dhcp,
            _ip_address[0] ? _ip_address : 0,
            _netmask[0] ? _netmask : 0,
            _gateway[0] ? _gateway : 0,
            DEFAULT_STACK);
}


nsapi_error_t WhdSTAInterface::disconnect()
{
    if (!_interface) {
        return NSAPI_STATUS_DISCONNECTED;
    }

    // bring down
    int err = EMACInterface::disconnect();
    if (err) {
        return err;
    }

    // leave network
    whd_result_t res = whd_wifi_leave(_whd_emac.ifp);
    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    return NSAPI_ERROR_OK;
}

int8_t WhdSTAInterface::get_rssi()
{
    int32_t rssi;
    whd_result_t res;

    res = (whd_result_t)whd_wifi_get_rssi(_whd_emac.ifp, &rssi);
    if (res != 0) {
        return 0;
    }

    return (int8_t)rssi;
}

struct whd_scan_userdata {
    whd_semaphore_type_t sema;
    WiFiAccessPoint *aps;
    whd_scan_result_t result_buff[SCAN_RESULT_BUFF_SIZE];
    unsigned count;
    unsigned offset;
    whd_interface_t ifp;
};

struct whd_scan_security_userdata {
    Semaphore sema;
    const char *ssid;
    int ssidlen;
    whd_security_t security;
};

#if 0
static whd_result_t whd_scan_security_handler(
        whd_scan_handler_result_t *result)
{
    whd_scan_security_userdata *data =
            (whd_scan_security_userdata*)result->user_data;
    malloc_transfer_to_curr_thread(result);

    // finished scan, either succesfully or through an abort
    if (result->status != WHD_SCAN_INCOMPLETE) {
        data->sema.release();
        free(result);
        return WHD_SUCCESS;
    }

    if (data->ssidlen == result->ap_details.SSID.length &&
        memcmp(data->ssid, result->ap_details.SSID.value, data->ssidlen) == 0) {
        // found a match
        data->security = result->ap_details.security;
        whd_wifi_abort_scan();
    }

    // release result
    free(result);
    return WHD_SUCCESS;
}
#endif

// static whd_result_t whd_scan_count_handler(whd_scan_result_t** result_ptr, 
//     void* user_data, whd_scan_status_t status)
// {
//     whd_scan_userdata *data = (whd_scan_userdata*)user_data;

//     // finished scan, either succesfully or through an abort
//     if (status != WHD_SCAN_INCOMPLETE) {
//         data->sema.release();
//         return WHD_SUCCESS;
//     }

//     whd_scan_result_t* record = ( *result_ptr );


//     // just count the available networks
//     data->offset += 1;

//     return WHD_SUCCESS;
// }

static void whd_scan_handler(whd_scan_result_t** result_ptr, 
    void* user_data, whd_scan_status_t status)
{

    whd_scan_userdata *data = (whd_scan_userdata*)user_data;

    // finished scan, either succesfully or through an abort
    if (status != WHD_SCAN_INCOMPLETE) {
        whd_rtos_set_semaphore(&data->sema, WHD_FALSE);
        return;
    }

    // can't really keep anymore scan results
    if (data->count > 0 && data->offset == data->count) {
        whd_wifi_stop_scan(data->ifp);
        return;
    }

    whd_scan_result_t* record = ( *result_ptr );

    for (int i=0; i<data->offset; i++) {
        if (CMP_MAC( data->result_buff[i].BSSID.octet, record->BSSID.octet ))
            return;
    }
    
    if (data->count > 0) {
        // get ap stats
        nsapi_wifi_ap ap;

        uint8_t length = record->SSID.length;
        if (length < sizeof(ap.ssid)-1) {
            length = sizeof(ap.ssid)-1;
        }
        memcpy(ap.ssid, record->SSID.value, length);
        ap.ssid[length] = '\0';

        memcpy(ap.bssid, record->BSSID.octet, sizeof(ap.bssid));

        ap.security = whd_tosecurity(record->security);
        ap.rssi = record->signal_strength;
        ap.channel = record->channel;
        data->aps[data->offset] = WiFiAccessPoint(ap);
    }

    // store as ap object
    data->result_buff[data->offset] = *record;
    data->offset += 1;

}


int WhdSTAInterface::scan(WiFiAccessPoint *aps, unsigned count)
{
    // initialize wiced, this is noop if already init
    if (!_whd_emac.powered_up)
        _whd_emac.power_up();

    whd_scan_userdata data;
    data.aps = aps;
    data.count = count;
    data.offset = 0;
    data.ifp = _whd_emac.ifp;
    whd_rtos_init_semaphore(&data.sema);
    whd_scan_result_t scan_result;
    whd_scan_result_t *result_ptr = &scan_result;
    whd_result_t res;

    res = (whd_result_t)whd_wifi_scan(_whd_emac.ifp, WHD_SCAN_TYPE_ACTIVE, WHD_BSS_TYPE_ANY,
        NULL, NULL, NULL, NULL, whd_scan_handler, (whd_scan_result_t **) &result_ptr, &data );
    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    res = whd_rtos_get_semaphore(&data.sema, NEVER_TIMEOUT, WHD_FALSE);
    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    return data.offset;

}

