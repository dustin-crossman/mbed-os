/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef MBED_ADVERTISING_PARAMETERS_H__
#define MBED_ADVERTISING_PARAMETERS_H__

#include <algorithm>

#include "BLETypes.h"
#include "BLEProtocol.h"
#include "blecommon.h"
#include "SafeEnum.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

namespace ble {

/**
 * Parameters defining the advertising process.
 *
 * Advertising parameters for legacy advertising are a triplet of three value:
 *   - The Advertising mode modelled after ble::advertising_type_t. It defines
 *     if the device is connectable and scannable. This value can be set at
 *     construction time, updated with setAdvertisingType() and queried by
 *     getAdvertisingType().
 *   - Time interval between advertisement. It can be set at construction time,
 *     updated by setInterval() and obtained from getInterval().
 *   - Duration of the advertising process. As others, it can be set at
 *     construction time, modified by setTimeout() and retrieved by getTimeout().
 */
class AdvertisingParameters {

    static const uint32_t DEFAULT_ADVERTISING_INTERVAL_MIN = 0x400;

    static const uint32_t DEFAULT_ADVERTISING_INTERVAL_MAX = 0x800;

    /**
     * Minimum Advertising interval for scannable and nonconnectable
     * undirected events in 625us units.
     *
     * @note Equal to 100ms.
     */
    static const uint32_t GAP_ADV_PARAMS_INTERVAL_MIN_NONCON = 0x00A0;

public:
    /**
     * Construct an instance of GapAdvertisingParams.
     *
     * @param[in] advType Type of advertising.
     * @param[in] minInterval Time interval between two advertisement in units of
     * 0.625ms.
     * @param[in] timeout Duration in seconds of the advertising process. A
     * value of 0 indicate that there is no timeout of the advertising process.
     *
     * @note If value in input are out of range, they will be normalized.
     */
    AdvertisingParameters(
        advertising_type_t advType = advertising_type_t::ADV_CONNECTABLE_UNDIRECTED,
        adv_interval_t minInterval = adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MIN),
        adv_interval_t maxInterval = adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MAX)
    ) :
        _advType(advType),
        _minInterval(minInterval),
        _maxInterval(maxInterval),
        _peerAddressType(ble::target_peer_address_type_t::PUBLIC),
        _ownAddressType(ble::own_address_type_t::PUBLIC),
        _policy(advertising_filter_policy_t::NO_FILTER),
        _primaryPhy(ble::phy_t::LE_1M),
        _secondaryPhy(ble::phy_t::LE_1M),
        _peerAddress(),
        _txPower(127),
        _maxSkip(0),
        _channel37(true),
        _channel38(true),
        _channel39(true),
        _anonymous(false),
        _notifyOnScan(false),
        _legacyPDU(true),
        _includeHeaderTxPower(false)
    {
        /* Min interval is slightly larger than in other modes. */
        if (_advType == advertising_type_t::ADV_NON_CONNECTABLE_UNDIRECTED) {
            _minInterval = adv_interval_t(std::max(_minInterval.value(), GAP_ADV_PARAMS_INTERVAL_MIN_NONCON));
            _maxInterval = adv_interval_t(std::max(_maxInterval.value(), GAP_ADV_PARAMS_INTERVAL_MIN_NONCON));
        }
    }

