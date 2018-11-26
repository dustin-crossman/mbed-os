/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef BLE_TYPES_H_
#define BLE_TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "ble/SafeEnum.h"
#include "ble/ArrayView.h"
#include "ble/common/Duration.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

namespace ble {

/* replace with std::clamp when it arrives */
template<typename T, typename R>
void clamp(T& value, const R& min, const R& max) {
    if (value > max) {
        value = max;
    } else if (value < min) {
        value = min;
    }
}

template<typename T, int32_t Min, int32_t Max>
struct Bounded {
    Bounded(T v) : _value(v) {
        if (v < Min) {
            _value = v;
        } else if (v > Max) {
            _value = v;
        }
    }

    T value() const {
        return _value;
    }

    static const T min = Min;
    static const T max = Max;

private:
    T _value;
};

typedef Duration<uint32_t,      625, Range<0x20, 0xFFFFFF> > unit_adv_interval_t;
typedef Duration<uint16_t,    10000, Range<0x00,   0xFFFF> > unit_adv_duration_t;
typedef Duration<uint16_t,    10000, Range<0x00,   0xFFFF> > unit_scan_duration_t;
typedef Duration<uint16_t,  1280000, Range<0x00,   0xFFFF> > unit_scan_period_t;
typedef Duration<uint16_t,      625, Range<0x04,   0xFFFF> > unit_scan_interval_t;
typedef Duration<uint16_t,      625, Range<0x04,   0xFFFF> > unit_scan_window_t;
typedef Duration<uint16_t,     1250, Range<0x06,   0x0C80> > unit_conn_interval_t;
typedef Duration<uint16_t,    10000, Range<0x0A,   0x0C80> > unit_supervision_timeout_t;
typedef Duration<uint16_t,      625, Range<   0,   0xFFFF> > unit_conn_event_length_t;
typedef Duration<uint16_t,    10000, Range<0x0A,   0x4000> > unit_sync_timeout_t;
typedef Duration<uint16_t,     1250, Range<0x06,   0xFFFF> > unit_periodic_interval_t;

typedef Duration<uint32_t, 1000> unit_ms_t;
typedef Duration<uint32_t,    1> unit_us_t;

typedef Bounded<uint16_t, 0, 0x01F3> unit_slave_latency_t;

/**
 * Opaque reference to a connection.
 *
 * Internally a connection handle is an unsigned integer capable of holding a
 * pointer.
 *
 * The real type (either a pointer to an object or an integer) is opaque for
 * users and platform dependent.
 */
typedef uintptr_t connection_handle_t;

/**
 * Reference to an attribute in a GATT database.
 */
typedef uint16_t attribute_handle_t;

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
  * Inclusive range of GATT attributes handles.
  *
  * @note Instances can be constructed with the help of the factory function
  * attribute_handle_range().
  */
struct attribute_handle_range_t {
    /**
     * Beginning of the range.
     */
    attribute_handle_t begin;

    /**
     * End of the range.
     */
    attribute_handle_t end;

    /**
     * Equal operator for attribute_handle_range_t.
     *
     * @param[in] lhs Left hand side of the expression.
     * @param[in] rhs Right hand side of the expression.
     *
     * @return true if lhs is equal to rhs and false otherwise.
     */
    friend bool operator==(
        const attribute_handle_range_t &lhs, const attribute_handle_range_t &rhs
    ) {
        return (lhs.begin == rhs.begin) && (lhs.end == rhs.end);
    }

