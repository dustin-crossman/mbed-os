
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

static char DEVICE_NAME[] = "Button 0";

static EventQueue event_queue(/* event count */ 10 * EVENTS_EVENT_SIZE);

void schedule_ble_events6(BLE::OnEventsToProcessCallbackContext *context);

void multi_adv_cb6(ble::vendor::cypress::hci_multi_adv_sub_opcode_t sub_opcode, uint8_t status);

void multi_adv_cb6(ble::vendor::cypress::hci_multi_adv_sub_opcode_t sub_opcode, uint8_t status)
{
    printf("sub_opcode 0x%02x status %d \n",sub_opcode, status);
}

class BatteryDemo6 : ble::Gap::EventHandler {
public:
    BatteryDemo6(BLE &ble, events::EventQueue &event_queue) :
        _ble(ble),
        _event_queue(event_queue),
        _led1(LED1, 1),
        _button(BLE_BUTTON_PIN_NAME),
        _button_service(NULL),
        _button_uuid(ButtonService::BUTTON_SERVICE_UUID),
        _adv_data_builder(_adv_buffer) { }

    void start() {
        _ble.gap().setEventHandler(this);

        _ble.init(this, &BatteryDemo6::on_init_complete);

        _event_queue.call_every(500, this, &BatteryDemo6::blink);

        _event_queue.dispatch(1000);
    }

private:
    /** Callback triggered when the ble initialization process has finished */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params) {
        if (params->error != BLE_ERROR_NONE) {
            printf("Ble initialization failed.");
            return;
        }

        print_mac_address();

        /* Setup primary service. */

        _button_service = new ButtonService(_ble, false /* initial value for button pressed */);

        _button.fall(Callback<void()>(this, &BatteryDemo6::button_pressed));
        _button.rise(Callback<void()>(this, &BatteryDemo6::button_released));

        start_advertising();
    }

    void start_advertising() {
        /* Create multi adv object */
        ble::vendor::cypress::CyMulti_adv  cy_multi;

        /* Register callback */
        cy_multi.cy_register_multi_adv_cmd_complete_cb(&multi_adv_cb6);

        /* ADV Instance 1 */
        /* Create advertising parameters */
        ble::vendor::cypress::cy_ble_multi_adv_params_t adv_param(ble::advertising_type_t::NON_CONNECTABLE_UNDIRECTED , 
                                            ble::adv_interval_t(281),ble::adv_interval_t(281));
        cy_multi.cy_set_multi_advertisement_params(1, adv_param);

        /* Create advertising payload */
        _adv_data_builder.setFlags();
        _adv_data_builder.setLocalServiceList(mbed::make_Span(&_button_uuid, 1));
        DEVICE_NAME[7] = '5';
        _adv_data_builder.setName(DEVICE_NAME);

        /* Set Multi Adv advertising payload */
         mbed::Span<const uint8_t> adv_data = _adv_data_builder.getAdvertisingData();
        cy_multi.cy_set_multi_advertisement_data((uint8_t *)adv_data.data(), adv_data.size(), 1);

        /*start Multi Adv */
        cy_multi.cy_start_multi_advertisements(1,1);

        /* ADV Instance 2 */
		adv_param._adv_type = ble::advertising_type_t::CONNECTABLE_UNDIRECTED;
        adv_param._adv_int_min = ble::adv_interval_t(311);
        adv_param._adv_int_max = ble::adv_interval_t(311);
        cy_multi.cy_set_multi_advertisement_params(2, adv_param);

        DEVICE_NAME[7] = '6';
        _adv_data_builder.setName(DEVICE_NAME);
        adv_data = _adv_data_builder.getAdvertisingData();
        cy_multi.cy_set_multi_advertisement_data((uint8_t *)adv_data.data(), adv_data.size(), 2);

        /*start Multi Adv */
        cy_multi.cy_start_multi_advertisements(1,2);

        /* ADV Instance 3 */
		adv_param._adv_type = ble::advertising_type_t::SCANNABLE_UNDIRECTED;
        adv_param._adv_int_min = ble::adv_interval_t(397);
        adv_param._adv_int_max = ble::adv_interval_t(397);
        cy_multi.cy_set_multi_advertisement_params(3, adv_param);

        DEVICE_NAME[7] = '7';
        _adv_data_builder.setName(DEVICE_NAME);
        adv_data = _adv_data_builder.getAdvertisingData();
        cy_multi.cy_set_multi_advertisement_data((uint8_t *)adv_data.data(), adv_data.size(), 3);

        /*start Multi Adv */
        cy_multi.cy_start_multi_advertisements(1,3);
    }

    void button_pressed(void) {
        _event_queue.call(Callback<void(bool)>(_button_service, &ButtonService::updateButtonState), true);
    }

    void button_released(void) {
        _event_queue.call(Callback<void(bool)>(_button_service, &ButtonService::updateButtonState), false);
    }

    void blink(void) {
        _led1 = !_led1;
    }

private:
    /* Event handler */

    virtual void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&) {
        _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
    }

private:
    BLE &_ble;
    events::EventQueue &_event_queue;

    DigitalOut  _led1;
    InterruptIn _button;
    ButtonService *_button_service;

    UUID _button_uuid;

    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;
};
void BLE_MultiADV_SetDiffADVType_01()
{
    printf("My Example Application Initialized !\n");
     BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_events6);

    BatteryDemo6 demo6(ble, event_queue);
    demo6.start();
	
	TEST_ASSERT(true);
	 
}
void schedule_ble_events6(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}