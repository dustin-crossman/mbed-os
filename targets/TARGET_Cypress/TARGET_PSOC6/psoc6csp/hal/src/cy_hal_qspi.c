/***************************************************************************//**
* \file cy_hal_qspi.c
*
* Description:
* Provides a high level interface for interacting with the Cypress QSPI (SMIF). 
* This is a wrapper around the lower level PDL API.
* 
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_hal_qspi.h"
#include "cy_smif.h"
#include "cy_hal_hwmgr.h"
#include "cy_hal_interconnect.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
*       Internal
*******************************************************************************/
/* in microseconds, timeout for all blocking functions */
#define TIMEOUT_10_MS (10000UL)

#define SMIF_MAX_RX_COUNT (65536UL)
#define QSPI_DESELECT_DELAY (7UL)

/* Default QSPI configuration */
static const cy_stc_smif_config_t default_qspi_config =
{
    .mode = (uint32_t)CY_SMIF_NORMAL,
    .deselectDelay = QSPI_DESELECT_DELAY,
    .rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
    .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};

/* List of available QSPI instances */
SMIF_Type * smif_base_addresses[CY_IP_MXSMIF_INSTANCES] =
{
#ifdef SMIF0
    SMIF0,
#endif /* ifdef SMIF0 */
};

/* List of available QSPI interrupt sources */
IRQn_Type CY_QSPI_IRQ_N[CY_IP_MXSMIF_INSTANCES] =
{
#ifdef SMIF0
    smif_interrupt_IRQn,
#endif /* ifdef SMIF0 */
};

static void cy_qspi_0_irq_handler(void) __attribute__((unused));

/* Callback information struct */
static struct
{
    cy_qspi_t *obj;
    cy_qspi_irq_handler_t handler;
    void *irq_arg;
} irq_data_stc[CY_IP_MXSMIF_INSTANCES];

/* Translate PDL QSPI interrupt cause into QSPI HAL interrupt cause */
static cy_qspi_irq_event_t cy_convert_interrupt_from_pdl(uint32_t pdl_intr_cause)
{
    cy_qspi_irq_event_t intr_cause = CY_QSPI_IRQ_NONE;

    intr_cause |= pdl_intr_cause & CY_SMIF_ALIGNMENT_ERROR ? CY_QSPI_IRQ_ALIGNMENT_ERROR : 0;
    intr_cause |= pdl_intr_cause & CY_SMIF_RX_DATA_FIFO_UNDERFLOW ? CY_QSPI_IRQ_RX_DATA_FIFO_UNRFLW : 0;
    intr_cause |= pdl_intr_cause & CY_SMIF_TX_DATA_FIFO_OVERFLOW ? CY_QSPI_IRQ_TX_DATA_FIFO_OVRFLW : 0;
    intr_cause |= pdl_intr_cause & CY_SMIF_TX_COMMAND_FIFO_OVERFLOW ? CY_QSPI_IRQ_TX_CMD_FIFO_OVRFLW : 0;
    intr_cause |= pdl_intr_cause & CY_SMIF_TX_DATA_FIFO_LEVEL_TRIGGER ? CY_QSPI_IRQ_TX_DATA_FIFO_LEVEL_TRG : 0;
    intr_cause |= pdl_intr_cause & CY_SMIF_RX_DATA_FIFO_LEVEL_TRIGGER ? CY_QSPI_IRQ_RX_DATA_FIFO_LEVEL_TRG : 0;

    return intr_cause;
}

/* Translate QSPI HAL interrupt cause into QSPI PDL interrupt cause */
static uint32_t cy_convert_interrupt_to_pdl(uint32_t event_cause)
{
    uint32_t pdl_intr_cause = 0u;

    pdl_intr_cause |= event_cause & CY_QSPI_IRQ_ALIGNMENT_ERROR ? CY_SMIF_ALIGNMENT_ERROR : 0;
    pdl_intr_cause |= event_cause & CY_QSPI_IRQ_RX_DATA_FIFO_UNRFLW ? CY_SMIF_RX_DATA_FIFO_UNDERFLOW : 0;
    pdl_intr_cause |= event_cause & CY_QSPI_IRQ_TX_DATA_FIFO_OVRFLW ? CY_SMIF_TX_DATA_FIFO_OVERFLOW : 0;
    pdl_intr_cause |= event_cause & CY_QSPI_IRQ_TX_CMD_FIFO_OVRFLW ? CY_SMIF_TX_COMMAND_FIFO_OVERFLOW : 0;
    pdl_intr_cause |= event_cause & CY_QSPI_IRQ_TX_DATA_FIFO_LEVEL_TRG ? CY_SMIF_TX_DATA_FIFO_LEVEL_TRIGGER : 0;
    pdl_intr_cause |= event_cause & CY_QSPI_IRQ_RX_DATA_FIFO_LEVEL_TRG ? CY_SMIF_RX_DATA_FIFO_LEVEL_TRIGGER : 0;

    return pdl_intr_cause;
}

