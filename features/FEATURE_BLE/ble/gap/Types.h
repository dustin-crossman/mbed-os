/* mbed Microcontroller Library
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

#ifndef BLE_GAP_TYPES_H
#define BLE_GAP_TYPES_H

#include "ble/common/Duration.h"
#include "ble/common/Bounded.h"
#include "ble/SafeEnum.h"

namespace ble {

/* BLE units, using microseconds as the common denominator */
typedef Duration<uint32_t,      625, Range<0x20, 0xFFFFFF> > adv_interval_t;
typedef Duration<uint16_t,    10000, Range<0x00,   0xFFFF>, Value<uint16_t, 0x0000> > adv_duration_t;
typedef Duration<uint16_t,    10000, Range<0x00,   0xFFFF>, Value<uint16_t, 0x0000> > scan_duration_t;
typedef Duration<uint16_t,  1280000, Range<0x00,   0xFFFF> > scan_period_t;
typedef Duration<uint16_t,      625, Range<0x04,   0xFFFF> > scan_interval_t;
typedef Duration<uint16_t,      625, Range<0x04,   0xFFFF> > scan_window_t;
typedef Duration<uint16_t,     1250, Range<0x06,   0x0C80> > conn_interval_t;
typedef Duration<uint16_t,    10000, Range<0x0A,   0x0C80> > supervision_timeout_t;
typedef Duration<uint16_t,      625, Range<   0,   0xFFFF> > conn_event_length_t;
typedef Duration<uint16_t,    10000, Range<0x0A,   0x4000> > sync_timeout_t;
typedef Duration<uint16_t,     1250, Range<0x06,   0xFFFF> > periodic_interval_t;

typedef Bounded<uint16_t, 0, 0x01F3> slave_latency_t;

/**
 * Handle of an advertising set.
 *
 * @note Range of valid handle is comprised between 0x00 and 0xEF.
 */
typedef uint8_t advertising_handle_t;

/**
 * Handle of a sync representing a periodic advertiser.
 *
 * @note Range of valid handle is comprised between 0x0000 and 0xFFFF.
 */
typedef uint16_t periodic_sync_handle_t;

/**
 * Encapsulates the peripheral advertising modes.
 *
 * It determine how the device appears to other scanner and peripheral
 * devices in the scanning range.
 */
struct advertising_type_t : SafeEnum<advertising_type_t, uint8_t> {
    enum type {
        /**
         * Device is connectable, scannable and doesn't expect connection from a
         * specific peer.
         *
         * @see Vol 3, Part C, Section 9.3.4 and Vol 6, Part B, Section 2.3.1.1.
         */
        ADV_CONNECTABLE_UNDIRECTED = 0x00,
        ADV_IND = 0x00,

        /**
         * Device is connectable and expects connection from a specific peer.
         * (3.75 ms or smaller Advertising Interval)
         * @see Vol 3, Part C, Section 9.3.3 and Vol 6, Part B, Section 2.3.1.2.
         */
        ADV_CONNECTABLE_DIRECTED = 0x01,
        ADV_DIRECT_IND = 0x01,

        /**
         * Device is scannable but not connectable.
         *
         * @see Vol 6, Part B, Section 2.3.1.4.
         */
        ADV_SCANNABLE_UNDIRECTED = 0x02,
        ADV_SCAN_IND = 0x02,

        /**
         * Device is not connectable and not scannable.
         *
         * @see Vol 3, Part C, Section 9.3.2 and Vol 6, Part B, Section 2.3.1.3.
         */
        ADV_NON_CONNECTABLE_UNDIRECTED = 0x03,
        ADV_NONCONN_IND = 0x03,

        /**
         * Device is connectable and expects connection from a specific peer (sent at long user set intervals).
         */
        ADV_CONNECTABLE_DIRECTED_LOW_DUTY = 0x04,
        ADV_DIRECT_IND_LOW_DUTY_CYCLE = 0x04
    };

    /**
     * Construct a new advertising_type_t value.
     */
    advertising_type_t(type value) : SafeEnum(value)
    {
    }
};


/** Used to indicate if the packet is complete and if it's truncated.
 */
struct advertising_data_status_t : SafeEnum<advertising_data_status_t, uint8_t> {
    enum type {
        COMPLETE = 0x00, /**< Advertising payload complete. */
        INCOMPLETE_MORE_DATA = 0x01, /**< Partial advertising payload, more to come. */
        INCOMPLETE_DATA_TRUNCATED = 0x02 /**< Advertising payload incomplete and no more is comoing. */
    };