    /**
     * Not equal operator for attribute_handle_range_t.
     *
     * @param[in] lhs Left hand side of the expression.
     * @param[in] rhs Right hand side of the expression.
     *
     * @return true if lhs is not equal to rhs and false otherwise.
     */
    friend bool operator!=(
        const attribute_handle_range_t &lhs, const attribute_handle_range_t &rhs
    ) {
        return !(lhs == rhs);
    }
};


/**
 * Construct an attribute_handle_range_t from its first and last attribute handle.
 *
 * @param[in] begin Handle at the beginning of the range.
 * @param[in] end Handle at the end of the range.
 *
 * @return An instance of attribute_handle_range_t where
 * attribute_handle_range_t::begin is equal to begin and
 * attribute_handle_range_t::end is equal to end.
 *
 * @note This function is defined instead of a constructor to keep "POD-ness"
 * of attribute_handle_range_t.
 */
static inline attribute_handle_range_t attribute_handle_range(
    attribute_handle_t begin,
    attribute_handle_t end
) {
    attribute_handle_range_t result = {
        begin,
        end
    };
    return result;
}

/**
 * Encapsulates the peripheral advertising modes.
 *
 * It determine how the device appears to other scanner and peripheral
 * devices in the scanning range.
 */
enum advertising_type_t {
    /**
     * Device is connectable, scannable and doesn't expect connection from a
     * specific peer.
     *
     * @see Vol 3, Part C, Section 9.3.4 and Vol 6, Part B, Section 2.3.1.1.
     */
    ADV_CONNECTABLE_UNDIRECTED,

    /**
     * Device is connectable and expects connection from a specific peer.
     * (3.75 ms or smaller Advertising Interval)
     * @see Vol 3, Part C, Section 9.3.3 and Vol 6, Part B, Section 2.3.1.2.
     */
    ADV_CONNECTABLE_DIRECTED,

    /**
     * Device is scannable but not connectable.
     *
     * @see Vol 6, Part B, Section 2.3.1.4.
     */
    ADV_SCANNABLE_UNDIRECTED,

    /**
     * Device is not connectable and not scannable.
     *
     * @see Vol 3, Part C, Section 9.3.2 and Vol 6, Part B, Section 2.3.1.3.
     */
    ADV_NON_CONNECTABLE_UNDIRECTED,

    /**
     * Device is connectable and expects connection from a specific peer (sent at long user set intervals).
     */
    ADV_CONNECTABLE_DIRECTED_LOW_DUTY
};

/** Used to indicate if the packet is complete and if it's truncated.
 */
struct advertising_data_status_t :  SafeEnum<advertising_data_status_t, uint8_t >{
    enum type {
        COMPLETE = 0x00, /**< Advertising payload complete. */
        INCOMPLETE_MORE_DATA = 0x01, /**< Partial advertising payload, more to come. */
        INCOMPLETE_DATA_TRUNCATED = 0x02 /**< Advertising payload incomplete and no more is comoing. */
    };

    /**
     * Construct a new advertising_data_status_t value.
     */
    advertising_data_status_t(type value) :
        SafeEnum<advertising_data_status_t, uint8_t>(value) { }

    /**
     * Explicit constructor from a raw value.
     */
    explicit advertising_data_status_t(uint8_t raw_value) :
        SafeEnum<advertising_data_status_t, uint8_t>(
            static_cast<advertising_data_status_t>(raw_value)
        )
    { }
};

/** Properties of an advertising event.
 */
struct advertising_event_t {
    /** Create based on a raw value.
     *
     * @param value
     */
    explicit advertising_event_t(uint8_t value) : value(value) { }

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
enum advertising_policy_mode_t {
    /**
     * The whitelist is not used to filter peer request during advertising.
     */
    ADV_POLICY_IGNORE_WHITELIST = 0,

    /**
     * The whitelist is used to filter peer scan requests.
     */
    ADV_POLICY_FILTER_SCAN_REQS = 1,

    /**
     * The whitelist is used to filter peer connection requests.
     */
    ADV_POLICY_FILTER_CONN_REQS = 2,

    /**
     * The whitelist is used to filter peer scan and connection requests.
     */
    ADV_POLICY_FILTER_ALL_REQS  = 3,
};

/**
 * Scanning policy filter mode.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.3.
 */
enum scanning_policy_mode_t {
    /**
     * The whitelist is not used for scanning operations.
     */
    SCAN_POLICY_IGNORE_WHITELIST = 0,

    /**
     * The whitelist is used to filter incoming advertising.
     */
    SCAN_POLICY_FILTER_ALL_ADV = 1,