/*******************************************************************************
*       Dispatcher Interrrupt Service Routine
*******************************************************************************/

static void cy_qspi_intr_wrapper_indexed(uint32_t instance_num)
{
    uint32_t intr_cause_pdl = Cy_SMIF_GetInterruptStatusMasked(irq_data_stc[instance_num].obj->base);
    cy_qspi_irq_event_t irq_status = cy_convert_interrupt_from_pdl(intr_cause_pdl);

    if (NULL != irq_data_stc[instance_num].handler)
    {
        /* Call registered callbacks here */
        irq_data_stc[instance_num].handler(irq_data_stc[instance_num].irq_arg, irq_status);
    }

    Cy_SMIF_ClearInterrupt(irq_data_stc[instance_num].obj->base, intr_cause_pdl);
}

/*******************************************************************************
*       (Internal) Interrrupt Service Routines
*******************************************************************************/

/* Handler for SMIF0 */
static void cy_qspi_0_irq_handler()
{
    cy_qspi_intr_wrapper_indexed(0);
}

/* List of interrupt handler, takes instance id as idx */
static void (*cy_qspi_intr_dispatcher_table[CY_IP_MXSMIF_INSTANCES])(void) =
{
#if (CY_IP_MXSMIF_INSTANCES > 0)
        cy_qspi_0_irq_handler,
#endif /* CY_IP_MXSMIF_INSTANCES > 0 */
};

/*******************************************************************************
*       (Internal) QSPI Pin Related Functions
*******************************************************************************/

/* Check if pin valid as resource and reserve it */
static cy_rslt_t check_pin_and_reserve(cy_gpio_t pin)
{
    cy_rslt_t result;
    cy_resource_inst_t pin_rsc = cy_utils_get_gpio_resource(pin);
    result = cy_hwmgr_reserve(&pin_rsc);
    return result;
}

/* Checks what pins are provided by user and calls check_pin_and_reserve for each */
static cy_rslt_t make_pin_reservations(cy_qspi_t *obj)
{
    cy_rslt_t result;
        
    /* reserve the SCLK pin */
    result = check_pin_and_reserve(obj->pin_sclk);

    /* reserve the ssel pin */
    if (result == CY_RSLT_SUCCESS)
    {
        result = check_pin_and_reserve(obj->pin_ssel);
    }
    /* reserve the io0 pin */
    if (result == CY_RSLT_SUCCESS)
    {
        result = check_pin_and_reserve(obj->pin_io0);
    }
    /* reserve the io1 pin */
    if ((NC != obj->pin_io1) && (CY_RSLT_SUCCESS == result))
    {
        result = check_pin_and_reserve(obj->pin_io1);
    }
    /* reserve the obj->pin_io2 pin */
    if ((NC != obj->pin_io2) && (CY_RSLT_SUCCESS == result))
    {
        result = check_pin_and_reserve(obj->pin_io2);
    }
    /* reserve the obj->pin_io3 pin */
    if ((NC != obj->pin_io3) && (CY_RSLT_SUCCESS == result))
    {
        result = check_pin_and_reserve(obj->pin_io3);
    }
    /* reserve the obj->pin_io4 pin */
    if ((NC != obj->pin_io4) && (CY_RSLT_SUCCESS == result))
    {
        result = check_pin_and_reserve(obj->pin_io4);
    }
    /* reserve the obj->pin_io5 pin */
    if ((NC != obj->pin_io5) && (CY_RSLT_SUCCESS == result))
    {
        result = check_pin_and_reserve(obj->pin_io5);
    }
    /* reserve the obj->pin_io6 pin */
    if ((NC != obj->pin_io6) && (CY_RSLT_SUCCESS == result))
    {
        result = check_pin_and_reserve(obj->pin_io6);
    }
    /* reserve the obj->pin_io7 pin */
    if ((NC != obj->pin_io7) && (CY_RSLT_SUCCESS == result))
    {
        result = check_pin_and_reserve(obj->pin_io7);        
    }
    return result;
}

