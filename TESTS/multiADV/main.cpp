
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble_tests.h"
#include "greentea-client/test_env.h"
#include "utest.h"
#include "rtos.h"
#include "unity.h"
#include "ButtonService.h"
#include "pretty_printer.h"
#include "CyMulti_adv.h"

using namespace utest::v1;


utest::v1::status_t test_setup(const size_t number_of_cases) {
    // Setup Greentea using a reasonable timeout in seconds
    GREENTEA_SETUP(100, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
	
}

Case cases[] = {
	
	Case(" Test Case Name : BLE_MultiADV_Discovery_Test",BLE_MultiADV_Discovery_Test),
	wait_ms(10000);
	Case(" Test Case Name : BLE_MultiADV_SetAdvType_01",BLE_MultiADV_SetAdvType_01),
	Case(" Test Case Name : BLE_MultiADV_SetAdvType_02",BLE_MultiADV_SetAdvType_02),
	Case(" Test Case Name : BLE_MultiADV_SetAdvType_03",BLE_MultiADV_SetAdvType_03),
	Case(" Test Case Name : BLE_MultiADV_SetAdvType_04",BLE_MultiADV_SetAdvType_04),
	Case(" Test Case Name : BLE_MultiADV_SingleInstanceTest",BLE_SingleADVInstance_Test),
	Case(" Test Case Name : BLE_MultiADV_SetDiffADVType_01",BLE_MultiADV_SetDiffADVType_01),
	Case(" Test Case Name : BLE_MultiADV_SetDiffADVType_02",BLE_MultiADV_SetDiffADVType_02),
	Case(" Test Case Name : BLE_MultiADVChannelMap_01",BLE_MultiADVChannelMap_01),
	Case(" Test Case Name : BLE_MultiADVChannelMap_02",BLE_MultiADVChannelMap_02),
	Case(" Test Case Name : BLE_DisableMultiADV_Test",BLE_DisableMultiADV_Test),
	
	
};

Specification specification(test_setup, cases);

int main()
{
	printf("I am in main()");
   return !Harness::run(specification);
}
