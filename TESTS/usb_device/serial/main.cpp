/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#error [NOT_SUPPORTED] USB Device not supported for this target
#endif

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "mbed.h"
#include <stdlib.h>
#include "USBCDC.h"

#define USB_CDC_VID 0x1f00
#define USB_CDC_PID 0x2013

#define MSG_KEY_PORT_OPEN_WAIT "port_open_wait"
#define MSG_KEY_SEND_BYTES_SINGLE "send_single"
#define MSG_KEY_SEND_BYTES_MULTIPLE "send_multiple"
#define MSG_KEY_LOOPBACK "loopback"

#define TX_BUFF_SIZE 32
#define RX_BUFF_SIZE 32
#define CDC_LOOPBACK_REPS 1200
#define USB_RECONNECT_DELAY_MS 200

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

#define EF_SEND (1ul << 0)
EventFlags event_flags;

bool send_testcase_data(const char *msg_key, uint16_t usb_vid, uint16_t usb_pid)
{
    char msg_value[10];
    int str_len = snprintf(msg_value, sizeof msg_value, "%04x,%04x", usb_vid, usb_pid);
    if (str_len != (sizeof msg_value) - 1) {
        utest_printf("Failed to compose a value string to be sent to host.");
        return false;
    }
    greentea_send_kv(msg_key, msg_value);
    return true;
}

/** Test CDC USB reconnect
 *
 * Given the host has successfully opened the port of a USB CDC device
 * When the USB device disconnects and connects again
 * Then the host is able to successfully open the port again
 */
void test_cdc_usb_reconnect()
{
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    // Connect the USB device.
    usb_cdc.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_cdc.configured()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    if (!send_testcase_data(MSG_KEY_PORT_OPEN_WAIT, USB_CDC_VID, USB_CDC_PID)) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    // Wait for the host to open the port.
    usb_cdc.wait_ready();
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_TRUE(usb_cdc.ready());

    // Disconnect the USB device.
    usb_cdc.disconnect();
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    wait_ms(USB_RECONNECT_DELAY_MS);
    // Connect the USB device again.
    usb_cdc.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_cdc.configured()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    if (!send_testcase_data(MSG_KEY_PORT_OPEN_WAIT, USB_CDC_VID, USB_CDC_PID)) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    // Wait for the host to open the port again.
    usb_cdc.wait_ready();
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_TRUE(usb_cdc.ready());

    // Disconnect the USB device again.
    usb_cdc.disconnect();
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());
}

/** Test CDC receive single bytes
 *
 * Given the USB CDC device connected to a host
 * When the host transmits a known sequence one byte at a time
 * Then every byte received by the device matches the sequence
 */
void test_cdc_rx_single_bytes()
{
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    usb_cdc.connect();
    if (send_testcase_data(MSG_KEY_SEND_BYTES_SINGLE, USB_CDC_VID, USB_CDC_PID) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    usb_cdc.wait_ready();
    uint8_t buff = 0x01;
    for (int expected = 0xff; expected >= 0; expected--) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        wait_ms(1);
    }
    usb_cdc.disconnect();
}

void tx_thread_fun(USBCDC *usb_cdc)
{
    uint8_t buff_val = 0;
    uint8_t buff[TX_BUFF_SIZE] = { 0 };
    while (event_flags.get() & EF_SEND) {
        if (!usb_cdc->send(buff, TX_BUFF_SIZE)) {
            wait_ms(1);
            continue;
        }
        buff_val++;
        memset(buff, buff_val, TX_BUFF_SIZE);
    }
}

/** Test CDC receive single bytes concurrently
 *
 * Given the USB CDC device connected to a host
 * When the host transmits a known sequence one byte at a time
 *     and at the same time the device transmits data to host
 * Then every byte received by the device matches the sequence
 */
