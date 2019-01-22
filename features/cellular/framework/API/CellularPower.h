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
#ifndef CELLULAR_API_CELLULARPOWER_H_
#define CELLULAR_API_CELLULARPOWER_H_

#include "nsapi_types.h"
#include "Callback.h"

namespace mbed {

/**
 *  Class CellularPower
 *
 *  An interface that provides power handling functions for modem/module.
 */
class CellularPower {
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

    /**
     * virtual Destructor
     */
    virtual ~CellularPower() {}

public:
    /** Set cellular device power on. Default implementation is empty.
     *  Device power on/off is modem/board specific behavior and must be done on inherited class if needed.
     *  Power on is done by toggling power pin/button.
     *
     *  @remark init must be called to initialize cellular device
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if not overridden by the target modem
     */
    virtual nsapi_error_t on() = 0;

    /** Set cellular device power off. Default implementation is empty.
     *  Device power on/off is modem/board specific behavior and must be done on inherited class if needed.
     *  Power off is done by toggling power pin/button.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if not overridden by the target modem
     */
    virtual nsapi_error_t off() = 0;

    /** Set cellular device power level by enabling/disabling functionality.
     *
     *  @param func_level:
     *  0   minimum functionality
     *  1   full functionality. Enable (turn on) the transmit and receive RF circuits for all supported radio access technologies.
     *      For MTs supporting +CSRA, this equals the RATs indicated by the response of +CSRA=?. Current +CSRA setting is ignored.
     *      It is not required that the MT transmit and receive RF circuits are in a disabled state for this setting to have effect.
     *  2   disable (turn off) MT transmit RF circuits only
     *  3   disable (turn off) MT receive RF circuits only
     *  4   disable (turn off) both MT transmit and receive RF circuits
     *  @param do_reset     0 for do not reset, 1 for reset the device when changing the functionality
     *
     *  @remark See 3GPP TS 27.007 CFUN for more details
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_power_level(int func_level, int do_reset = 0) = 0;

    /** Reset and wake-up cellular device.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t reset() = 0;
};

} // namespace mbed

#endif /* CELLULAR_API_CELLULARPOWER_H_ */
