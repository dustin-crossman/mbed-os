/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include "USBPhyHw.h"

#include "PinNamesTypes.h"
// Convert physical endpoint number to bit array
#define EP_BIT(endpoint) (1UL<<CY_USBFS_DEV_DRV_EPADDR2EP(endpoint))

// Bit array of aborted data endpoints
static uint32_t aborted_ep;


// Callbacks for event processing to tie driver and 
static void usb_dev_bus_reset_callback(void);
static void usb_dev_ep0_setup_callback(void);
static void usb_dev_ep0_in_callback(void);
static void usb_dev_ep0_out_callback(void); 
static void usb_dev_endpoint_callback(cyhal_usb_dev_ep_t endpointAddr);
static void sof_callback(uint32_t frame_number);

static USBPhyHw *instance;

USBPhy *get_usb_phy()
{
    static USBPhyHw usbphy;
    return &usbphy;
}

USBPhyHw::USBPhyHw(): events(NULL)
{

}

USBPhyHw::~USBPhyHw()
{

}


static void usb_dev_endpoint_callback(cyhal_usb_dev_ep_t endpointAddr)
{
    
    if(0U == (aborted_ep & EP_BIT(endpointAddr)))
    {
        if (CY_USBFS_DEV_DRV_IS_EP_DIR_IN(endpointAddr))
        {
            // Execute callback for endpoint IN
            instance->events->in(endpointAddr);
        }
        else
        {
            // Execute callback for endpoint OUT
            instance->events->out(endpointAddr);
        }
    }
}


void USBPhyHw::init(USBPhyEvents *events)
{
    cyhal_usb_dev_t *obj;
    // Initialize instance to access class data
    instance = this;

    // Get access to the usb object
    obj = &(instance->obj);

    // Disable IRQ
    cyhal_usb_dev_irq_enable(obj, false);
    
    // Store events
    this->events = events;

    // TODO -  Setup clocks

    // TODO - Enable USB module

    // Init USB driver
    cyhal_clock_divider_t clkPll = 
    {
        /* .div_type */ CY_SYSCLK_DIV_16_BIT,
        /* .div_num  */ 0U
    }; 
    cyhal_clock_divider_t clkDiv = 
    {
        /* .div_type */ CY_SYSCLK_DIV_16_BIT,
        /* .div_num  */ 0U
    }; ; 
    
    if (CY_RSLT_SUCCESS != cyhal_usb_dev_init(obj, USBDP, USBDM, &clkPll, &clkDiv, NULL))
    {
        //error("usb: driver initialization failed (Cy_USBFS_Dev_Drv_Init).");
    }
    
    // Hook device handlers to be called by driver
    cyhal_usb_dev_register_event_callback(obj, CYHAL_USB_DEV_EVENT_BUS_RESET, &usb_dev_bus_reset_callback);
    cyhal_usb_dev_register_event_callback(obj, CYHAL_USB_DEV_EVENT_EP0_SETUP, &usb_dev_ep0_setup_callback);
    cyhal_usb_dev_register_event_callback(obj, CYHAL_USB_DEV_EVENT_EP0_IN, &usb_dev_ep0_in_callback);
    cyhal_usb_dev_register_event_callback(obj, CYHAL_USB_DEV_EVENT_EP0_OUT, &usb_dev_ep0_out_callback);

    cyhal_usb_dev_register_sof_callback(obj, &sof_callback);

    // Configure interrupt and hook it interrupt handler
    cyhal_usb_dev_register_irq(obj, (cyhal_usb_dev_irq_handler_t)&_usbisr);
    cyhal_usb_dev_irq_enable(obj, true);
}


static void usb_dev_bus_reset_callback(void)
{
    // Process usb bus reset
    instance->events->reset();
}


static void usb_dev_ep0_setup_callback(void)
{
    // Process endpoint 0 SETUP packet (Setup stages)
    instance->events->ep0_setup();
}


static void usb_dev_ep0_in_callback(void)
{
    // Process endpoint 0 IN packet (Data or Status stages)
    instance->events->ep0_in();
}


static void usb_dev_ep0_out_callback(void)
{
    // Process endpoint 0 OUT packet (Data or Status stages)
    instance->events->ep0_out();
}


static void sof_callback(uint32_t frame_number)
{
    // Calls SOF packet callback
    instance->events->sof(frame_number);
}


void USBPhyHw::deinit()
{
    cyhal_usb_dev_t *obj = &(instance->obj);

    // Disconnect and disable interrupt
    cyhal_usb_dev_free(obj);
}

bool USBPhyHw::powered()
{
    // TODO - return true if powered false otherwise. Devices which don't support
    //    this should always return true
    return true;
}

void USBPhyHw::connect()
{
    cyhal_usb_dev_t *obj = &(instance->obj);

    // Enable block for operation: enable pull-up on D+
    cyhal_usb_dev_connect(obj);
}

