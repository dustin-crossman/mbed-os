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

#ifndef MBED_EXTENDED_CONNECT_PARAMETERS_H__
#define MBED_EXTENDED_CONNECT_PARAMETERS_H__

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

class ExtendedConnectParameters_t {
    const size_t MAX_PARAM_PHYS = 3;
public:
    ExtendedConnectParameters_t() :
        _filterPolicy(ble::scanning_policy_mode_t::SCAN_POLICY_FILTER_ALL_ADV),
        _ownAddressType(ble::own_address_type_t::PUBLIC)
    {
        for (int i = 0; i < MAX_PARAM_PHYS; ++i) {
            _scanInterval[i] = 4;
            _scanWindow[i] = 4;
            _minConnectionInterval[i] = 6;
            _maxConnectionInterval[i] = 6;
            _slaveLatency[i] = 0;
            _connectionSupervisionTimeout[i] = 0xC80;
            _minimumEventLength[i] = 0;
            _maximumEventLength[i] = 0xFFFF;
            _enabledPhy[i] = false;
        }
    };

    ExtendedConnectParameters_t& setScanParamteres(
        uint32_t scanInterval,
        uint32_t scanWindow,
        ble::phy_t phy = ble::phy_t::LE_1M
    ) {
        uint8_t phy_index = handlePhyToggle(phy, true);

        scanInterval = scanInterval[phy_index];
        scanWindow = scanWindow[phy_index];

        return *this;
    }

    ExtendedConnectParameters_t& setConnectionParamteres(
        uint16_t minConnectionInterval,
        uint16_t maxConnectionInterval,
        uint16_t slaveLatency,
        uint16_t connectionSupervisionTimeout,
        uint32_t _minimumEventLength,
        uint32_t _maximumEventLength,
        ble::phy_t phy = ble::phy_t::LE_1M
    ) {
        uint8_t phy_index = handlePhyToggle(phy, true);

        _minConnectionInterval = minConnectionInterval[phy_index];
        maxConnectionInterval = maxConnectionInterval[phy_index];
        slaveLatency = slaveLatency[phy_index];
        connectionSupervisionTimeout = connectionSupervisionTimeout[phy_index];
        _minimumEventLength = _minimumEventLength[phy_index];
        _maximumEventLength = _maximumEventLength[phy_index];

        return *this;
    }

    ExtendedConnectParameters_t& setScanParamteres(
        ble::own_address_type_t ownAddress
    ) {
        _ownAddressType = ownAddress;

        return *this;
    }

    ExtendedConnectParameters_t& setScanParamteres(
        ble::scanning_policy_mode_t filterPolicy
    ) {
        _filterPolicy = filterPolicy;

        return *this;
    }

    ExtendedConnectParameters_t& togglePhy(
        bool phy1M,
        bool phy2M,
        bool phyCoded,
    ) {
        handlePhyToggle(ble::phy_t::LE_1M, phy1M);
        handlePhyToggle(ble::phy_t::LE_2M, phy2M);
        handlePhyToggle(ble::phy_t::LE_CODED, phyCoded);

        return *this;
    }

    ExtendedConnectParameters_t& disablePhy(
        ble::phy_t phy = ble::phy_t::LE_1M
    ) {
        handlePhyToggle(phy, false);

        return *this;
    }

    ExtendedConnectParameters_t& enablePhy(
        ble::phy_t phy = ble::phy_t::LE_1M
    ) {
        handlePhyToggle(phy, true);

        return *this;
    }

    uint32_t* getScanIntervalArray() {
        return &_scanInterval[getFirstEnabledPhy()];
    }

    uint32_t* getScanWindowArray() {
        return &_scanWindow[getFirstEnabledPhy()];
    }

    uint32_t* getMinConnectionIntervalArray() {
        return &_minConnectionInterval[getFirstEnabledPhy()];
    }

    uint32_t* getMaxConnectionIntervalArray() {
        return &_maxConnectionInterval[getFirstEnabledPhy()];
    }

    uint32_t* getSlaveLatencyArray() {
        return &_slaveLatency[getFirstEnabledPhy()];
    }

    uint32_t* getConnectionSupervisionTimeoutArray() {
        return &_connectionSupervisionTimeout[getFirstEnabledPhy()];
    }

    uint32_t* getMinimumEventLengthArray() {
        return &_minimumEventLength[getFirstEnabledPhy()];
    }

    uint32_t* getMaximumEventLengthArray() {
        return &_maximumEventLength[getFirstEnabledPhy()];
    }