    /**
     * Accept all advertising packets except directed advertising packets
     * where the initiator's identity address does not address this device.
     *
     * @note Directed advertising packets where the initiator's address is a
     * resolvable private address that cannot be resolved are also accepted.
     */
    SCAN_POLICY_IGNORE_WHITELIST_INCLUDE_UNRESOLVABLE = 2,

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
    SCAN_POLICY_FILTER_ALL_ADV_INCLUDE_UNRESOLVABLE = 3
};

/**
 * Scanning policy filter mode.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.3.
 */
enum scanning_filter_duplicates_t {
    SCAN_FILTER_DUPLICATES_DISABLED = 0, /**< Do not remove duplicate reports. */
    SCAN_FILTER_DUPLICATES_ENABLED = 1, /**< Remove duplicate reports. */
    SCAN_FILTER_DUPLICATES_ENABLED_PER_PERIOD = 2 /**< Remove reports which are duplicate
                                                       within a single period. */
};

/**
 * Type that describes link's encryption state.
 */
struct link_encryption_t : SafeEnum<link_encryption_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        NOT_ENCRYPTED,          /**< The link is not secured. */
        ENCRYPTION_IN_PROGRESS, /**< Link security is being established. */
        ENCRYPTED,              /**< The link is secure. */
        ENCRYPTED_WITH_MITM,    /**< The link is secure and authenticated. */
        ENCRYPTED_WITH_SC_AND_MITM  /**< The link is secure and authenticated with a secure connection key. */
    };

    /**
     * Construct a new instance of link_encryption_t.
     */
    link_encryption_t(type value) : SafeEnum<link_encryption_t, uint8_t>(value) { }
};

/**
 * Type that describe a pairing failure.
 */
struct pairing_failure_t : SafeEnum<pairing_failure_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        PASSKEY_ENTRY_FAILED = 0x01,
        OOB_NOT_AVAILABLE = 0x02,
        AUTHENTICATION_REQUIREMENTS = 0x03,
        CONFIRM_VALUE_FAILED = 0x04,
        PAIRING_NOT_SUPPORTED = 0x05,
        ENCRYPTION_KEY_SIZE = 0x06,
        COMMAND_NOT_SUPPORTED = 0x07,
        UNSPECIFIED_REASON = 0x08,
        REPEATED_ATTEMPTS = 0x09,
        INVALID_PARAMETERS = 0x0A,
        DHKEY_CHECK_FAILED = 0x0B,
        NUMERIC_COMPARISON_FAILED = 0x0c,
        BR_EDR_PAIRING_IN_PROGRESS = 0x0D,
        CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED = 0x0E
    };

    /**
     * Construct a new instance of pairing_failure_t.
     */
    pairing_failure_t(type value) : SafeEnum<pairing_failure_t, uint8_t>(value) { }
};


/**
 * Type that describe the IO capability of a device; it is used during Pairing
 * Feature exchange.
 */
struct io_capability_t : SafeEnum<io_capability_t, uint8_t> {
    enum type {
        DISPLAY_ONLY = 0x00,
        DISPLAY_YES_NO = 0x01,
        KEYBOARD_ONLY = 0x02,
        NO_INPUT_NO_OUTPUT = 0x03,
        KEYBOARD_DISPLAY = 0x04
    };

    /**
     * Construct a new instance of io_capability_t.
     */
    io_capability_t(type value) : SafeEnum<io_capability_t, uint8_t>(value) { }
};

/**
 * Passkey stored as a number.
 */
typedef uint32_t passkey_num_t;

/**
 * Passkey stored as a string of digits.
 */
class PasskeyAscii {
public:
    static const uint8_t PASSKEY_LEN = 6;
    static const uint8_t NUMBER_OFFSET = '0';

    /**
     * Default to all zeroes
     */
    PasskeyAscii() {
        memset(ascii, NUMBER_OFFSET, PASSKEY_LEN);
    }

    /**
     * Initialize a data from a string.
     *
     * @param[in] passkey value of the data.
     */
    PasskeyAscii(const uint8_t* passkey) {
        if (passkey) {
            memcpy(ascii, passkey, PASSKEY_LEN);
        } else {
            memset(ascii, NUMBER_OFFSET, PASSKEY_LEN);
        }
    }