    /**
     * Construct a new advertising_data_status_t value.
     */
    advertising_data_status_t(type value) : SafeEnum(value)
    {
    }

    /**
     * Explicit constructor from a raw value.
     */
    explicit advertising_data_status_t(uint8_t raw_value) :
        SafeEnum(static_cast<advertising_data_status_t>(raw_value))
    {
    }
};

/** Properties of an advertising event.
 */
struct advertising_event_t {
    /** Create based on a raw value.
     *
     * @param value
     */
    explicit advertising_event_t(uint8_t value) : value(value)
    {
    }

    /** Is advertising connectable.
     *
     * @return True if connectable.
     */
    bool connectable() const
    {
        return static_cast<bool>(value & (1 << 0));
    }

    /** Is advertising scannable.
     *
     * @return True if scannable
     */
    bool scannable_advertising() const
    {
        return static_cast<bool>(value & (1 << 1));
    }

    /** Is advertising directed.
     *
     * @return True if directed.
     */
    bool directed_advertising() const
    {
        return static_cast<bool>(value & (1 << 2));
    }

    /** Is this a scan response.
     *
     * @return True if scan response.
     */
    bool scan_response() const
    {
        return static_cast<bool>(value & (1 << 3));
    }

    /** Is this legacy advertising.
     *
     * @return True if legacy.
     */
    bool legacy_advertising() const
    {
        return static_cast<bool>(value & (1 << 4));
    }

    /** Payload completeness status.
     *
     * @return @see advertising_data_status_t for details.
     */
    advertising_data_status_t data_status() const
    {
        return static_cast<advertising_data_status_t::type>((value >> 5) & 0x03);
    }

    /** Is payload complete.
     *
     * @return True if payload is coplete.
     */
    bool complete() const
    {
        return data_status().value() == advertising_data_status_t::COMPLETE;
    }

    /** Is there more data coming.
     *
     * @return True if more data coming.
     */
    bool more_data_to_come() const
    {
        return data_status().value() == advertising_data_status_t::INCOMPLETE_MORE_DATA;
    }

    /** Is the payload truncated.
     *
     * @return True if no more data coming.
     */
    bool truncated() const
    {
        return data_status().value() == advertising_data_status_t::INCOMPLETE_DATA_TRUNCATED;
    }

private:
    uint8_t value;
};

/**
 * Identify an advertising SID. Range: [0x00, 0x0F]
 */
typedef uint8_t advertising_sid_t;

/** Received signal strength. Range <-127, 20>.
 *  @note Special value 127 means RSSI is not available. */
typedef int8_t rssi_t;

/**
 * Describe the advertising power.
 *
 * Value comprised between -127 and +126 are considered power values in dBm while
 * the special value 127 can be used as a wildcard to indicates that the host
 * has no preference or if the power information is not available.
 */
typedef int8_t advertising_power_t;

/**
 * Advertising policy filter modes.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.2.
 */
struct advertising_filter_policy_t : SafeEnum<advertising_filter_policy_t, uint8_t> {
    enum type {
        /**
         * Process connection and scan requests from all devices. The whitelist is
         * not used.
         */
            NO_FILTER = 0x00,

        /**
         * Process connection requests from all devices but filter out scan requests
         * of devices which are not in the whitelist.
         */
            FILTER_SCAN_REQUESTS = 0x01,

        /**
         * Process scan requests from all devices but filter out connection requests
         * of devices which are not in the whitelist.
         */
            FILTER_CONNECTION_REQUEST = 0x02,

        /**
         * Filter out scan or connection requests of devices which are not in the
         * whitelist.
         */
            FILTER_SCAN_AND_CONNECTION_REQUESTS = 0x03
    };

    advertising_filter_policy_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Scanning policy filter mode.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.3.
 */
struct scanning_filter_policy_t : SafeEnum<scanning_filter_policy_t, uint8_t> {
    enum type {
        /**
        * Accept all advertising packets except directed advertising packet not
        * addressed to this device.
        */
        NO_FILTER = 0x00,

        /**
        * Accept only advertising packets from devices in the whitelist except
        * directed advertising packet not addressed to this device.
        */
        FILTER_ADVERTISING = 0x01,


