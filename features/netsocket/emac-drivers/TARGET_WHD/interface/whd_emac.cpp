/*
 * Copyright (c) 2018 ARM Limited
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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "whd_emac.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "mbed_shared_queues.h"
#include "whd_wlioctl.h"
#include "whd_buffer_api.h"
#include "cybsp_api_wifi.h"

WHD_EMAC::WHD_EMAC(whd_interface_role_t role)
    : interface_type(role)
{
}

WHD_EMAC::WHD_EMAC()
    : interface_type(WHD_STA_ROLE)
{
}

WHD_EMAC &WHD_EMAC::get_instance() {
    return get_instance(WHD_STA_ROLE);
}

WHD_EMAC &WHD_EMAC::get_instance(whd_interface_role_t role) {
    static WHD_EMAC emac_sta(WHD_STA_ROLE);
    static WHD_EMAC emac_ap(WHD_AP_ROLE);
    return role == WHD_AP_ROLE ? emac_ap : emac_sta;
}

uint32_t WHD_EMAC::get_mtu_size() const
{
    return WHD_PAYLOAD_MTU;
}

uint32_t WHD_EMAC::get_align_preference() const
{
    return 0;
}

void WHD_EMAC::add_multicast_group(const uint8_t *addr)
{
    memcpy(multicast_addr.octet, addr, sizeof(multicast_addr.octet));
    whd_wifi_register_multicast_address(ifp, &multicast_addr);
}

void WHD_EMAC::remove_multicast_group(const uint8_t *addr)
{
    whd_wifi_unregister_multicast_address(ifp, &multicast_addr);
}

void WHD_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

void WHD_EMAC::power_down()
{
    powered_up = false;
    whd_wifi_off(ifp);
    whd_deinit(ifp);
}

bool WHD_EMAC::power_up()
{
	drvp = *(cybsp_get_wifi_driver());
	whd_wifi_on(drvp, &ifp /* OUT */);
    powered_up = true;
    if (link_state && emac_link_state_cb ) {
        emac_link_state_cb(link_state);
    }
    return true;
}

bool WHD_EMAC::get_hwaddr(uint8_t *addr) const
{
    whd_mac_t mac;
    whd_result_t res = whd_wifi_get_mac_address(ifp, &mac);
    MBED_ASSERT(res == WHD_SUCCESS);
    memcpy(addr, mac.octet, sizeof(mac.octet));
    return true;
}

void WHD_EMAC::set_hwaddr(const uint8_t *addr)
{
    /* No-op at this stage */
}

uint8_t WHD_EMAC::get_hwaddr_size() const
{
    whd_mac_t mac;
    return sizeof(mac.octet);
}

void WHD_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void WHD_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void WHD_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

bool WHD_EMAC::link_out(emac_mem_buf_t *buf)
{
    uint16_t offset = 64;
    whd_buffer_t buffer;

    uint16_t size = memory_manager->get_total_len(buf);

    whd_result_t res = whd_host_buffer_get(drvp, &buffer, WHD_NETWORK_TX, size+offset, WHD_TRUE);
    if ( res != WHD_SUCCESS)
    {
    	memory_manager->free(buf);
    	return true;
    }
    MBED_ASSERT(res == WHD_SUCCESS);

    whd_buffer_add_remove_at_front(drvp, &buffer, offset);

    void *dest = whd_buffer_get_current_piece_data_pointer(drvp, buffer);
    memory_manager->copy_from_buf(dest, size, buf);

    if (activity_cb) {
        activity_cb(true);
    }
    whd_network_send_ethernet_data(ifp, buffer);
    memory_manager->free(buf);
    return true;
}

void WHD_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, "whd", size);
}

void WHD_EMAC::set_activity_cb(mbed::Callback<void(bool)> cb)
{
    activity_cb = cb;
}

extern "C"
{
void cy_network_process_ethernet_data(whd_interface_t ifp, whd_buffer_t buffer)
{
	emac_mem_buf_t *mem_buf = NULL;

    WHD_EMAC &emac = WHD_EMAC::get_instance();

    if (!emac.powered_up && !emac.emac_link_input_cb) {
        // ignore any trailing packets
        whd_buffer_release(emac.drvp, buffer, WHD_NETWORK_RX);
        return;
    }

    uint16_t size = whd_buffer_get_current_piece_size(emac.drvp, buffer);

    if (size > 0) {
           mem_buf = buffer;
            if (emac.activity_cb) {
                emac.activity_cb(false);
            }
           emac.emac_link_input_cb(mem_buf);
    }
}

void whd_emac_wifi_link_state_changed(bool state_up, whd_interface_t ifp)
{
    WHD_EMAC &emac = WHD_EMAC::get_instance(ifp->role);

    emac.link_state = state_up;
    if (emac.emac_link_state_cb ) {
        emac.emac_link_state_cb(state_up);
    }
}

} // extern "C"