    /**
     * Initialize a data from a number.
     *
     * @param[in] passkey value of the data.
     */
    PasskeyAscii(passkey_num_t passkey) {
        for (int i = 5, m = 100000; i >= 0; --i, m /= 10) {
            uint32_t result = passkey / m;
            ascii[i] = NUMBER_OFFSET + result;
            passkey -= result * m;
        }
    }

    /**
     * Cast to number.
     */
    operator passkey_num_t() {
        return to_num(ascii);
    }

    /**
     * Convert ASCII string of digits into a number.
     * @param[in] ascii ASCII string of 6 digits stored as ASCII characters
     * @return Passkey as a number.
     */
    static uint32_t to_num(const uint8_t *ascii) {
        uint32_t passkey = 0;
        for (size_t i = 0, m = 1; i < PASSKEY_LEN; ++i, m *= 10) {
            passkey += (ascii[i] - NUMBER_OFFSET) * m;
        }
        return passkey;
    }

    /**
     * Return the pointer to the buffer holding the string.
     */
    uint8_t* value() {
        return ascii;
    }
private:
    uint8_t ascii[PASSKEY_LEN];
};

/**
 * Returns true if every byte is equal to zero
 */
template <class byte_array_class>
bool is_all_zeros(byte_array_class &byte_array) {
    for (size_t i = 0; i < byte_array.size(); i++) {
        if (byte_array[i] != 0) {
            return false;
        }
    }
    return true;
}

/**
 * Zero out all bytes
 */
template <class byte_array_class>
void set_all_zeros(byte_array_class &byte_array) {
    memset(&byte_array[0], 0x00, byte_array.size());
}

template <size_t array_size>
struct byte_array_t {
    /**
     * Size of the array; accessible at compile time.
     */
    static const size_t size_ = array_size;

    /**
     * Default to all zeroes
     */
    byte_array_t() {
        memset(_value, 0x00, sizeof(_value));
    }

    /**
     * Initialize a data from an array of bytes.
     *
     * @param[in] input_value value of the data.
     */
    byte_array_t(const uint8_t *input_value) {
        memcpy(_value, input_value, sizeof(_value));
    }

    /**
     * Initialize a data from an buffer of bytes.
     *
     * @param[in] input_value pointer to buffer.
     * @param[in] size buffer size
     */
    byte_array_t(const uint8_t* input_value, size_t size) {
        memcpy(_value, input_value, size);
    }

    /**
     * Equal operator between two octet types.
     */
    friend bool operator==(const byte_array_t& lhs, const byte_array_t& rhs) {
        return memcmp(lhs._value, rhs._value, sizeof(lhs._value)) == 0;
    }

    /**
     * Non equal operator between two octet types.
     */
    friend bool operator!=(const byte_array_t& lhs, const byte_array_t& rhs) {
        return !(lhs == rhs);
    }

    /**
     * Subscript operator to access data content
     */
    uint8_t& operator[](size_t i) {
        return _value[i];
    }

    /**
     * Subscript operator to access data content
     */
    uint8_t operator[](size_t i) const {
        return _value[i];
    }

    /**
     * Return the pointer to the buffer holding data.
     */
    const uint8_t* data() const {
        return _value;
    }

    /**
     * Return the pointer to the buffer holding data.
     */
    uint8_t* data() {
        return _value;
    }

    /**
     * Size in byte of a data.
     */
    static size_t size() {
        return array_size;
    }

protected:
    uint8_t _value[array_size];
};

/**
 * Construct a fixed size ArrayView from a byte_array_t.
 *
 * @param src byte_array_t to create a view from.
 *
 * @return An ArrayView to @p src.
 */
template<size_t Size>
ArrayView<uint8_t, Size> make_ArrayView(byte_array_t<Size>& src)
{
    return ArrayView<uint8_t, Size>(src.data(), src.size());
}

/**
 * Construct a fixed size ArrayView from a const byte_array_t.
 *
 * @param src byte_array_t to create a view from.
 *
 * @return An ArrayView to @p src.
 */
template<size_t Size>
ArrayView<const uint8_t, Size> make_const_ArrayView(const byte_array_t<Size>& src)
{
    return ArrayView<const uint8_t, Size>(src.data(), src.size());
}

/** 128 bit keys used by paired devices */
typedef byte_array_t<16> irk_t;
typedef byte_array_t<16> csrk_t;
typedef byte_array_t<16> ltk_t;