public:

    /**
     * Update the advertising type.
     *
     * @param[in] newAdvType The new advertising type.
     *
     * @return reference to this object.
     */
    AdvertisingParameters &setType(advertising_type_t newAdvType)
    {
        _advType = newAdvType;
        return *this;
    }

    /**
     * Return advertising type.
     *
     * @return Advertising type.
     */
    advertising_type_t getType() const
    {
        return _advType;
    }

    /** Set the advertising intervals on the primary channels.
     *
     * @param min Minimum interval .
     * @param max Maximum interval .
     *
     * @return reference to this object.
     */
    AdvertisingParameters& setPrimaryInterval(
        adv_interval_t min , adv_interval_t max
    )
    {
        _minInterval = min;
        _maxInterval = max;
        return *this;
    }

    /** Get the minimum advertising intervals on the primary channels.
     *
     * @return The lower bound of the primary interval selected.
     */
    adv_interval_t getMinPrimaryInterval() const
    {
        return _minInterval;
    }

    /** Get the maximum advertising intervals on the primary channels.
     *
     * @return The higher bound of the primary interval selected.
     */
    adv_interval_t getMaxPrimaryInterval() const
    {
        return _maxInterval;
    }

    /** Set which channels are to be used for primary advertising.
     *  At least must be used. If all are set to disabled all channels will be used.
     *
     * @param channel37 Use channel 37.
     * @param channel38 Use channel 38.
     * @param channel39 Use channel 39.
     *
     * @return a reference to this object.
     */
    AdvertisingParameters &setPrimaryChannels(
        bool channel37, bool channel38, bool channel39
    )
    {
        if (!channel37 && !channel38 && !channel39) {
            channel37 = channel38 = channel39 = true;
        }
        _channel37 = channel37;
        _channel38 = channel38;
        _channel39 = channel39;
        return *this;
    }

    /** Check if channel 37 is used for primary advertising.
     *
     * @return True if channel used.
     */
    bool getChannel37() const
    {
        return _channel37;
    }


    /** Check if channel 38 is used for primary advertising.
     *
     * @return True if channel used.
     */
    bool getChannel38() const
    {
        return _channel38;
    }


    /** Check if channel 39 is used for primary advertising.
     *
     * @return True if channel used.
     */
    bool getChannel39() const
    {
        return _channel39;
    }

    /** Get what type of address is to be used as your own address during advertising.
     *
     * @return a reference to this object.
     */
    AdvertisingParameters &setOwnAddressType(own_address_type_t addressType)
    {
        _ownAddressType = addressType;
        return *this;
    }

    /** Get what type of address is to be used as your own address during advertising.
     *
     * @return Addres tpe used.
     */
    own_address_type_t getOwnAddressType() const
    {
        return _ownAddressType;
    }

    /** Set peer address and type used during directed advertising.
     *
     * @param address Peer's address bytes.
     * @param addressType Peer's address type.
     *
     * @return a reference to this object.
     */
    AdvertisingParameters &setPeer(
        const address_t &address,
        target_peer_address_type_t addressType
    ) {
        _peerAddress = address;
        _peerAddressType = addressType;
        return *this;
    };

    /** Get the peer address used during directed advertising.
     *
     * @return Address of the peer targeted by directed advertising.
     */
    const address_t &getPeerAddress() const
    {
        return _peerAddress;
    };


    /** Get the peer address type used during directed advertising.
     *
     * @return The type of address of the peer targeted by directed advertising.
     */
    target_peer_address_type_t getPeerAddressType() const
    {
        return _peerAddressType;
    };

    /** Set the policy of whitelist use during advertising;
     *
     * @param Policy to use.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setPolicyMode(advertising_filter_policy_t mode)
    {
        _policy = mode;
        return *this;
    }

    /** Get the policy of whitelist use during advertising;
     *
     * @return Policy used.
     */
    ble::advertising_filter_policy_t getPolicyMode() const
    {
        return _policy;
    }

    /* Extended advertising parameters */


    /** Get PHYs used on primary and secondary advertising channels.
     *
     * @param primaryPhy Primary advertising channels PHY.
     * @param secondaryPhy Secondary advertising channels PHY.
     *
     * @return A reference to this.
     */
    AdvertisingParameters &setPhy(ble::phy_t primaryPhy, ble::phy_t secondaryPhy)
    {
        _primaryPhy = primaryPhy;
        _secondaryPhy = secondaryPhy;
        return *this;
    }

    /** Get PHY used for primary advertising.
     *
     * @return PHY used for primary advertising.
     */
    ble::phy_t getPrimaryPhy() const
    {
        return _primaryPhy;
    }

    /** Get PHY used for secondary advertising.
     *
     * @return PHY used for secondary advertising.
     */
    ble::phy_t getSecondaryPhy() const
    {
        return _secondaryPhy;
    }

    /** Set the advertising TX power.
     *
     * @param txPower Advertising TX power.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setTxPower(advertising_power_t txPower)
    {
        _txPower = txPower;
        return *this;
    }

    /** Get the advertising TX power.
     *
     * @return Advertising TX power.
     */
    advertising_power_t getTxPower() const
    {
        return _txPower;
    }

    /** Set how many events can be skipped on the secondary channel.
     *
     * @param eventNumber Number of events that can be skipped.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setSecondaryMaxSkip(uint8_t eventNumber)
    {
        _maxSkip = eventNumber;
        return *this;
    }

    /** Return how many events can be skipped on the secondary channel.
     *
     * @return How many events can be skipped on the secondary channel.
     */
    uint8_t getSecondaryMaxSkip() const
    {
        return _maxSkip;
    }

    /** Enabled or disable the callback that notifies the user about a scan request.
     *
     * @param enable Enable callback if true.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setScanRequestNotification(bool enable = true)
    {
        _notifyOnScan = enable;
        return *this;
    }

    /** Return of the callback for scan request is enabled.
     *
     * @return True if callback is enabled.
     */
    bool getScanRequestNotification() const
    {
        return _notifyOnScan;
    }

    /** Use legacy PDU during advertising.
     *
     * @param enable If true legacy PDU will be used.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setUseLegacyPDU(bool enable = true)
    {
        _legacyPDU = enable;
        return *this;
    }

    /** Check if legacy PDU is used during advertising.
     *
     * @return True legacy PDU will be used.
     */
    bool getUseLegacyPDU() const
    {
        return _legacyPDU;
    }

    /** Set if TX power should be included in the header.
     *
     * @param enable If true include the TX power in the header.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &includeTxPowerInHeader(bool enable = true)
    {
        _includeHeaderTxPower = enable;
        return *this;
    }

    /** Check if TX power should be included in the header.
     *
     * @return True if TX power is included in the header.
     */
    bool getTxPowerInHeader() const
    {
        return _includeHeaderTxPower;
    }

    /** Advertise without your own address.
     *
     * @param enable Advertising anonymous if true.
     *
     * @return reference to this object.
     */
    AdvertisingParameters &setAnonymousAdvertising(bool enable)
    {
        _anonymous = enable;
        return *this;
    }

    /** Check if advertising is anonymous.
     *
     * @return True if advertising is anonymous.
     */
    bool getAnonymousAdvertising() const
    {
        return _anonymous;
    }

private:
    advertising_type_t _advType;
    /* The advertising interval in ADV duration units (in other words, 0.625ms). */
    adv_interval_t _minInterval;
    /* The advertising max interval in ADV duration units (in other words, 0.625ms) used in extended advertising. */
    adv_interval_t _maxInterval;

    target_peer_address_type_t _peerAddressType;
    own_address_type_t _ownAddressType;
    advertising_filter_policy_t _policy;
    phy_t _primaryPhy;
    phy_t _secondaryPhy;
    address_t _peerAddress;
    advertising_power_t _txPower;
    uint8_t _maxSkip;
    bool _channel37:1;
    bool _channel38:1;
    bool _channel39:1;
    bool _anonymous:1;
    bool _notifyOnScan:1;
    bool _legacyPDU:1;
    bool _includeHeaderTxPower:1;
};

} // namespace ble

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_ADVERTISING_PARAMETERS_H__ */