void USBPhyHw::disconnect()
{
    cyhal_usb_dev_t *obj = &(instance->obj);

    // Disable block for operation: Disable pull-up on D+
    cyhal_usb_dev_disconnect(obj);
}

void USBPhyHw::configure()
{
    cyhal_usb_dev_t *obj = &(instance->obj);
    cyhal_usb_dev_configure(obj);
}

void USBPhyHw::unconfigure()
{
    cyhal_usb_dev_t *obj = &(instance->obj);
    cyhal_usb_dev_unconfigure(obj);
}

void USBPhyHw::sof_enable()
{
    cyhal_usb_dev_t *obj = &(instance->obj);

    cyhal_usb_dev_sof_enable(obj, true);
}

void USBPhyHw::sof_disable()
{
    cyhal_usb_dev_t *obj = &(instance->obj);

    cyhal_usb_dev_sof_enable(obj, false);
}

void USBPhyHw::set_address(uint8_t address)
{   
    // set the device address. Address must take effect
    // after the status phase of the current transfer
  
    cyhal_usb_dev_t *obj = &(instance->obj);
 
    cyhal_usb_dev_set_address(obj, address);
}

void USBPhyHw::remote_wakeup()
{
    // TODO - Sent remote wakeup over USB lines (if supported)
}

#define USB_EP_ATTR_DATA_EP     (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_ALLOW_ISO)
#define USB_EP_ATTR_NON_ISO     (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT )

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t lpc_table = {
        512, 
        // CY USB IP has hardware buffer of 512 bytes that is shared among 8 data endpoint.
        // The buffer has static allocation as follows:
        // - 4 endpoints of 64 that supports BULK and INT.
        // - 2 endpoints of 128 that support ISOC, BULK and INT.
        // The static allocation of max packet for BULK and INT allows to handle change interface 
        // alternates properly if endpoint size is changed (the endpoint is not overlapped with 
        // endpoints of neighborhood interface).
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0}, //corresponds to dedicated endpoint 0 hardware buffer (separate buffer from data endpoint buffer)
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_NON_ISO    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_DATA_EP    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {USB_EP_ATTR_DATA_EP    | USB_EP_ATTR_DIR_IN_OR_OUT,  0, 0},
            {0, 0, 0},
            {0, 0, 0},
        }
    };
    
    return &lpc_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    (void) max_packet;
    cyhal_usb_dev_t *obj = &(instance->obj);
    
    // Endpoint 0 has dedicated hardware 8 bytes buffer.
    return cyhal_usb_dev_ep0_get_max_packet(obj);
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
     cyhal_usb_dev_t *obj = &(instance->obj);
  
    cyhal_usb_dev_ep0_setup_read_result(obj, buffer, size);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    // Setup data buffer to receive next endpoint 0 OUT packet
    cyhal_usb_dev_t *obj = &(instance->obj);
    
    cyhal_usb_dev_ep0_read(obj, data, size);
}

uint32_t USBPhyHw::ep0_read_result()
{
    // Return the size of the last OUT packet received on endpoint 0
    cyhal_usb_dev_t *obj = &(instance->obj);
    
    return cyhal_usb_dev_ep0_read_result(obj);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    //Start transferring buffer on endpoint 0 IN
    cyhal_usb_dev_t *obj = &(instance->obj);

    cyhal_usb_dev_ep0_write(obj, buffer, size);
}

void USBPhyHw::ep0_stall()
{
    cyhal_usb_dev_t *obj = &(instance->obj);

    cyhal_usb_dev_ep0_stall(obj);
}