/* Disconnect (interconnect) pin and free it (hwmgr) as resource */
static cy_rslt_t free_pin(cy_gpio_t pin)
{
    cy_resource_inst_t rsc;
    cy_rslt_t result = cy_disconnect_pin(pin);
    if (CY_RSLT_SUCCESS == result)
    {
        rsc = cy_utils_get_gpio_resource(pin);
        result = cy_hwmgr_free(&rsc);
    }
    return result;
}

/* Free all QSPI pins */
static cy_rslt_t free_pin_connections(cy_qspi_t *obj)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    cy_rslt_t ret_result = CY_RSLT_SUCCESS;

    if (NC != obj->pin_sclk)
    {
        result = free_pin(obj->pin_sclk);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_ssel)
    {
        result = free_pin(obj->pin_ssel);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io0)
    {
        result = free_pin(obj->pin_io0);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io1)
    {
        result = free_pin(obj->pin_io1);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io2)
    {
        result = free_pin(obj->pin_io2);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io3)
    {
        result = free_pin(obj->pin_io3);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io4)
    {
        result = free_pin(obj->pin_io4);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io5)
    {
        result = free_pin(obj->pin_io5);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io6)
    {
        result = free_pin(obj->pin_io6);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    if (NC != obj->pin_io7)
    {
        result = free_pin(obj->pin_io7);
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }
    return ret_result;
}

/*******************************************************************************
*       (Internal) QSPI Config Related Functions
*******************************************************************************/

/* Translates HAL bus width to PDL bus width */
static cy_en_smif_txfr_width_t get_cy_bus_width(cy_qspi_bus_width_t bus_width)
{
    cy_en_smif_txfr_width_t cy_bus_width;

    switch (bus_width)
    {
        case CY_QSPI_CFG_BUS_SINGLE:
            cy_bus_width = CY_SMIF_WIDTH_SINGLE;
            break;

        case CY_QSPI_CFG_BUS_DUAL:
            cy_bus_width = CY_SMIF_WIDTH_DUAL;
            break;

        case CY_QSPI_CFG_BUS_QUAD:
            cy_bus_width = CY_SMIF_WIDTH_QUAD;
            break;

        case CY_QSPI_CFG_BUS_OCTAL:
            cy_bus_width = CY_SMIF_WIDTH_OCTAL;
            break;

        default:
            cy_bus_width = CY_SMIF_WIDTH_SINGLE;
    }

    return cy_bus_width;
}

/* Translates cy_qspi_command_t to cy_stc_smif_mem_cmd_t */
static void convert_to_cy_cmd_config(const cy_qspi_command_t *qspi_command, cy_stc_smif_mem_cmd_t *const cy_cmd_config)
{
    /* This function does not check 'disabled' of each sub-structure in qspi_command_t
    *  It is the responsibility of the caller to check it. */
    cy_cmd_config->command = qspi_command->instruction.value;
    cy_cmd_config->cmdWidth = get_cy_bus_width(qspi_command->instruction.bus_width);
    cy_cmd_config->addrWidth = get_cy_bus_width(qspi_command->address.bus_width);
    cy_cmd_config->mode = qspi_command->alt.value;
    cy_cmd_config->modeWidth = get_cy_bus_width(qspi_command->alt.bus_width);
    cy_cmd_config->dummyCycles = qspi_command->dummy_count;
    cy_cmd_config->dataWidth = get_cy_bus_width(qspi_command->data.bus_width);
}

static void uint32_to_byte_array(uint32_t value, uint8_t *byteArray, uint32_t startPos, uint32_t size)
{
    do
    {
        size--;
        byteArray[size + startPos] = (uint8_t)(value & 0xFF);
        value >>= 0x08;
    } while (size > 0);
}

/* cy_qspi_size_t to number bytes */
static uint32_t get_size(cy_qspi_size_t hal_size)
{
    uint32_t size;

    switch (hal_size)
    {
        case CY_QSPI_CFG_SIZE_8:
            size = 1;
            break;

        case CY_QSPI_CFG_SIZE_16:
            size = 2;
            break;

        case CY_QSPI_CFG_SIZE_24:
            size = 3;
            break;

        case CY_QSPI_CFG_SIZE_32:
            size = 4;
            break;

        default:
            size = 0;
    }

    return size;
}

