/* WHD implementation of NetworkInterfaceAPI
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

#ifndef WHD_STA_INTERFACE_H
#define WHD_STA_INTERFACE_H

#include "mbed.h"
#include "EthernetInterface.h"
#include "netsocket/OnboardNetworkStack.h"
#include "whd_emac.h"
#include "whd_types_int.h"


/** WhdSTAInterface class
 *  Implementation of the NetworkStack for the WHD
 */
class WhdSTAInterface : public WiFiInterface, public EMACInterface
{
public:

    WhdSTAInterface(
            WHD_EMAC &emac = WHD_EMAC::get_instance(),
            OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance());

    
    static WhdSTAInterface *get_default_instance();

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t connect();

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   This parameter is not supported, setting it to anything else than 0 will result in NSAPI_ERROR_UNSUPPORTED
     *  @return          0 on success, or error code on failure
     */
    nsapi_error_t connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0);

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    nsapi_error_t disconnect();

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    nsapi_error_t set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);

    /** Set the WiFi network channel - NOT SUPPORTED
     *
     * This function is not supported and will return NSAPI_ERROR_UNSUPPORTED
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          Not supported, returns NSAPI_ERROR_UNSUPPORTED
     */
    nsapi_error_t set_channel(uint8_t channel) {
        if (channel != 0) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return 0;
    }

    /** Gets the current radio signal strength for active connection
     *
     * @return          Connection strength in dBm (negative value)
     */
    int8_t get_rssi();

    /** Scan for available networks
     *
     * This function will block.
     *
     * @param  ap       Pointer to allocated array to store discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned count);

    /* is interface connected, if yes return WICED_SUCCESS else WICED_NOT_CONNECTED */
    int is_interface_connected();

    /* get bssid of the AP  if success return WICED_SUCCESS else WICED_ERROR */
    int get_bssid(uint8_t *bssid);

    /* read WHD log */
    int whd_log ( char *buffer, int buffer_size );

    /* Get EDCF AC params */
    nsapi_error_t wifi_get_ac_params_sta(void * ac_param );

    /* get iovar value */
    int wifi_get_iovar_value ( const char *iovar, uint32_t *value );

    /* set iovar value */
    int wifi_set_iovar_value ( const char *iovar, uint32_t value  );

    /* set ioctl value */
    int wifi_set_ioctl_value( uint32_t ioctl, uint32_t value ) ;

    /* set wifi interface up */
    int wifi_set_up (void );

    /* set wifi interface down */
    int wifi_set_down (void );

private:

    char _ssid[33]; /* The longest possible name (defined in 802.11) +1 for the \0 */
    char _pass[64]; /* The longest allowed passphrase + 1 */
    nsapi_security_t _security;
    WHD_EMAC& _whd_emac;
};

extern int wiced_leave_ap      ( int interface );
#endif
