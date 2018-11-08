/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

#include "CordioPalGap.h"
#include "hci_api.h"
#include "dm_api.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

bool Gap::is_feature_supported(
    Gap::ControllerSupportedFeatures_t feature
) {
    return (HciGetLeSupFeat() & (1 << feature.value()));
}

ble_error_t Gap::initialize() {
    return BLE_ERROR_NONE;
}

ble_error_t Gap::terminate() {
    return BLE_ERROR_NONE;
}

address_t Gap::get_device_address() {
    return address_t(HciGetBdAddr());
}

address_t Gap::get_random_address() {
    return device_random_address;
}

ble_error_t Gap::set_random_address(const address_t& address) {
    device_random_address = address;
    DmDevSetRandAddr(const_cast<uint8_t*>(address.data()));
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_advertising_parameters(
    uint16_t advertising_interval_min,
    uint16_t advertising_interval_max,
    advertising_type_t advertising_type,
    own_address_type_t own_address_type,
    advertising_peer_address_type_t peer_address_type,
    const address_t& peer_address,
    advertising_channel_map_t advertising_channel_map,
    advertising_filter_policy_t advertising_filter_policy
) {
    DmAdvSetInterval(
        DM_ADV_HANDLE_DEFAULT,
        advertising_interval_min,
        advertising_interval_max
    );

    DmAdvSetAddrType(own_address_type.value());

    DmAdvSetChannelMap(
        DM_ADV_HANDLE_DEFAULT,
        advertising_channel_map.value()
    );

    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_ADV,
        advertising_filter_policy.value()
    );

    DmAdvConfig(
        DM_ADV_HANDLE_DEFAULT,
        advertising_type.value(),
        peer_address_type.value(),
        const_cast<uint8_t*>(peer_address.data())
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_advertising_data(
    uint8_t advertising_data_length,
    const advertising_data_t& advertising_data
) {
    DmAdvSetData(
        DM_ADV_HANDLE_DEFAULT,
        HCI_ADV_DATA_OP_COMP_FRAG,
        DM_DATA_LOC_ADV,
        advertising_data_length,
        const_cast<uint8_t*>(advertising_data.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_scan_response_data(
    uint8_t scan_response_data_length,
    const advertising_data_t& scan_response_data
) {
    DmAdvSetData(
        DM_ADV_HANDLE_DEFAULT,
        HCI_ADV_DATA_OP_COMP_FRAG,
        DM_DATA_LOC_SCAN,
        scan_response_data_length,
        const_cast<uint8_t*>(scan_response_data.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::advertising_enable(bool enable) {
    if (enable) {
        uint8_t adv_handles[] = { DM_ADV_HANDLE_DEFAULT };
        uint16_t adv_durations[] = { /* infinite */ 0 };
        uint8_t max_ea_events[] = { 0 };
        DmAdvStart(1, adv_handles, adv_durations, max_ea_events);
    } else {
        uint8_t adv_handles[] = { DM_ADV_HANDLE_DEFAULT };
        DmAdvStop(1, adv_handles);
    }
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_scan_parameters(
    bool active_scanning,
    uint16_t scan_interval,
    uint16_t scan_window,
    own_address_type_t own_address_type,
    scanning_filter_policy_t filter_policy
) {
    use_active_scanning = active_scanning;
    DmScanSetInterval(HCI_INIT_PHY_LE_1M_BIT, &scan_interval, &scan_window);
    DmScanSetAddrType(own_address_type.value());
    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_SCAN,
        filter_policy.value()
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::scan_enable(
    bool enable,
    bool filter_duplicates
) {
    if (enable) {
        uint8_t scanType = use_active_scanning ? DM_SCAN_TYPE_ACTIVE : DM_SCAN_TYPE_PASSIVE;
        DmScanStart(
            HCI_SCAN_PHY_LE_1M_BIT,
            DM_DISC_MODE_NONE,
            &scanType,
            filter_duplicates,
            0,
            0
        );
    } else {
        DmScanStop();
    }
    return BLE_ERROR_NONE;
}

ble_error_t Gap::create_connection(
    uint16_t scan_interval,
    uint16_t scan_window,
    initiator_policy_t initiator_policy,
    connection_peer_address_type_t peer_address_type,
    const address_t& peer_address,
    own_address_type_t own_address_type,
    uint16_t connection_interval_min,
    uint16_t connection_interval_max,
    uint16_t connection_latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
) {
    DmConnSetScanInterval(scan_interval, scan_window);
    DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, initiator_policy.value());
    DmConnSetAddrType(own_address_type.value());

    hciConnSpec_t conn_spec = {
        connection_interval_min,
        connection_interval_max,
        connection_latency,
        supervision_timeout,
        minimum_connection_event_length,
        maximum_connection_event_length
    };
    DmConnSetConnSpec(&conn_spec);

    dmConnId_t connection_id = DmConnOpen(
        DM_CLIENT_ID_APP,
        HCI_INIT_PHY_LE_1M_BIT,
        peer_address_type.value(),
        const_cast<uint8_t*>(peer_address.data())
    );

    if (connection_id == DM_CONN_ID_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    return BLE_ERROR_NONE;
}

ble_error_t Gap::cancel_connection_creation() {
    DmConnClose(
        DM_CLIENT_ID_APP,
        /* connection handle - invalid */ DM_CONN_ID_NONE,
        /* reason - invalid (use success) */ 0x00
    );
    return BLE_ERROR_NONE;
}

uint8_t Gap::read_white_list_capacity() {
    return HciGetWhiteListSize();
}

ble_error_t Gap::clear_whitelist() {
    DmDevWhiteListClear();
    return BLE_ERROR_NONE;
}

ble_error_t Gap::add_device_to_whitelist(
    whitelist_address_type_t address_type,
    address_t address
) {
    DmDevWhiteListAdd(
        address_type.value(),
        const_cast<uint8_t*>(address.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::remove_device_from_whitelist(
    whitelist_address_type_t address_type,
    address_t address
) {
    DmDevWhiteListRemove(
        address_type.value(),
        const_cast<uint8_t*>(address.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::connection_parameters_update(
    connection_handle_t connection,
    uint16_t connection_interval_min,
    uint16_t connection_interval_max,
    uint16_t connection_latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
) {
    if (DmConnCheckIdle(connection) != 0) {
        return BLE_ERROR_INVALID_STATE;
    }

    hciConnSpec_t connection_spec = {
        connection_interval_min,
        connection_interval_max,
        connection_latency,
        supervision_timeout,
        minimum_connection_event_length,
        maximum_connection_event_length
    };
    DmConnUpdate(
        connection,
        &connection_spec
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::accept_connection_parameter_request(
    connection_handle_t connection_handle,
    uint16_t interval_min,
    uint16_t interval_max,
    uint16_t latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
) {
    hciConnSpec_t connection_spec = {
        interval_min,
        interval_max,
        latency,
        supervision_timeout,
        minimum_connection_event_length,
        maximum_connection_event_length
    };
    DmRemoteConnParamReqReply(connection_handle, &connection_spec);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::reject_connection_parameter_request(
    connection_handle_t connection_handle,
    hci_error_code_t rejection_reason
) {
    DmRemoteConnParamReqNegReply(
        connection_handle,
        rejection_reason.value()
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::disconnect(
    connection_handle_t connection,
    disconnection_reason_t disconnection_reason
) {
    DmConnClose(
        DM_CLIENT_ID_APP,
        connection,
        disconnection_reason.value()
    );
    return BLE_ERROR_NONE;
}

bool Gap::is_privacy_supported() {
    // We only support controller-based privacy, so return whether the controller supports it
    return HciLlPrivacySupported();
}

ble_error_t Gap::set_address_resolution(
    bool enable
) {
    DmPrivSetAddrResEnable(enable);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::read_phy(connection_handle_t connection) {
    if (is_feature_supported(ControllerSupportedFeatures_t::LE_2M_PHY)
        || is_feature_supported(ControllerSupportedFeatures_t::LE_CODED_PHY)) {
        DmReadPhy(connection);
        return BLE_ERROR_NONE;
    }
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::set_preferred_phys(
    const phy_set_t& tx_phys,
    const phy_set_t& rx_phys
) {
    DmSetDefaultPhy(
        create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value()
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_phy(
    connection_handle_t connection,
    const phy_set_t& tx_phys,
    const phy_set_t& rx_phys,
    coded_symbol_per_bit_t coded_symbol
) {
    /* if phy set is empty set corresponding all_phys bit to 1 */
    uint8_t all_phys = 0;
    if (tx_phys.value() == 0) {
        all_phys |= 0x01;
    }
    if (rx_phys.value() == 0) {
        all_phys |= 0x02;
    }

    DmSetPhy(
        connection,
        create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value(),
        coded_symbol.value()
    );

    return BLE_ERROR_NONE;
}

// singleton of the ARM Cordio client
Gap& Gap::get_gap() {
    static Gap _gap;
    return _gap;
}

/**
 * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
 */
void Gap::gap_handler(const wsfMsgHdr_t* msg) {
    typedef bool (*event_handler_t)(const wsfMsgHdr_t* msg);

    if (msg == NULL) {
        return;
    }

    connection_handle_t handle = (connection_handle_t)msg->param;
    EventHandler *handler = get_gap()._pal_event_handler;


    switch(msg->event) {
        case DM_PHY_READ_IND: {
            if (!handler) {
                break;
            }
            const hciLeReadPhyCmdCmplEvt_t* evt = (const hciLeReadPhyCmdCmplEvt_t*)msg;

            handler->on_read_phy(
                (hci_error_code_t::type)msg->status,
                handle,
                (ble::phy_t::type)evt->txPhy,
                (ble::phy_t::type)evt->rxPhy
            );
        } break;

        case DM_PHY_UPDATE_IND: {
            if (!handler) {
                break;
            }

            const hciLePhyUpdateEvt_t* evt = (const hciLePhyUpdateEvt_t*)msg;

            handler->on_phy_update_complete(
                (hci_error_code_t::type)msg->status,
                handle,
                (ble::phy_t::type)evt->txPhy,
                (ble::phy_t::type)evt->rxPhy
            );
        } break;

        case DM_PER_ADV_SYNC_EST_IND: {
            if (!handler) {
                break;
            }

            const hciLePerAdvSyncEstEvt_t* evt = (const hciLePerAdvSyncEstEvt_t*) msg;

            handler->on_periodic_advertising_sync_established(
                hci_error_code_t(evt->status),
                evt->syncHandle,
                evt->advSid,
                connection_peer_address_type_t(evt->advAddrType),
                evt->advAddr,
                evt->perAdvInterval,
                clock_accuracy_t(evt->clockAccuracy)
            );
        } break;

        case DM_PER_ADV_REPORT_IND: {
            if (!handler) {
                break;
            }

            const hciLePerAdvReportEvt_t* evt = (const hciLePerAdvReportEvt_t*) msg;

            handler->on_periodic_advertising_report(
                evt->syncHandle,
                evt->txPower,
                evt->rssi,
                advertising_data_status_t(evt->status),
                evt->len,
                evt->pData
            );
        } break;

        case DM_PER_ADV_SYNC_LOST_IND: {
            if (!handler) {
                break;
            }

            const hciLePerAdvSyncLostEvt_t* evt = (const hciLePerAdvSyncLostEvt_t*) msg;
            handler->on_periodic_advertising_sync_loss(evt->syncHandle);
        }  break;

        case DM_CONN_OPEN_IND: {
            if (!handler) {
                break;
            }

            // TODO: filter with old event ...
            const hciLeConnCmplEvt_t* evt = (const hciLeConnCmplEvt_t*) msg;
            handler->on_enhanced_connection_complete(
                hci_error_code_t(evt->status),
                evt->handle,
                connection_role_t(evt->role),
                connection_peer_address_type_t(evt->addrType),
                evt->peerAddr,
                evt->localRpa,
                evt->peerRpa,
                evt->connInterval,
                evt->connLatency,
                evt->supTimeout,
                clock_accuracy_t(evt->clockAccuracy)
            );
        } break;

        case DM_SCAN_REQ_RCVD_IND: {
            if (!handler) {
                break;
            }

            const hciLeScanReqRcvdEvt_t* evt = (const hciLeScanReqRcvdEvt_t*) msg;
            handler->on_scan_request_received(
                evt->advHandle,
                connection_peer_address_type_t(evt->scanAddrType),
                evt->scanAddr
            );
        } break;

        case DM_ADV_SET_STOP_IND: {
            if (!handler) {
                break;
            }

            const hciLeAdvSetTermEvt_t* evt = (const hciLeAdvSetTermEvt_t*) msg;
            handler->on_advertising_set_terminated(
                hci_error_code_t(evt->status),
                evt->advHandle,
                evt->handle,
                evt->numComplEvts
            );
        } break;

        case DM_EXT_SCAN_STOP_IND: {
            if (!handler) {
                break;
            }

            const hciLeScanTimeoutEvt_t* evt = (const hciLeScanTimeoutEvt_t*) msg;
            handler->on_scan_timeout();
        } break;

        case DM_EXT_SCAN_REPORT_IND: {
            if (!handler) {
                break;
            }

            const hciLeExtAdvReportEvt_t* evt = (const hciLeExtAdvReportEvt_t*) msg;
            connection_peer_address_type_t addr_type(evt->addrType);
            phy_t sec_phy(evt->secPhy);

            handler->on_extended_advertising_report(
                advertising_event_t(evt->eventType),
                (evt->addrType == HCI_ADDR_TYPE_ANONYMOUS) ?  NULL : &addr_type,
                evt->addr,
                phy_t(evt->priPhy),
                evt->secPhy == HCI_ADV_RPT_PHY_SEC_NONE ? NULL : &sec_phy,
                evt->advSid,
                evt->txPower,
                evt->rssi,
                evt->perAdvInter,
                direct_address_type_t(evt->directAddrType),
                evt->directAddr,
                evt->len,
                evt->pData
            );
        } break;
    }

    // all handlers are stored in a static array
    static const event_handler_t handlers[] = {
        &event_handler<ConnectionCompleteMessageConverter>,
        &event_handler<GapAdvertisingReportMessageConverter>,
        &event_handler<DisconnectionMessageConverter>,
        &event_handler<ConnectionUpdateMessageConverter>,
        &event_handler<RemoteConnectionParameterRequestMessageConverter>
    };

    // event->hdr.param: connection handle

    // traverse all handlers and execute them with the event in input.
    // exit if an handler has handled the event.
    for(size_t i = 0; i < (sizeof(handlers)/sizeof(handlers[0])); ++i) {
        if (handlers[i](msg)) {
            return;
        }
    }
}

/**
 * T shall define a can_convert and convert function and a type
 */
template<typename T>
bool Gap::event_handler(const wsfMsgHdr_t* msg) {
    if (T::can_convert(msg)) {
        get_gap().emit_gap_event(T::convert((const typename T::type*)msg));
        return true;
    }
    return false;
}

ble_error_t Gap::set_advertising_set_random_address(
    advertising_handle_t advertising_handle,
    const address_t &address
)
{
    DmAdvSetRandAddr(advertising_handle, address.data());
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_extended_advertising_parameters(
    advertising_handle_t advertising_handle,
    advertising_event_properties_t event_properties,
    advertising_interval_t primary_advertising_interval_min,
    advertising_interval_t primary_advertising_interval_max,
    advertising_channel_map_t primary_advertising_channel_map,
    own_address_type_t own_address_type,
    advertising_peer_address_type_t peer_address_type,
    const address_t &peer_address,
    advertising_filter_policy_t advertising_filter_policy,
    advertising_power_t advertising_power,
    phy_t primary_advertising_phy,
    uint8_t secondary_advertising_max_skip,
    phy_t secondary_phy,
    uint8_t advertising_sid,
    bool scan_request_notification
)
{
    DmAdvSetInterval(
        advertising_handle,
        primary_advertising_interval_min,
        primary_advertising_interval_max
    );

    DmAdvSetAddrType(own_address_type.value());

    DmAdvSetChannelMap(
        advertising_handle,
        primary_advertising_channel_map.value()
    );

    DmDevSetExtFilterPolicy(
        advertising_handle,
        DM_FILT_POLICY_MODE_ADV,
        advertising_filter_policy.value()
    );

    DmAdvScanReqNotifEnable(advertising_handle, scan_request_notification);

    DmAdvSetPhyParam(
        advertising_handle,
        primary_advertising_phy.value(),
        secondary_advertising_max_skip,
        secondary_phy.value()
    );

    DmAdvIncTxPwr(
        advertising_handle,
        event_properties.include_tx_power,
        advertising_power
    );

    DmAdvUseLegacyPdu(advertising_handle, event_properties.use_legacy_pdu);
    DmAdvOmitAdvAddr(advertising_handle, event_properties.omit_advertisser_address);

    DmAdvConfig(
        advertising_handle,
        event_properties.value(), // TODO: use the raw value here ???
        peer_address_type.value(),
        const_cast<uint8_t *>(peer_address.data())
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_periodic_advertising_parameters(
    advertising_handle_t advertising_handle,
    periodic_advertising_interval_t periodic_advertising_min,
    periodic_advertising_interval_t periodic_advertising_max,
    bool advertise_power
)
{
    DmPerAdvIncTxPwr(advertising_handle, advertise_power);
    DmPerAdvSetInterval(
        advertising_handle,
        periodic_advertising_min,
        periodic_advertising_max
    );
    DmPerAdvConfig(advertising_handle);

    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_extended_advertising_data(
    advertising_handle_t advertising_handle,
    advertising_fragment_description_t operation,
    bool minimize_fragmentation,
    uint8_t advertising_data_size,
    const uint8_t *advertising_data
)
{
    uint8_t frag_pref = minimize_fragmentation ?
        HCI_ADV_DATA_FRAG_PREF_NO_FRAG :
        HCI_ADV_DATA_FRAG_PREF_FRAG;

    DmAdvSetFragPref(advertising_handle, frag_pref);

    DmAdvSetData(
        advertising_handle,
        operation.value(),
        DM_DATA_LOC_ADV,
        advertising_data_size,
        const_cast<uint8_t *>(advertising_data)
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_periodic_advertising_data(
    advertising_handle_t advertising_handle,
    advertising_fragment_description_t fragment_description,
    uint8_t advertising_data_size,
    const uint8_t *advertising_data
)
{
    DmPerAdvSetData(
        advertising_handle,
        fragment_description.value(),
        advertising_data_size,
        const_cast<uint8_t *>(advertising_data)
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_extended_scan_response_data(
    advertising_handle_t advertising_handle,
    advertising_fragment_description_t operation,
    bool minimize_fragmentation,
    uint8_t scan_response_data_size,
    const uint8_t *scan_response_data
)
{
    uint8_t frag_pref = minimize_fragmentation ?
        HCI_ADV_DATA_FRAG_PREF_NO_FRAG :
        HCI_ADV_DATA_FRAG_PREF_FRAG;

    DmAdvSetFragPref(advertising_handle, frag_pref);

    DmAdvSetData(
        advertising_handle,
        operation.value(),
        DM_DATA_LOC_SCAN,
        scan_response_data_size,
        const_cast<uint8_t *>(scan_response_data)
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::extended_advertising_enable(
    bool enable,
    uint8_t number_of_sets,
    const advertising_handle_t *handles,
    const uint16_t *durations,
    const uint8_t *max_extended_advertising_events
)
{
    if (enable) {
        uint16_t* durations_ms = new uint16_t[number_of_sets];
        for (size_t i = 0; i < number_of_sets; ++i) {
            uint32_t r = durations[i] * 10;
            durations_ms[i] = r > 0xFFFF ? 0xFFFF : r;
        }

        DmAdvStart(
            number_of_sets,
            const_cast<uint8_t *>(handles),
            durations_ms,
            const_cast<uint8_t *>(max_extended_advertising_events)
        );

        delete[] durations_ms;
    } else {
        DmAdvStop(
            number_of_sets,
            const_cast<uint8_t *>(handles)
        );
    }

    return BLE_ERROR_NONE;
}

ble_error_t Gap::periodic_advertising_enable(
    bool enable,
    advertising_handle_t advertising_handle
)
{
    if (enable) {
        DmPerAdvStart(advertising_handle);
    } else {
        DmPerAdvStop(advertising_handle);
    }

    return BLE_ERROR_NONE;
}

uint16_t Gap::get_maximum_advertising_data_length()
{
    return HciGetMaxAdvDataLen();
}

uint8_t Gap::get_max_number_of_advertising_sets()
{
    return HciGetNumSupAdvSets();
}

ble_error_t Gap::remove_advertising_set(advertising_handle_t advertising_handle)
{
    DmAdvRemoveAdvSet(advertising_handle);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::clear_advertising_sets()
{
    DmAdvClearAdvSets();
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_extended_scan_parameters(
    own_address_type_t own_address_type,
    scanning_filter_policy_t filter_policy,
    phy_set_t scanning_phys,
    const bool *active_scanning,
    const uint16_t *scan_interval,
    const uint16_t *scan_window
)
{
    DmScanSetAddrType(own_address_type.value());

    for (size_t i = 0, count = scanning_phys.count(); i < count; ++i) {
        extended_scan_type[i] = active_scanning[i] ?
            DM_SCAN_TYPE_ACTIVE :
            DM_SCAN_TYPE_PASSIVE;
    }

    this->scanning_phys = scanning_phys;

    DmScanSetInterval(
        scanning_phys.value(),
        const_cast<uint16_t *>(scan_interval),
        const_cast<uint16_t *>(scan_window)
    );

    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_SCAN,
        filter_policy.value()
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::extended_scan_enable(
    bool enable,
    duplicates_filter_t filter_duplicates,
    uint16_t duration,
    uint16_t period
)
{
    if (enable) {
        uint32_t duration_ms = duration * 10;

        DmScanModeExt();
        DmScanStart(
            scanning_phys.value(),
            DM_DISC_MODE_NONE,
            extended_scan_type,
            filter_duplicates.value(), // TODO: cordio API incomplete ???
            duration_ms > 0xFFFF ? 0xFFFF : duration_ms,
            period
        );
    } else {
        DmScanStop();
    }

    return BLE_ERROR_NONE;
}

ble_error_t Gap::periodic_advertising_create_sync(
    bool use_periodic_advertiser_list,
    uint8_t advertising_sid,
    peer_address_type_t peer_address_type,
    const address_t &peer_address,
    uint16_t allowed_skip,
    uint16_t sync_timeout
)
{
    if (use_periodic_advertiser_list) {
        DmDevSetExtFilterPolicy(
            DM_ADV_HANDLE_DEFAULT,
            DM_FILT_POLICY_MODE_SYNC,
            HCI_FILT_PER_ADV_LIST
        );
    }

    DmSyncStart(
        advertising_sid,
        peer_address_type.value(),
        peer_address.data(),
        allowed_skip,
        sync_timeout
    );

    return BLE_ERROR_INVALID_PARAM;
}

ble_error_t Gap::cancel_periodic_advertising_create_sync()
{
    // FIXME: Find a way to use it!
    // Function not directly exposed by the cordio stack.
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::periodic_advertising_terminate_sync(sync_handle_t sync_handle)
{
    DmSyncStop(sync_handle);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::add_device_to_periodic_advertiser_list(
    advertising_peer_address_type_t advertiser_address_type,
    const address_t &advertiser_address,
    uint8_t advertising_sid
)
{
    DmAddDeviceToPerAdvList(
        advertiser_address_type.value(),
        const_cast<uint8_t *>(advertiser_address.data()),
        advertising_sid
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::remove_device_from_periodic_advertiser_list(
    advertising_peer_address_type_t advertiser_address_type,
    const address_t &advertiser_address,
    uint8_t advertising_sid
)
{
    DmRemoveDeviceFromPerAdvList(
        advertiser_address_type.value(),
        const_cast<uint8_t *>(advertiser_address.data()),
        advertising_sid
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::clear_periodic_advertiser_list()
{
    DmClearPerAdvList();
    return BLE_ERROR_NONE;
}

uint8_t Gap::read_periodic_advertiser_list_size()
{
    return HciGetPerAdvListSize();
}

ble_error_t Gap::extended_create_connection(
    initiator_policy_t initiator_policy,
    own_address_type_t own_address_type,
    peer_address_type_t peer_address_type,
    const address_t &peer_address,
    phy_set_t initiating_phys,
    const uint16_t *scan_intervals,
    const uint16_t *scan_windows,
    const uint16_t *connection_intervals_min,
    const uint16_t *connection_intervals_max,
    const uint16_t *connection_latencies,
    const uint16_t *supervision_timeouts,
    const uint16_t *minimum_connection_event_lengths,
    const uint16_t *maximum_connection_event_lengths
)
{
    DmExtConnSetScanInterval(
        initiating_phys.value(),
        const_cast<uint16_t *>(scan_intervals),
        const_cast<uint16_t *>(scan_windows)
    );

    DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, initiator_policy.value());
    DmConnSetAddrType(own_address_type.value());

    // At most 3 phys are in used
    hciConnSpec_t conn_specs[3];
    for (size_t i = 0, count = initiating_phys.count(); i < count; ++i) {
        conn_specs[i].connIntervalMin = connection_intervals_min[i];
        conn_specs[i].connIntervalMax = connection_intervals_max[i];
        conn_specs[i].connLatency = connection_latencies[i];
        conn_specs[i].supTimeout = supervision_timeouts[i];
        conn_specs[i].minCeLen = minimum_connection_event_lengths[i];
        conn_specs[i].maxCeLen = maximum_connection_event_lengths[i];
    }

    DmExtConnSetConnSpec(initiating_phys.value(), conn_specs);

    dmConnId_t connection_id = DmConnOpen(
        DM_CLIENT_ID_APP,
        initiating_phys.value(),
        peer_address_type.value(),
        const_cast<uint8_t *>(peer_address.data())
    );

    if (connection_id == DM_CONN_ID_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    return BLE_ERROR_NONE;
}

} // cordio
} // vendor
} // pal
} // ble