        /**
         * Accept all advertising packets except directed advertising packets
         * where the initiator's identity address does not address this device.
         *
         * @note Directed advertising packets where the initiator's address is a
         * resolvable private address that cannot be resolved are also accepted.
         */
        NO_FILTER_INCLUDE_UNRESOLVABLE_DIRECTED = 2,

        /**
         * Accept all advertising packets except:
         * - advertising packets where the advertiser's
         * identity address is not in the White List,
         * - directed advertising packets where the initiator's identity address
         * does not address this device.
         *
         * @note Directed advertising packets where the initiator's address is a
         * resolvable private address that cannot be resolved are also accepted.
         */
        FILTER_ADVERTISING_INCLUDE_UNRESOLVABLE_DIRECTED = 3
    };

    /**
     * Construct a new instance of scanning_filter_policy_t.
     */
    scanning_filter_policy_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Filter policy which can be used during connection initiation.
 */
struct initiator_filter_policy_t : SafeEnum<initiator_filter_policy_t, uint8_t> {
    enum type {
        /**
        * The whitelist is not used to determine which advertiser to connect to.
        */
        NO_FILTER,

        /**
        * Whitelist is used to determine which advertiser to connect to.
        */
        USE_WHITE_LIST
    };

    initiator_filter_policy_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Scanning policy filter mode.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.3.
 */
struct duplicates_filter_t : SafeEnum<duplicates_filter_t, uint8_t> {
    enum type {
        /**
         * Disable duplicate filtering.
         */
        DISABLE,

        /**
         * Enable duplicate filtering.
         */
        ENABLE,

        /**
         * Enable duplicate filtering, reset the cache periodically.
         */
        PERIODIC_RESET
    };

    /**
     * Construct a new duplicates_filter_t value.
     */
    duplicates_filter_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Type used to model the own address used during the following GAP operations:
 * advertising, scanning and initiating
 */
struct own_address_type_t : SafeEnum<own_address_type_t, uint8_t> {
    enum type {
        /**
         * Use the public device address.
         */
        PUBLIC_ADDRESS = 0x00,
        PUBLIC = 0x00,

        /**
         * Use the random device address.
         */
        RANDOM_ADDRESS = 0x01,
        RANDOM = 0x01,

        /**
         * Generated resolvable private address based on the local IRK from the
         * resolving list. Use the public address if no entry match in the resolving
         * list.
         */
        RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK = 0x02,

        /**
         * Generated resolvable private address based on the local IRK from the
         * resolving list. Use the random address if no entry match in the resolving
         * list.
         */
        RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK = 0x03,
    };

    /**
     * Construct a new instance of own_address_type_t.
     */
    own_address_type_t(type value) : SafeEnum(value)
    {
    }
};

struct target_peer_address_type_t : SafeEnum<target_peer_address_type_t, uint8_t> {
    enum type {
        PUBLIC = 0x00, /**< Public Device Address or Public Identity Address. */
        PUBLIC_ADDRESS = 0x00,

        RANDOM = 0x01, /**< Random Device Address or Random (static) Identity Address. */
        RANDOM_ADDRESS = 0x01
    };
    target_peer_address_type_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Accuracy of the master clock.
 */
struct clock_accuracy_t : SafeEnum<clock_accuracy_t, uint8_t> {
    enum type {
        /**
         * 500 PPM
         */
        PPM_500 = 0x00,

        /**
         * 250 PPM
         */
        PPM_250 = 0x01,

        /**
         * 150 PPM
         */
        PPM_150 = 0x02,

        /**
         * 100 PPM
         */
        PPM_100 = 0x03,

        /**
         * 75 PPM
         */
        PPM_75 = 0x04,

        /**
         * 50 PPM
         */
        PPM_50 = 0x05,

        /**
         * 30 PPM
         */
        PPM_30 = 0x06,

        /**
         * 20 PPM
         */
        PPM_20 = 0x07
    };

    /** Get clock accuracy.
     *
     * @return Parts per million as a number.
     */
    uint16_t get_ppm()
    {
        switch (value()) {
            case PPM_500:
                return 500;
            case PPM_250:
                return 250;
            case PPM_150:
                return 150;
            case PPM_100:
                return 100;
            case PPM_75:
                return 75;
            case PPM_50:
                return 50;
            case PPM_30:
                return 30;
            case PPM_20:
                return 20;
            default:
                return 0;
        }
    }