/* Sends QSPI command with certain set of data */
/* Address passed through 'command' is not used, instead the value in 'addr' is used. */
static cy_rslt_t qspi_command_transfer(cy_qspi_t *obj, const cy_qspi_command_t *command, uint32_t addr, bool endOfTransfer)
{
    /* max address size is 4 bytes and max alt size is 4 bytes */
    uint8_t cmd_param[8] = {0};
    uint32_t start_pos = 0;
    uint32_t addr_size = 0;
    uint32_t alt_size = 0;
    cy_en_smif_txfr_width_t bus_width = CY_SMIF_WIDTH_SINGLE;
    cy_stc_smif_mem_cmd_t cy_cmd_config;
    cy_rslt_t result = CY_RSLT_SUCCESS;

    convert_to_cy_cmd_config(command, &cy_cmd_config);

    /*  Does not support different bus_width for address and alt. 
     * bus_width is selected based on what (address or alt) is enabled. 
     * If both are enabled, bus_width of alt is selected
     * It is either possible to support 1 byte alt with different bus_width
     * by sending the alt byte as command as done in Cy_SMIF_Memslot_CmdRead()
     * in cy_smif_memslot.c or support multiple bytes of alt with same bus_width
     * as address by passing the alt bytes as cmd_param to Cy_SMIF_TransmitCommand(). 
     * Second approach is implemented here. This restriction is because of the way
     * PDL API is implemented. 
     */

    if (!command->address.disabled && !command->alt.disabled)
    {
        if (cy_cmd_config.addrWidth != cy_cmd_config.modeWidth)
        {
            result = CY_RSLT_QSPI_BUS_WIDTH_ERROR;
        }
    }

    if (CY_RSLT_SUCCESS != result)
    {
        if (!command->address.disabled)
        {
            addr_size = get_size(command->address.size);
            uint32_to_byte_array(addr, cmd_param, start_pos, addr_size);
            start_pos += addr_size;
            bus_width = cy_cmd_config.addrWidth;
        }

        if (!command->alt.disabled)
        {
            alt_size = get_size(command->alt.size);
            uint32_to_byte_array(cy_cmd_config.mode, cmd_param, start_pos, alt_size);
            bus_width = cy_cmd_config.modeWidth;
        }

        uint32_t cmpltTxfr = ((endOfTransfer) ? 1UL : 0UL);
        result = (cy_rslt_t)Cy_SMIF_TransmitCommand(obj->base, cy_cmd_config.command,
                                                                 cy_cmd_config.cmdWidth, cmd_param, (addr_size + alt_size),
                                                                 bus_width, obj->slave_select, cmpltTxfr, &obj->context);
    }

    return result;
}

/* Checks, that user provided all needed pins and returns max bus width */
static cy_rslt_t check_user_pins(cy_qspi_t * obj, cy_qspi_bus_width_t * max_width)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    /* Single mode */
    if (NC == obj->pin_io0)
    {
        result = CY_RSLT_QSPI_PIN_ERROR;
    }
    else
    {
    	*max_width = CY_QSPI_CFG_BUS_SINGLE;
    }
    /* Dual mode */
    if (NC != obj->pin_io1)
    {
    	*max_width = CY_QSPI_CFG_BUS_DUAL;
    }
    /* Quad Mode */
    if ((NC != obj->pin_io2) && (CY_RSLT_SUCCESS == result))
    {
        result = NC != obj->pin_io1 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
        if (CY_RSLT_SUCCESS == result)
        {
            result = NC != obj->pin_io3 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
            if (CY_RSLT_SUCCESS == result)
            {
            	*max_width = CY_QSPI_CFG_BUS_QUAD;
            }
        }
    }
    /* Octo Mode */
    if ((NC != obj->pin_io4) && (CY_RSLT_SUCCESS == result))
    {
        result = NC != obj->pin_io1 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
        if (CY_RSLT_SUCCESS == result)
        {
            result = NC != obj->pin_io2 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = NC != obj->pin_io3 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = NC != obj->pin_io5 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = NC != obj->pin_io6 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = NC != obj->pin_io7 ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
        }
        if (CY_RSLT_SUCCESS == result)
		{
        	*max_width = CY_QSPI_CFG_BUS_OCTAL;
		}

    }
    if (CY_RSLT_SUCCESS == result)
    {
        result = NC != obj->pin_sclk ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = NC != obj->pin_ssel ? CY_RSLT_SUCCESS : CY_RSLT_QSPI_PIN_ERROR;
    }
    return result;
}

