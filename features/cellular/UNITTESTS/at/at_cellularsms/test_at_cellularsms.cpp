/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularsms.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "ATHandler_stub.h"
#include "AT_CellularSMS.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;

uint32_t mbed::cellular_log_time = 0;

Test_AT_CellularSMS::Test_AT_CellularSMS()
{
    ATHandler_stub::return_given_size = false;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
}

Test_AT_CellularSMS::~Test_AT_CellularSMS()
{
}

void Test_AT_CellularSMS::test_AT_CellularSMS_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS *sms = new AT_CellularSMS(at);

    delete sms;
}

void Test_AT_CellularSMS::test_AT_CellularSMS_initialize()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.initialize(CellularSMS::CellularSMSMmodeText));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_send_sms()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    CHECK(NSAPI_ERROR_PARAMETER == sms.send_sms(NULL, "2", 1));

    sms.initialize(CellularSMS::CellularSMSMmodeText);
    ATHandler_stub::size_value = 1;
    CHECK(1 == sms.send_sms("1", "22", 2));

    ATHandler_stub::size_value = 2;
    CHECK(2 == sms.send_sms("1", "22", 2));

    sms.initialize(CellularSMS::CellularSMSMmodePDU);
    CHECK(2 == sms.send_sms("1", "23", 2));

    ATHandler_stub::nsapi_error_ok_counter = 1;
    ATHandler_stub::size_value = 32;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.send_sms("1", "23232323", 8));

    ATHandler_stub::nsapi_error_ok_counter = 2;
    ATHandler_stub::size_value = 32;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.send_sms("1", "23232323", 8));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    char table[] = "232323232323232323232323232323232323232323232323232323\
                    232323232323232323232323232323232323232323232323232323\
                    232323232323232323232323232323232323232323232323232323\
                    23232323232323232323232323232323232323\0";
    ATHandler_stub::size_value = 0;
    ATHandler_stub::return_given_size = true;
    CHECK(218 == sms.send_sms("1", table, strlen(table)));

    CHECK(218 == sms.send_sms("12", table, strlen(table)));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_get_sms()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    char buf[16];
    char phone[21];
    char stamp[21];
    int size;
    CHECK(NSAPI_ERROR_PARAMETER == sms.get_sms(NULL, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::int_value = 0;
    CHECK(-1 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_value = 11;
    CHECK(0 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));
    //TODO: Should make add_info to happen, before calling get_sms!

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_value = 11;
    sms.initialize(CellularSMS::CellularSMSMmodePDU);
    CHECK(0 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_sms_callback()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    sms.set_sms_callback(NULL);
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_cpms()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.set_cpms("2", "3", "4"));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_csca()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.set_csca("2", 1));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_cscs()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.set_cscs("2"));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_delete_all_messages()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.delete_all_messages());
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_extra_sim_wait_time()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS sms(at);
    sms.set_extra_sim_wait_time(56);

}