#define USB_MAX_NON_ISO_SIZE                    (64U)
#define USB_MAX_ISO_SIZE                        (128U)

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    bool status = false;
    cy_stc_usb_dev_ep_config_t epConfig;

    epConfig.enableEndpoint = true;     // MBED adds only active endpoints
    epConfig.allocBuffer = true;        // MBED needs buffer allocation (the buffer size is predefined in advance)
    epConfig.maxPacketSize = (USB_EP_TYPE_ISO == type) ? USB_MAX_ISO_SIZE : USB_MAX_NON_ISO_SIZE;
    epConfig.bufferSize = (uint16_t) max_packet; // Buffer size is predefined
    epConfig.endpointAddr = endpoint;            // Includes direction bit
    epConfig.attributes = type;                  // Type equal to attributes

    cyhal_usb_dev_t *obj = &(instance->obj);
    
    if(CY_USBFS_DEV_DRV_SUCCESS ==  Cy_USBFS_Dev_Drv_AddEndpoint(obj->base, 
                                        &epConfig, &obj->context))
    {
        cyhal_usb_dev_register_endpoint_callback(obj, endpoint, &usb_dev_endpoint_callback);
        status = true;
    }

    return status;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    // disable and remove this endpoint
    cyhal_usb_dev_t *obj = &(instance->obj);
    (void) Cy_USBFS_Dev_Drv_RemoveEndpoint(obj->base, endpoint, &obj->context);
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    // stall this endpoint until it is explicitly cleared (remove direction bit)
    cyhal_usb_dev_t *obj = &(instance->obj);
    (void) Cy_USBFS_Dev_Drv_StallEndpoint(obj->base, CY_USBFS_DEV_DRV_EPADDR2EP(endpoint), &obj->context);
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    // unstall this endpoint (remove direction bit)
    cyhal_usb_dev_t *obj = &(instance->obj);
    (void) Cy_USBFS_Dev_Drv_UnStallEndpoint(obj->base, CY_USBFS_DEV_DRV_EPADDR2EP(endpoint), &obj->context);
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    bool retStatus = false;
    cyhal_usb_dev_t *obj = &(instance->obj);    
    cy_en_usb_dev_ep_state_t epState;
    
    // Remove direction bit 
    uint32_t ep_num = CY_USBFS_DEV_DRV_EPADDR2EP(endpoint);
    
    epState = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &obj->context);
    
    if ((CY_USB_DEV_EP_IDLE == epState) || (CY_USB_DEV_EP_COMPLETED == epState))
    {
        Cy_USBFS_Dev_Drv_EnableOutEndpoint(obj->base, ep_num, &obj->context);
        
        //Save pointer and size
        obj->rd_data[ep_num] = data;
        obj->rd_size[ep_num] = size; 

        retStatus = true;
    }

    return retStatus;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint32_t actSize = 0;    
    cyhal_usb_dev_t *obj = &(instance->obj);
    cy_en_usb_dev_ep_state_t epState;  
  
    // Remove direction bit 
    uint32_t ep_num = CY_USBFS_DEV_DRV_EPADDR2EP(endpoint);

    epState = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &obj->context);

    if (CY_USB_DEV_EP_COMPLETED == epState)
    {
        cy_en_usbfs_dev_drv_status_t drvStatus;
        drvStatus = Cy_USBFS_Dev_Drv_ReadOutEndpoint(obj->base, ep_num, 
                                                     obj->rd_data[ep_num], 
                                                     obj->rd_size[ep_num], 
                                                     &actSize, 
                                                     &obj->context);

        if (CY_USBFS_DEV_DRV_SUCCESS != drvStatus)
        {
            actSize = 0U;
        }
    }

    return actSize;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint32_t retStatus = false;
    cyhal_usb_dev_t *obj = &(instance->obj);
    cy_en_usb_dev_ep_state_t epState;
    
    // Remove direction bit 
    uint32_t ep_num = CY_USBFS_DEV_DRV_EPADDR2EP(endpoint);    
    
    epState = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &obj->context);

    // Check that endpoint is ready for operation 
    if ((CY_USB_DEV_EP_IDLE == epState) || (CY_USB_DEV_EP_COMPLETED == epState))
    {
        cy_en_usbfs_dev_drv_status_t drvStatus;
        drvStatus = Cy_USBFS_Dev_Drv_LoadInEndpoint(obj->base, ep_num, 
                                                    data, 
                                                    size, 
                                                    &obj->context);
                    
        if (CY_USBFS_DEV_DRV_SUCCESS == drvStatus)
        {
            retStatus = true;
        }
    }
    
    return retStatus;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    // stop the current transfer on this endpoint and don't call the IN or OUT callback
    cyhal_usb_dev_t *obj = &(instance->obj);
    cy_en_usb_dev_ep_state_t epState;

    // Remove direction bit 
    uint32_t ep_num = CY_USBFS_DEV_DRV_EPADDR2EP(endpoint); 

    //Set flag to avoid generation of event
    aborted_ep |= EP_BIT(endpoint);

    // Request abort operation
    epState = Cy_USBFS_Dev_Drv_Abort(obj->base, ep_num, &obj->context);

    if (CY_USB_DEV_EP_PENDING == epState)
    {
        // Wait 1 ms to complete possible on-going transfer
        //wait_ms(1); Need to remove wait per ARM request

        epState = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &obj->context);
        if (CY_USB_DEV_EP_PENDING == epState)
        {
            (void) Cy_USBFS_Dev_Drv_AbortComplete(obj->base, ep_num, &obj->context);
        }
    }

    // Abort completed release flag
    aborted_ep &= ~EP_BIT(endpoint);
}

void USBPhyHw::process()
{
    cyhal_usb_dev_t *obj = &(instance->obj);

    // Handle interrupt 
    cyhal_usb_dev_process_irq(obj);

    // Re-enable interrupt
    cyhal_usb_dev_irq_enable(obj, true);
}

void USBPhyHw::_usbisr(void) 
{
    
    cyhal_usb_dev_t *obj = &(instance->obj);
    cyhal_usb_dev_irq_enable(obj, false);
    
    instance->events->start_process();
}