/** Used to identify LTK for legacy pairing connections */
typedef byte_array_t<2> ediv_t;
typedef byte_array_t<8> rand_t;

/** Out of band data exchanged during pairing */
typedef byte_array_t<16> oob_tk_t; /**< legacy pairing TK */
typedef byte_array_t<16> oob_lesc_value_t; /**< secure connections oob random 128 value */
typedef byte_array_t<16> oob_confirm_t; /**< secure connections oob confirmation value */

/** data to be encrypted */
typedef byte_array_t<16> encryption_block_t;

/** public key coordinate, two of which define the public key */
typedef byte_array_t<32> public_key_coord_t;

/** Diffie-Hellman key */
typedef byte_array_t<32> dhkey_t;

/** counter for signed data writes done by GattClient */
typedef uint32_t sign_count_t;

/**
 * MAC address data type.
 */
struct address_t : public byte_array_t<6> {
    /**
     * Create an invalid mac address, equal to 00:00:00:00:00:00
     */
    address_t() {
        memset(_value, 0x00, sizeof(_value));
    }

    /**
     * Initialize a data from an array of bytes.
     *
     * @param[in] input_value value of the data.
     */
    address_t(const uint8_t *input_value) {
        memcpy(_value, input_value, sizeof(_value));
    }
};

/**
 * Type that describes a random device address type.
 */
struct random_address_type_t : SafeEnum<random_address_type_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        STATIC, /**< Random static device address. */
        NON_RESOLVABLE_PRIVATE, /**< Random non resolvable private address. */
        RESOLVABLE_PRIVATE /**< Random resolvable private address. */
    };

    /**
     * Construct a new instance of random_address_type_t.
     */
    random_address_type_t(type value) :
        SafeEnum<random_address_type_t, uint8_t>(value) { }
};

/**
 * Security requirement that can be attached to an attribute operation.
 */
struct att_security_requirement_t : SafeEnum<att_security_requirement_t, uint8_t> {
    /**
     * Number of bits required to store the value.
     *
     * This value can be used to define a bitfield that host a value of this
     * enum.
     */
    static const uint8_t size = 2;

    /** struct scoped enum wrapped by the class */
    enum type {
        /**
         * The operation does not have security requirements.
         *
         * It is equivalent to: SecurityMode 1 level 1: No authentication, no
         * encryption and no signing required.
         *
         * @note This security mode is not applicable for signed operation.
         *
         * @note Equivalent to SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK.
         */
        NONE,

        /**
         * The operation requires security and there's no requirement towards
         * peer authentication.
         *
         * @note Security can be achieved either by signing messages or
         * encrypting the link.
         *
         * @note Signing is only applicable for signed write operations.
         *
         * @note Equivalent to SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM
         * or SecurityManager::SECURITY_MODE_SIGNED_NO_MITM.
         */
        UNAUTHENTICATED,

        /**
         * The operation requires security and the peer must be authenticated.
         *
         * @note Security can be achieved either by signing messages or
         * encrypting the link.
         *
         * @note Equivalent to SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM
         * or SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM.
         */
        AUTHENTICATED,

        /**
         * The operation require encryption with an authenticated peer that
         * paired using secure connection pairing.
         *
         * @note This security mode is not applicable for signed operation;
         * security is achieved with link encryption.
         */
        SC_AUTHENTICATED
    };

    /**
     * Construct a new instance of att_security_requirement_t.
     */
    att_security_requirement_t(type value) :
        SafeEnum<att_security_requirement_t, uint8_t>(value) { }
};

/**
 * Type that describes a peer device address type.
 */
struct peer_address_type_t :SafeEnum<peer_address_type_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        /**
         * Public device address.
         */
        PUBLIC = 0,

        /**
         * Random address.
         *
         * Use Gap::getRandomAddressType to retrieve the type of the random
         * address.
         */
        RANDOM,

        /**
         * A Public address used as a device identity address.
         */
        PUBLIC_IDENTITY,

        /**
         * A Random static address used as a device identity address.
         */
        RANDOM_STATIC_IDENTITY,

        /**
         * No address provided (anonymous advertisement).
         */
        ANONYMOUS = 0xFF
    };

    /**
     * Construct a new instance of peer_address_type_t.
     */
    peer_address_type_t(type value) :
        SafeEnum<peer_address_type_t, uint8_t>(value) { }

    /**
     * Default initialization of peer_address_type_t.
     */
    peer_address_type_t() :
        SafeEnum<peer_address_type_t, uint8_t>(PUBLIC) { }
};