    /**
     * Construct a new clock_accuracy_t value.
     */
    clock_accuracy_t(type value) : SafeEnum(value)
    {
    }

    /**
     * Construct a new clock_accuracy_t value from a raw value.
     * @param raw_value The value of the clock accuracy.
     */
    explicit clock_accuracy_t(uint8_t raw_value) : SafeEnum(raw_value)
    {
    }
};

/**
 * Enumeration of GAP roles.
 *
 * @note The BLE API does not express the broadcaster and scanner roles.
 *
 * @attention A device can fulfill different roles concurrently.
 */
struct connection_role_t : SafeEnum<connection_role_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        /**
         * Central Role.
         *
         * The device can scan and initiate connection to peripherals. It
         * acts as the master when a connection is established.
         *
         * @note A central is a scanner.
         */
        CENTRAL = 0x00,

        /**
         * @see CENTRAL
         */
        MASTER = 0x00,

        /**
         * Peripheral Role.
         *
         * The device can advertise and it can be connected by a central. It
         * acts as a slave when connected.
         *
         * @note A peripheral is a broadcaster.
         */
        PERIPHERAL = 0x01,

        /**
         * @see PERIPHERAL
         */
        SLAVE = 0x01
    };

    /**
     * Construct a new instance of role_t.
     */
    connection_role_t(type value) : SafeEnum(value)
    {
    }


    /**
     * Explicit constructor from a raw value.
     * @param raw_value The role.
     */
    explicit connection_role_t(uint8_t raw_value) : SafeEnum(raw_value)
    {
    }
};

/**
 * Enumeration of disconnection reasons that should be transmited to the peer.
 */
struct local_disconnection_reason_t : SafeEnum<local_disconnection_reason_t, uint8_t> {
    enum type {
        /**
         * GAP or GATT failed to authenticate the peer.
         */
        AUTHENTICATION_FAILURE = 0x05,
        AUTHENTICATION_FAILLURE = 0x05,

        /**
         * Connection terminated by the user.
         */
        USER_TERMINATION = 0x13,
        REMOTE_USER_TERMINATED_CONNECTION = 0x13,

        /**
         * Connection termination due to low resources.
         */
        LOW_RESOURCES = 0x14,
        REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES = 0x14,

        /**
         * Connection termination due to power off.
         */
        POWER_OFF = 0x15,
        REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF = 0x15,

        /**
         * Remote feature not supported
         */
        UNSUPPORTED_REMOTE_FEATURE = 0x1A,

        /**
         * Not possible to pai with a unit key.
         */
        PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29,

        /**
         * Connection parameters were unacceptable.
         */
        CONN_INTERVAL_UNACCEPTABLE = 0x3B,
        UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B
    };

    /**
     * Construct a new instance of disconnection_reason_t.
     */
    local_disconnection_reason_t(type value) : SafeEnum(value)
    {
    }
};


/**
 * Enumeration of disconnection reasons received in a disconnection event.
 */
struct disconnection_reason_t : SafeEnum<disconnection_reason_t, uint8_t> {
    enum type {
        /**
         * GAP or GATT failed to authenticate the peer.
         */
        AUTHENTICATION_FAILURE = 0x05,

        /**
         * The connection timed out.
         */
        CONNECTION_TIMEOUT = 0x08,

        /**
         * Connection terminated by the user.
         */
        REMOTE_USER_TERMINATED_CONNECTION = 0x13,

        /**
         * Remote device terminated connection due to low resources.
         */
        REMOTE_DEV_TERMINATION_DUE_TO_LOW_RESOURCES = 0x14,

        /**
         * Remote device terminated connection due to power off.
         */
        REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF = 0x15,

        /**
         * Indicate that the local user or the internal
         * Bluetooth subsystem terminated the connection.
         */
        LOCAL_HOST_TERMINATED_CONNECTION = 0x16,

        /**
         * Connection parameters were unacceptable.
         */
        CONN_INTERVAL_UNACCEPTABLE = 0x3B
    };

    /**
     * Construct a new instance of disconnection_reason_t.
     */
    disconnection_reason_t(type value) : SafeEnum(value)
    {
    }
};


} // namespace ble

#endif //BLE_GAP_TYPES_H
