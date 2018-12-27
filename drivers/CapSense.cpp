#include "drivers/CapSense.h"

#if defined(DEVICE_CAPSENSE)

namespace mbed {

void CapSense::Init()
{
    capsense_init(ptrObj);
}  


void CapSense::Enable()
{
    capsense_enable(ptrObj);
}


unsigned int CapSense::IsBusy()
{
    unsigned int retVal;

    retVal = capsense_is_busy(ptrObj);

    return(retVal);
}


void CapSense::ProcessAllWidgets()
{
    capsense_process_all_widgets(ptrObj);
}


void CapSense::ScanAllwidgets()
{
    capsense_scan_all_widgets(ptrObj);
}


unsigned int CapSense::IsSensorActive(unsigned int wdgtIndex, unsigned int snsIndex)
{
    unsigned int retVal;
    
    retVal = capsense_is_sensor_active(wdgtIndex, snsIndex, ptrObj);
    
    return(retVal);
}


void CapSense::RegisterEosCallback(cy_capsense_callback_t callbackFunction)
{
    capsense_register_callback(CY_CAPSENSE_END_OF_SCAN_E, callbackFunction, ptrObj);
}


CapSense::CapSense(capsense_t *obj)
{
    ptrObj = obj;
}


CapSense::~CapSense()
{
    
}

}

#endif /* DEVICE_CAPSENSE */