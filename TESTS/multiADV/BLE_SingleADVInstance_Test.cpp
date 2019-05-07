
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

void schedule_ble_events4(BLE::OnEventsToProcessCallbackContext *context);

void multi_adv_cb4(ble::vendor::cypress::hci_multi_adv_sub_opcode_t sub_opcode, uint8_t status);

void multi_adv_cb4(ble::vendor::cypress::hci_multi_adv_sub_opcode_t sub_opcode, uint8_t status)
{
    printf("sub_opcode 0x%02x status %d \n",sub_opcode, status);
}

class BatteryDemo4 : ble::Gap::EventHandler {
public:
    BatteryDemo4(BLE &ble, events::EventQueue &event_queue) :
        _ble(ble),
        _event_queue(event_queue),
        _led1(LED1, 1),
        _button(BLE_BUTTON_PIN_NAME),
        _button_service(NULL),
        _button_uuid(ButtonService::BUTTON_SERVICE_UUID),
        _adv_data_builder(_adv_buffer) { }

    void start() {
        _ble.gap().setEventHandler(this);

        _ble.init(this, &BatteryDemo4::on_init_complete);

        _event_queue.call_every(500, this, &BatteryDemo4::blink);

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

        _button.fall(Callback<void()>(this, &BatteryDemo4::button_pressed));
        _button.rise(Callback<void()>(this, &BatteryDemo4::button_released));

        start_advertising();
    }

    void start_advertising() {
        /* Create multi adv object */
        ble::vendor::cypress::CyMulti_adv  cy_multi;

        /* Register callback */
        cy_multi.cy_register_multi_adv_cmd_complete_cb(&multi_adv_cb4);

        /* ADV Instance 1 */
        /* Create advertising parameters */
        ble::vendor::cypress::cy_ble_multi_adv_params_t adv_param(ble::advertising_type_t::CONNECTABLE_UNDIRECTED , 
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
void BLE_SingleADVInstance_Test()
{
    printf("My Example Application Initialized !\n");
     BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_events4);

    BatteryDemo4 demo4(ble, event_queue);
    demo4.start();
	
	TEST_ASSERT(true);
	 
}
void schedule_ble_events4(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}