/* Based on ssel pin chosen, determines SMIF slave select parameter and pin mapping */
static const cy_resource_pin_mapping_t * get_slaveselect(cy_gpio_t ssel, cy_en_smif_slave_select_t *slave_select)
{
    bool pin_found = false;
    const cy_resource_pin_mapping_t * pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cy_pin_map_smif_spi_select0);
    if (NULL != pin_mapping)
    {
        pin_found = true;
        *slave_select = CY_SMIF_SLAVE_SELECT_0;
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cy_pin_map_smif_spi_select1);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *slave_select = CY_SMIF_SLAVE_SELECT_1;
        }
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cy_pin_map_smif_spi_select2);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *slave_select = CY_SMIF_SLAVE_SELECT_2;
        }
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cy_pin_map_smif_spi_select3);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *slave_select = CY_SMIF_SLAVE_SELECT_3;
        }  
    }

    return pin_mapping;
}

/* Based on data pins chosen, determines SMIF data select parameter */
static const cy_resource_pin_mapping_t * get_dataselect(cy_gpio_t io0, cy_en_smif_data_select_t *data_select)
{
    bool pin_found = false;
    const cy_resource_pin_mapping_t * pin_mapping = CY_UTILS_GET_RESOURCE(io0, cy_pin_map_smif_spi_data0);
    if (NULL != pin_mapping)
    {
        pin_found = true;
        *data_select = CY_SMIF_DATA_SEL0;
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(io0, cy_pin_map_smif_spi_data2);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL1;
        }
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(io0, cy_pin_map_smif_spi_data4);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL2;
        }
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(io0, cy_pin_map_smif_spi_data6);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL3;
        }
    }
    return pin_mapping;
}

/* Check if obj->base instance is instance of QSPI */
static bool check_instance_is_correct(cy_qspi_t *obj)
{
	bool instance_correct = false;
	uint32_t i;

	for (i=0;i<CY_IP_MXSMIF_INSTANCES;i++)
	{
		if (obj->base == smif_base_addresses[i])
		{
			instance_correct = true;
			break;
		}
	}

	return instance_correct;
}

/* Free all allocated resources */
static cy_rslt_t free_resources(cy_qspi_t *obj)
{
    cy_rslt_t result;
    cy_rslt_t ret_result = CY_RSLT_SUCCESS;

    if (check_instance_is_correct(obj))
    {
    	Cy_SMIF_DeInit(obj->base);
    }
    else
    {
    	ret_result = CY_RSLT_QSPI_INSTANCE_ERROR;
    }

    if (CY_RSC_INVALID != obj->resource.type)
    {
        result = cy_hwmgr_free(&(obj->resource));
        ret_result = CY_RSLT_SUCCESS == result ? ret_result : result;
    }

    result = free_pin_connections(obj);
    ret_result = (CY_RSLT_SUCCESS == result) ? ret_result : result;

    return ret_result;
}

/*******************************************************************************
*       Functions
*******************************************************************************/