void test_cdc_rx_single_bytes_concurrent()
{
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    usb_cdc.connect();
    if (send_testcase_data(MSG_KEY_SEND_BYTES_SINGLE, USB_CDC_VID, USB_CDC_PID) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    usb_cdc.wait_ready();
    Thread tx_thread;
    event_flags.set(EF_SEND);
    tx_thread.start(mbed::callback(tx_thread_fun, &usb_cdc));
    uint8_t buff = 0x01;
    for (int expected = 0xff; expected >= 0; expected--) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    event_flags.clear(EF_SEND);
    tx_thread.join();
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        wait_ms(1);
    }
    usb_cdc.disconnect();
}

/** Test CDC receive multiple bytes
 *
 * Given the USB CDC device connected to a host
 * When the host transmits chunks of data following a known sequence
 * Then every chunk received by the device matches the sequence
 */
void test_cdc_rx_multiple_bytes()
{
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    usb_cdc.connect();
    if (send_testcase_data(MSG_KEY_SEND_BYTES_MULTIPLE, USB_CDC_VID, USB_CDC_PID) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    usb_cdc.wait_ready();
    uint8_t buff[RX_BUFF_SIZE] = { 0 };
    uint8_t expected_buff[RX_BUFF_SIZE] = { 0 };
    for (int expected = 0xff; expected >= 0; expected--) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        wait_ms(1);
    }
    usb_cdc.disconnect();
}

/** Test CDC receive multiple bytes concurrently
 *
 * Given the USB CDC device connected to a host
 * When the host transmits chunks of data following a known sequence
 *     and at the same time the device transmits data to host
 * Then every chunk received by the device matches the sequence
 */
void test_cdc_rx_multiple_bytes_concurrent()
{
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    usb_cdc.connect();
    if (send_testcase_data(MSG_KEY_SEND_BYTES_MULTIPLE, USB_CDC_VID, USB_CDC_PID) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    usb_cdc.wait_ready();
    Thread tx_thread;
    event_flags.set(EF_SEND);
    tx_thread.start(mbed::callback(tx_thread_fun, &usb_cdc));
    uint8_t buff[RX_BUFF_SIZE] = { 0 };
    uint8_t expected_buff[RX_BUFF_SIZE] = { 0 };
    for (int expected = 0xff; expected >= 0; expected--) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    event_flags.clear(EF_SEND);
    tx_thread.join();
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        wait_ms(1);
    }
    usb_cdc.disconnect();
}

/** Test CDC loopback
 *
 * Given the USB CDC device connected to a host
 * When the device transmits random bytes to host
 *     and the host transmits them back to the device
 * Then every byte received by the device is equal to byte preciously sent
 */
void test_cdc_loopback()
{
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    usb_cdc.connect();
    if (send_testcase_data(MSG_KEY_LOOPBACK, USB_CDC_VID, USB_CDC_PID) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    usb_cdc.wait_ready();
    uint8_t rx_buff, tx_buff;
    for (int i = 0; i < CDC_LOOPBACK_REPS; i++) {
        tx_buff = (uint8_t)(rand() % 0x100);
        rx_buff = (uint8_t)(tx_buff + 1);
        TEST_ASSERT(usb_cdc.send(&tx_buff, 1));
        TEST_ASSERT(usb_cdc.receive(&rx_buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(tx_buff, rx_buff);
    }
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        wait_ms(1);
    }
    usb_cdc.disconnect();
}

utest::v1::status_t testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(25, "usb_device_serial");
    srand((unsigned) ticker_read_us(get_us_ticker_data()));
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("CDC USB reconnect", test_cdc_usb_reconnect),
    Case("CDC RX single bytes", test_cdc_rx_single_bytes),
    Case("CDC RX single bytes concurrent", test_cdc_rx_single_bytes_concurrent),
    Case("CDC RX multiple bytes", test_cdc_rx_multiple_bytes),
    Case("CDC RX multiple bytes concurrent", test_cdc_rx_multiple_bytes_concurrent),
    Case("CDC loopback", test_cdc_loopback),
};

Specification specification(testsuite_setup, cases);

int main()
{
    return !Harness::run(specification);
}
