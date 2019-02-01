#ifndef MBED_CAPSENSE_H
#define MBED_CAPSENSE_H

#if defined(DEVICE_CAPSENSE)

#include "platform/platform.h"
#include "capsense_api.h"
#include "platform/mbed_critical.h"

namespace mbed {

class CapSense{
    
    public:
        void Init(void);
        void Enable(void);
        unsigned int IsBusy(void);
        void ProcessAllWidgets(void);
        void ScanAllwidgets(void);
        void RegisterEosCallback(cy_capsense_callback_t callbackFunction);
        unsigned int IsSensorActive(unsigned int wdgyIndex, unsigned int snsIndex);

        CapSense(capsense_t *obj);
        ~CapSense();
    
    private:
        capsense_t *ptrObj;
  
};

}

#endif /* DEVICE_CAPSENSE */

#endif