cy_rslt_t cy_qspi_init(
    cy_qspi_t *obj, cy_gpio_t io0, cy_gpio_t io1, cy_gpio_t io2, cy_gpio_t io3,
    cy_gpio_t io4, cy_gpio_t io5, cy_gpio_t io6, cy_gpio_t io7, cy_gpio_t sclk,
    cy_gpio_t ssel, uint32_t hz, uint8_t mode)
{
    /* mode (CPOL and CPHA) is not supported in PSoC 6 */
    (void)mode;

    cy_en_smif_slave_select_t slave_select = CY_SMIF_SLAVE_SELECT_0;
    cy_en_smif_data_select_t data_select = CY_SMIF_DATA_SEL0;
    cy_qspi_bus_width_t max_width;

    obj->pin_sclk = sclk;
    obj->pin_ssel = ssel;
    obj->pin_io0 = io0;
    obj->pin_io1 = io1;
    obj->pin_io2 = io2;
    obj->pin_io3 = io3;
    obj->pin_io4 = io4;
    obj->pin_io5 = io5;
    obj->pin_io6 = io6;
    obj->pin_io7 = io7;

    cy_rslt_t result = check_user_pins(obj, &max_width);  

    const cy_resource_pin_mapping_t *ssel_map = NULL;
    const cy_resource_pin_mapping_t *io0_map = NULL;
    const cy_resource_pin_mapping_t *io1_map = NULL;
    const cy_resource_pin_mapping_t *io2_map = NULL;
    const cy_resource_pin_mapping_t *io3_map = NULL;
    const cy_resource_pin_mapping_t *io4_map = NULL;
    const cy_resource_pin_mapping_t *io5_map = NULL;
    const cy_resource_pin_mapping_t *io6_map = NULL;
    const cy_resource_pin_mapping_t *io7_map = NULL;
    const cy_resource_pin_mapping_t *sclk_map = CY_UTILS_GET_RESOURCE(sclk, cy_pin_map_smif_spi_clk);    

    if (NULL == sclk_map)
    {
        result = CY_RSLT_QSPI_PIN_ERROR;
    }
    if (CY_RSLT_SUCCESS == result)
    {
        ssel_map = get_slaveselect(ssel, &slave_select);
        if (ssel_map == NULL)
        {
            result = CY_RSLT_QSPI_PIN_ERROR;
        }
        else
        {
            obj->slave_select = slave_select;
        }
        
    }
    if (CY_RSLT_SUCCESS == result)
    {
        io0_map = get_dataselect(obj->pin_io0, &data_select);
        if (io0_map == NULL)
        {
            result = CY_RSLT_QSPI_PIN_ERROR;
        }
        else
        {
            obj->data_select = data_select;
        }
        
    }
    if (CY_RSLT_SUCCESS == result)
    {
        switch (data_select)
        {
            case CY_SMIF_DATA_SEL0:
                if (max_width >= CY_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_io1, cy_pin_map_smif_spi_data1);
                    if (NULL == io1_map)
                    {
                        result = CY_RSLT_QSPI_PIN_ERROR;
                    }
                }
                if (max_width >= CY_QSPI_CFG_BUS_QUAD)
                {
                    io2_map = CY_UTILS_GET_RESOURCE(obj->pin_io2, cy_pin_map_smif_spi_data2);
                    io3_map = CY_UTILS_GET_RESOURCE(obj->pin_io3, cy_pin_map_smif_spi_data3);
                    if ((NULL == io2_map) || (NULL == io3_map))
                    {
                        result = CY_RSLT_QSPI_PIN_ERROR;
                    }
                }
                if (max_width >= CY_QSPI_CFG_BUS_OCTAL)
                {
                    io4_map = CY_UTILS_GET_RESOURCE(obj->pin_io4, cy_pin_map_smif_spi_data4);
                    io5_map = CY_UTILS_GET_RESOURCE(obj->pin_io5, cy_pin_map_smif_spi_data5);
                    io6_map = CY_UTILS_GET_RESOURCE(obj->pin_io6, cy_pin_map_smif_spi_data6);
                    io7_map = CY_UTILS_GET_RESOURCE(obj->pin_io7, cy_pin_map_smif_spi_data7);
                    if ((NULL == io4_map) || (NULL == io5_map) || (NULL == io6_map) || (NULL == io7_map))
                    {
                        result = CY_RSLT_QSPI_PIN_ERROR;    
                    }
                }
                break;

            case CY_SMIF_DATA_SEL1:
                if (max_width >= CY_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_io1, cy_pin_map_smif_spi_data3);
                    if (NULL == io1_map)
                    {
                        result = CY_RSLT_QSPI_PIN_ERROR;
                    }
                }
                break;

            case CY_SMIF_DATA_SEL2:
                if (max_width >= CY_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_io1, cy_pin_map_smif_spi_data5);
                    if (NULL == io1_map)
                    {
                        result = CY_RSLT_QSPI_PIN_ERROR;    
                    }
                }
                if (max_width >= CY_QSPI_CFG_BUS_QUAD)
                {
                    io2_map = CY_UTILS_GET_RESOURCE(obj->pin_io2, cy_pin_map_smif_spi_data6);
                    io3_map = CY_UTILS_GET_RESOURCE(obj->pin_io3, cy_pin_map_smif_spi_data7);
                    if ((NULL == io2_map) || (NULL == io3_map))
                    {
                        result = CY_RSLT_QSPI_PIN_ERROR;    
                    }
                }
                break;

            case CY_SMIF_DATA_SEL3:
                if (max_width >= CY_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_io1, cy_pin_map_smif_spi_data7);
                    if (NULL == io1_map)
                    {
                        result = CY_RSLT_QSPI_PIN_ERROR;
                    }
                }
                break;

            default:
                result = CY_RSLT_QSPI_DATA_SEL_ERROR;
        }
    }
    /* Check that all pins are belongs to same instance */
    if (CY_RSLT_SUCCESS == result)
    {
        if (sclk_map->inst->block_num != ssel_map->inst->block_num)
        {
            result = CY_RSLT_QSPI_PIN_ERROR;
        }
    }
    if (CY_RSLT_SUCCESS == result)
    {
        if (sclk_map->inst->block_num != io0_map->inst->block_num)
        {
            result = CY_RSLT_QSPI_PIN_ERROR;
        }
        if ((max_width >= CY_QSPI_CFG_BUS_DUAL) && (CY_RSLT_SUCCESS == result))
        {
            if (sclk_map->inst->block_num != io1_map->inst->block_num)
            {
				result = CY_RSLT_QSPI_PIN_ERROR;
			}
        }
    }
    /* Pins io2 and io3 are only available in CY_SMIF_DATA_SEL0 and CY_SMIF_DATA_SEL2 modes */
    if ((CY_RSLT_SUCCESS == result) && ((data_select == CY_SMIF_DATA_SEL0) || (data_select == CY_SMIF_DATA_SEL2)) 
        && (max_width >= CY_QSPI_CFG_BUS_QUAD))
    {
        if ((sclk_map->inst->block_num != io2_map->inst->block_num) ||
            (sclk_map->inst->block_num != io3_map->inst->block_num))
        {
            result = CY_RSLT_QSPI_PIN_ERROR;
        }
    }
    /* Pins io4, io5, io6 and io7 are only available in CY_SMIF_DATA_SEL0 mode */
    if ((CY_RSLT_SUCCESS == result) && (data_select == CY_SMIF_DATA_SEL0) && (max_width >= CY_QSPI_CFG_BUS_OCTAL))
    {
        if ((sclk_map->inst->block_num != io4_map->inst->block_num) ||
            (sclk_map->inst->block_num != io5_map->inst->block_num) ||
            (sclk_map->inst->block_num != io6_map->inst->block_num) ||
            (sclk_map->inst->block_num != io7_map->inst->block_num))
        {
            result = CY_RSLT_QSPI_PIN_ERROR;
        }
    }

    obj->resource = *sclk_map->inst;

    if (CY_RSLT_SUCCESS == result)
    {
        result = make_pin_reservations(obj);
    }

    obj->base = smif_base_addresses[obj->resource.block_num];

    if (CY_RSLT_SUCCESS == result)
    {
        result = cy_connect_pin(sclk_map);
        if (CY_RSLT_SUCCESS == result)
        {
            result = cy_connect_pin(ssel_map);
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = cy_connect_pin(io0_map);
        }
        if ((CY_RSLT_SUCCESS == result) && (max_width >= CY_QSPI_CFG_BUS_DUAL))
        {
            result = cy_connect_pin(io1_map);
        }
    }        
    if ((CY_RSLT_SUCCESS == result) && ((data_select == CY_SMIF_DATA_SEL0) || (data_select == CY_SMIF_DATA_SEL2)) &&
        (max_width >= CY_QSPI_CFG_BUS_QUAD))
    {
        result = cy_connect_pin(io2_map);
        if (CY_RSLT_SUCCESS == result)
        {
            result = cy_connect_pin(io3_map);
        }
    } 
    if ((CY_RSLT_SUCCESS == result) && (data_select == CY_SMIF_DATA_SEL0) && (max_width >= CY_QSPI_CFG_BUS_OCTAL))
    {
        result = cy_connect_pin(io4_map);
        if (CY_RSLT_SUCCESS == result)
        {
            result = cy_connect_pin(io5_map);
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = cy_connect_pin(io6_map);
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = cy_connect_pin(io7_map);
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cy_qspi_frequency(obj, hz);
    }

    bool configured = false;
    if (CY_RSLT_SUCCESS == result)
    {
        result = cy_hwmgr_is_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num, &configured);
    }

    if ((CY_RSLT_SUCCESS == result) && !configured)
    {
        result = (cy_rslt_t)Cy_SMIF_Init(obj->base, &default_qspi_config, TIMEOUT_10_MS, &obj->context);

        Cy_SMIF_SetDataSelect(obj->base, slave_select, data_select);
        Cy_SMIF_Enable(obj->base, &obj->context);
        cy_hwmgr_set_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
    }

    if (CY_RSLT_SUCCESS == result)
    {
    	result = cy_hwmgr_reserve(&(obj->resource));
    }

    if (CY_RSLT_SUCCESS != result)
    {
        free_resources(obj);
    }

    return result;
}

