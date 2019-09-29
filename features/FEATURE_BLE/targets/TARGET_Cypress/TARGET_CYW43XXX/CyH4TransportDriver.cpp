/* mbed Microcontroller Library
 * $ Copyright Cypress Semiconductor Apache2 $
 *
 */

#if DEVICE_SERIAL && DEVICE_SERIAL_FC

#include "CyH4TransportDriver.h"
#include "cycfg_pins.h"
#include "mbed_power_mgmt.h"
extern "C" void hci_cy_TrSerialRxIncoming(uint8_t *pBuf, uint8_t len);

namespace ble {
namespace vendor {
namespace cypress_ble {

CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name) :
    uart(tx, rx, baud), cts(cts), rts(rts),
    bt_host_wake_name(bt_host_wake_name),
    bt_device_wake_name(bt_device_wake_name),
    bt_host_wake(bt_host_wake_name, PIN_INPUT, PullNone, 0),
    bt_device_wake(bt_device_wake_name, PIN_OUTPUT, PullDefault, 1)
{
    bt_host_wake_active = false;
}

void CyH4TransportDriver::bt_host_wake_rise_irq_handler(void)
{
    if(bt_host_wake_active == true)
    {
        /* lock PSoC 6 DeepSleep entry as long as host_wake is asserted */
        sleep_manager_unlock_deep_sleep();
        bt_host_wake_active = false;
    }
}

void CyH4TransportDriver::bt_host_wake_fall_irq_handler(void)
{
    /* lock PSoC 6 DeepSleep entry as long as host_wake is asserted */
    sleep_manager_lock_deep_sleep();
    bt_host_wake_active = true;
}

void CyH4TransportDriver::initialize()
{
	InterruptIn *host_wake_pin;

    sleep_manager_lock_deep_sleep();

    uart.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    uart.set_flow_control(
        /* flow */ SerialBase::RTSCTS,
        /* rts */ rts,
        /* cts */ cts
    );

    uart.attach(
        callback(this, &CyH4TransportDriver::on_controller_irq),
        SerialBase::RxIrq
    );

    /* Deep sleep unlock to revert the deepsleep lock taken by HCI UART on attaching the RxIrq */
    sleep_manager_unlock_deep_sleep();

    //Register IRQ for Host WAKE
    host_wake_pin = new InterruptIn(bt_host_wake_name);
    host_wake_pin->fall(callback(this, &CyH4TransportDriver::bt_host_wake_fall_irq_handler));
    host_wake_pin->rise(callback(this, &CyH4TransportDriver::bt_host_wake_rise_irq_handler));

    bt_device_wake = 0;
    sleep_manager_unlock_deep_sleep();
}

void CyH4TransportDriver::terminate() {  }

uint16_t CyH4TransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;

    sleep_manager_lock_deep_sleep();
    assert_bt_dev_wake();

    while (i < len + 1) {
        uint8_t to_write = i == 0 ? type : pData[i - 1];
        while (uart.writeable() == 0);
        uart.putc(to_write);
        ++i;
    }

    deassert_bt_dev_wake();
    sleep_manager_unlock_deep_sleep();
    return len;
}

void CyH4TransportDriver::on_controller_irq()
{
    sleep_manager_lock_deep_sleep();

	while (uart.readable()) {
        uint8_t char_received = uart.getc();
        hci_cy_TrSerialRxIncoming(&char_received, 1);
    }

    sleep_manager_unlock_deep_sleep();
}

void CyH4TransportDriver::assert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    bt_device_wake = 0;
#endif
}

void CyH4TransportDriver::deassert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)

    //wait for UART HW FIFO's to be flushed
    while(serial_is_bt_tx_ongoing() == true) {};

    //de-assert bt_device_wake
	bt_device_wake = 1;
#endif
}

void CyH4TransportDriver::update_uart_baud_rate(int baud)
{
	uart.baud(baud);
}

} // namespace cypress_ble
} // namespace vendor
} // namespace ble

#endif