struct own_address_type_t : ble::SafeEnum<own_address_type_t, uint8_t> {
    enum type {
        PUBLIC = 0, /**< Public Device Address. */
        RANDOM,     /**< Random Device Address. */
        RANDOM_RESOLVABLE_PUBLIC_FALLBACK, /**< Controller generates the Resolvable Private Address based on
                                                the local IRK from the resolving list. If the resolving list
                                                contains no matching entry, use the public address. */
        RANDOM_RESOLVABLE_RANDOM_FALLBACK  /**< Controller generates the Resolvable Private Address based on
                                                the local IRK from the resolving list. If the resolving list
                                                contains no matching entry, use previously set random address. */
    };
    own_address_type_t(type value) : ble::SafeEnum<own_address_type_t, uint8_t>(value) { }
};

struct target_peer_address_type_t : ble::SafeEnum<target_peer_address_type_t, uint8_t> {
    enum type {
        PUBLIC = 0, /**< Public Device Address or Public Identity Address. */
        RANDOM      /**< Random Device Address or Random (static) Identity Address. */
    };
    target_peer_address_type_t(type value) : ble::SafeEnum<target_peer_address_type_t, uint8_t>(value) { }
};

/**
 * Type that describes a bluetooth PHY(sical) transport.
 */
struct phy_t : SafeEnum<phy_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        /**
         * No phy selected.
         *
         * @note This value can be used to indicate the absence of phy
         */
        NONE = 0,

        /**
         * 1Mbit/s LE.
         *
         * @note This physical transport was available since Bluetooth 4.0
         */
        LE_1M = 1,

        /**
         * 2Mbit/s LE.
         *
         * Modulation is similar to LE_1M but differs in rate. Therefore range
         * performances are in the same ballpark as LE_1M while the increased rate
         * minimize time spent to transfer or receive a packet which leads to a
         * better power consumption and/or faster transfer.
         *
         * @note This transport has been introduced with the Bluetooth 5.
         * @note When operating at 2Mbit/s range is not exactly identical to the
         * range at 1Mbit/s due to a loss in sensitivity.
         */
        LE_2M = 2,

        /**
         * LE Coded PHY.
         *
         * This transport reuse the 1Mbit/s channel with different coding schemes.
         * Either two (S=2) or eight (S=8) symbols can be used to represent a
         * bit while the 1Mbit/s transport use 1 symbol to code 1 bit of data.
         *
         * Here is the data rate of the two coding schemes:
         *   - S=2: 500kbit/s
         *   - S=8: 125kbit/s
         *
         * The goal of the coded PHY is to increase the range of BLE devices.
         * Of course given it takes more time to transfer data, transmission
         * and reception last longer which leads to an increase in power
         * consumption.
         *
         * @note This transport has been introduced with the Bluetooth 5.
         */
        LE_CODED
    };

    /**
     * Construct a new instance of phy_t.
     */
    phy_t(type value) :
        SafeEnum<phy_t, uint8_t>(value) { }

    explicit phy_t(uint8_t raw_value) : SafeEnum(raw_value) { }
};

/**
 * Type that describe a set of PHY(sical) transports. This is used to
 * indicate preference for the PHY transports set within it.
 */
class phy_set_t {
public:
    enum PhysFlags_t {
        PHY_SET_1M    = 0x01,
        PHY_SET_2M    = 0x02,
        PHY_SET_CODED = 0x04
    };

    /**
     * Create set that indicates no preference.
     */
    phy_set_t() : _value(0) { }

    /**
     * Create a set based on the mask specified in the Bluetooth spec.
     *
     * @param value Octet containing the set of preferred PHYs
     */
    phy_set_t(uint8_t value) : _value(value) { }