cy_rslt_t cy_qspi_free(cy_qspi_t *obj)
{
    cy_rslt_t result;
    cy_rslt_t ret_result;

    if (NULL != obj)
    {
    	ret_result = free_resources(obj);
    	result = cy_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
		ret_result = (CY_RSLT_SUCCESS == result) ? ret_result : result;
    }
    else
    {
    	ret_result = CY_RSLT_QSPI_INSTANCE_ERROR;
    }

    return ret_result;
}

cy_rslt_t cy_qspi_frequency(cy_qspi_t *obj, uint32_t hz)
{
    /* TODO after HAL clock management implemented JIRA: BSP-510 */
    (void) obj;
    (void) hz;
    return CY_RSLT_SUCCESS;
}

/* no restriction on the value of length. This function splits the read into multiple chunked transfers. */
cy_rslt_t cy_qspi_read(cy_qspi_t *obj, const cy_qspi_command_t *command, void *data, size_t *length)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint32_t chunk = 0;
    size_t read_bytes = *length;
    uint32_t addr = command->address.value;

    /* SMIF can read only up to 65536 bytes in one go. Split the larger read into multiple chunks */
    while (read_bytes > 0)
    {
        chunk = (read_bytes > SMIF_MAX_RX_COUNT) ? (SMIF_MAX_RX_COUNT) : read_bytes;

        /* Address is passed outside command during a read of more than 65536 bytes. Since that
         * read has to be split into multiple chunks, the address value needs to be incremented
         * after every chunk has been read. This requires either modifying the address stored in
         * 'command' passed to read() which is not possible since command is a const pointer or 
         * to create a copy of the command object. Instead of copying the object, the address is
         * passed separately. 
         */
        status = qspi_command_transfer(obj, command, addr, false);

        if (CY_RSLT_SUCCESS == status)
        {
            if (command->dummy_count > 0u)
            {
                status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
            }

            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_ReceiveDataBlocking(obj->base, (uint8_t *)data, chunk,
                                                                            get_cy_bus_width(command->data.bus_width), 
                                                                            &obj->context);
                if (CY_RSLT_SUCCESS != status)
                {
                    break;
                }
            }
        }
        read_bytes -= chunk;
        addr += chunk;
        data = (uint8_t *)data + chunk;
    }

    return status;
}