    uint8_t getNumberOfEnabledPhys() {
        return (_enabledPhy[ble::phy_t::LE_1M] * 1 +
            _enabledPhy[ble::phy_t::LE_2M] * 1 +
            _enabledPhy[ble::phy_t::LE_CODED] * 1);
    }

private:
    uint8_t getFirstEnabledPhy() {
        if (_enabledPhy[ble::phy_t::LE_1M]) {
            return 0;
        } else if (_enabledPhy[ble::phy_t::LE_2M]) {
            return 1;
        } else if (_enabledPhy[ble::phy_t::LE_CODED]) {
            return 2;
        }
    }

    /** Handle toggling PHYs on and off and return the correct index to use to set the configuration elements.
     *
     * @param phy Which Phy is being toggle.
     * @param enable On or Off.
     * @return The index to the array of settings.
     */
    uint8_t handlePhyToggle(ble::phy_t phy, bool enable) {
        uint8_t index = phy;

        if (_enabledPhy[phy] != enable) {
            if (phy == ble::phy_t::LE_2M) {
                if (_enabledPhy[ble::phy_t::LE_CODED]) {
                    flipCodedAnd2M();
                }
            } else if (phy == ble::phy_t::LE_CODED) {
                if (!_enabledPhy[ble::phy_t::LE_2M]) {
                    flipCodedAnd2M();
                    index = ble::phy_t::LE_2M;
                }
            }
        }

        _enabledPhy[phy] = enable;

        return index;
    }

    /** Handle the swapping of 2M and CODED so that the array is ready for the pal call. */
    void flipCodedAnd2M() {
        uint32_t scanInterval = _scanInterval[ble::phy_t::LE_2M];
        uint32_t scanWindow = _scanWindow[ble::phy_t::LE_2M];
        uint16_t minConnectionInterval = _minConnectionInterval[ble::phy_t::LE_2M];
        uint16_t maxConnectionInterval = _maxConnectionInterval[ble::phy_t::LE_2M];
        uint16_t slaveLatency = _maxConnectionInterval[ble::phy_t::LE_2M];
        uint16_t connectionSupervisionTimeout = _connectionSupervisionTimeout[ble::phy_t::LE_2M];
        uint32_t minimumEventLength = _minimumEventLength[ble::phy_t::LE_2M];
        uint32_t maximumEventLength = _maximumEventLength[ble::phy_t::LE_2M];

        _scanInterval[ble::phy_t::LE_2M] = _scanInterval[ble::phy_t::LE_CODED];
        _scanWindow[ble::phy_t::LE_2M] = _scanWindow[ble::phy_t::LE_CODED];
        _minConnectionInterval[ble::phy_t::LE_2M] = _minConnectionInterval[ble::phy_t::LE_CODED];
        _maxConnectionInterval[ble::phy_t::LE_2M] = _maxConnectionInterval[ble::phy_t::LE_CODED];
        _slaveLatency[ble::phy_t::LE_2M] = _slaveLatency[ble::phy_t::LE_CODED];
        _connectionSupervisionTimeout[ble::phy_t::LE_2M] = _connectionSupervisionTimeout[ble::phy_t::LE_CODED];
        _minimumEventLength[ble::phy_t::LE_2M] = _minimumEventLength[ble::phy_t::LE_CODED];
        _maximumEventLength[ble::phy_t::LE_2M] = _maximumEventLength[ble::phy_t::LE_CODED];

        _scanInterval[ble::phy_t::LE_CODED] = scanInterval;
        _scanWindow[ble::phy_t::LE_CODED] = scanWindow;
        _minConnectionInterval[ble::phy_t::LE_CODED] = minConnectionInterval;
        _maxConnectionInterval[ble::phy_t::LE_CODED] = maxConnectionInterval;
        _slaveLatency[ble::phy_t::LE_CODED] = slaveLatency;
        _connectionSupervisionTimeout[ble::phy_t::LE_CODED] = connectionSupervisionTimeout;
        _minimumEventLength[ble::phy_t::LE_CODED] = minimumEventLength;
        _maximumEventLength[ble::phy_t::LE_CODED] = maximumEventLength;
    }

private:
    ble::scanning_policy_mode_t _filterPolicy;
    ble::own_address_type_t _ownAddressType;

    uint32_t _scanInterval[MAX_PARAM_PHYS];
    uint32_t _scanWindow[MAX_PARAM_PHYS];
    uint16_t _minConnectionInterval[MAX_PARAM_PHYS];
    uint16_t _maxConnectionInterval[MAX_PARAM_PHYS];
    uint16_t _slaveLatency[MAX_PARAM_PHYS];
    uint16_t _connectionSupervisionTimeout[MAX_PARAM_PHYS];
    uint32_t _minimumEventLength[MAX_PARAM_PHYS];
    uint32_t _maximumEventLength[MAX_PARAM_PHYS];

    bool _enabledPhy[MAX_PARAM_PHYS];
};

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_EXTENDED_CONNECT_PARAMETERS_H__ */