    /**
     * Create a set based on individual settings.
     *
     * @param phy_1m Prefer LE 1M
     * @param phy_2m Prefer LE 2M if avaiable
     * @param phy_coded Prefer coded modulation if avaiable
     */
    phy_set_t(
        bool phy_1m,
        bool phy_2m,
        bool phy_coded
    ) {
        set_1m(phy_1m);
        set_2m(phy_2m);
        set_coded(phy_coded);
    }

    /** Prefer 1M PHY. */
    void set_1m(bool enabled = true) {
        if (enabled) {
            _value |= PHY_SET_1M;
        } else {
            _value &= ~PHY_SET_1M;
        }
    }

    /** Prefer 2M PHY. */
    void set_2m(bool enabled = true) {
        if (enabled) {
            _value |= PHY_SET_2M;
        } else {
            _value &= ~PHY_SET_2M;
        }
    }

    /** Prefer coded PHY. */
    void set_coded(bool enabled = true) {
        if (enabled) {
            _value |= PHY_SET_CODED;
        } else {
            _value &= ~PHY_SET_CODED;
        }
    }

    bool get_1m() const {
        return (_value & PHY_SET_1M);
    }

    bool get_2m() const {
        return (_value & PHY_SET_2M);
    }

    bool get_coded() const {
        return (_value & PHY_SET_CODED);
    }

    operator uint8_t() const {
        return _value;
    }

    uint8_t value() const {
        return _value;
    }

    uint8_t count() const {
        return (get_1m() ? 1 : 0) + (get_2m() ? 1 : 0) + (get_coded() ? 1 : 0);
    }

private:
    uint8_t _value;
};

/**
 * Type describing the number of symbols per bit in le coded PHY.
 */
struct coded_symbol_per_bit_t :SafeEnum<coded_symbol_per_bit_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        /**
         * The Number of symbol used to code a bit is undefined.
         */
        UNDEFINED,

        /**
         * Two symbols to code a bit.
         */
        S2,

        /**
         * Eight symbols to code a bit.
         */
        S8
    };

    /**
     * Construct a new instance of coded_symbol_per_bit_t.
     */
    coded_symbol_per_bit_t(type value) :
        SafeEnum<coded_symbol_per_bit_t, uint8_t>(value) { }
};

/**
 * Accuracy of the master clock.
 */
struct clock_accuracy_t : SafeEnum<clock_accuracy_t, uint8_t >{
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
    uint16_t get_ppm() {
        uint16_t ppm = 0;

        switch(value()) {
            case PPM_500:
                ppm = 500;
                break;
            case PPM_250:
                ppm = 250;
                break;
            case PPM_150:
                ppm = 150;
                break;
            case PPM_100:
                ppm = 100;
                break;
            case PPM_75:
                ppm = 75;
                break;
            case PPM_50:
                ppm = 50;
                break;
            case PPM_30:
                ppm = 30;
                break;
            case PPM_20:
                ppm = 20;
                break;
        }

        return ppm;
    }

    /**
     * Construct a new clock_accuracy_t value.
     */
    clock_accuracy_t(type value) : SafeEnum<clock_accuracy_t, uint8_t>(value) { }

    explicit clock_accuracy_t(uint8_t raw_value) :
        SafeEnum<clock_accuracy_t, uint8_t>(static_cast<type>(raw_value)) { }
};

/**
 * Enumeration of GAP roles.
 *
 * @note The BLE API does not express the broadcaster and scanner roles.
 *
 * @attention A device can fulfill different roles concurrently.
 */
struct connection_role_t :SafeEnum<connection_role_t, uint8_t> {
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
         * @see SLAVE
         */
        SLAVE = 0x01
    };

    /**
     * Construct a new instance of role_t.
     */
    connection_role_t(type value) :
        SafeEnum<connection_role_t, uint8_t>(value) { }

    /**
     * Explicit constructor from a raw value.
     * @param raw_value The role.
     */
    explicit connection_role_t(uint8_t raw_value) :
        SafeEnum<connection_role_t, uint8_t>(raw_value) { }
};

} // namespace ble

/**
 * @}
 * @}
 */

#endif /* BLE_TYPES_H_ */