/* length can be up to 65536. */
cy_rslt_t cy_qspi_write(cy_qspi_t *obj, const cy_qspi_command_t *command, const void *data, size_t *length)
{
    cy_rslt_t status = qspi_command_transfer(obj, command, command->address.value, false);

    if (CY_RSLT_SUCCESS == status)
    {
        if (command->dummy_count > 0u)
        {
            status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
        }

        if (CY_SMIF_SUCCESS == status)
        {
            status = (cy_rslt_t)Cy_SMIF_TransmitDataBlocking(obj->base, (uint8_t *)data, *length,
                                                      get_cy_bus_width(command->data.bus_width), &obj->context);
        }
    }

    return status;
}

cy_rslt_t cy_qspi_transfer(
    cy_qspi_t *obj, const cy_qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0))
    {
        /* only command, no rx or tx */
        status = qspi_command_transfer(obj, command, command->address.value, true);
    }
    else
    {
        if (tx_data != NULL && tx_size)
        {
            status = cy_qspi_write(obj, command, tx_data, &tx_size);
        }

        if (status == CY_RSLT_SUCCESS)
        {
            if (rx_data != NULL && rx_size)
            {
                status = cy_qspi_read(obj, command, rx_data, &rx_size);
            }
        }
    }
    return status;
}

cy_rslt_t cy_qspi_register_irq(cy_qspi_t *obj, cy_qspi_irq_handler_t handler, void *handler_arg)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    uint8_t idx = obj->resource.block_num;
    irq_data_stc[idx].obj = obj;
    irq_data_stc[idx].handler = handler;
    irq_data_stc[idx].irq_arg = handler_arg;

    if (NVIC_GetEnableIRQ(CY_QSPI_IRQ_N[idx]) == 0)
    {
        /* default interrupt priority of 7 (lowest possible priority). */
        cy_stc_sysint_t irqCfg = {
            .intrSrc = CY_QSPI_IRQ_N[idx],
            .intrPriority = 7u};
        Cy_SMIF_SetInterruptMask(
            irq_data_stc[idx].obj->base, cy_convert_interrupt_to_pdl(irq_data_stc[idx].obj->irq_cause));
        Cy_SysInt_Init(&irqCfg, cy_qspi_intr_dispatcher_table[idx]);
        NVIC_EnableIRQ(CY_QSPI_IRQ_N[idx]);
    }

    return result;
}

cy_rslt_t cy_qspi_irq_enable(cy_qspi_t *obj, cy_qspi_irq_event_t event, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */
