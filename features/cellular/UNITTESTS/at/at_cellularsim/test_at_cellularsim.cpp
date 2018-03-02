/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularsim.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularSIM.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "ATHandler_stub.h"

using namespace mbed;
using namespace events;

uint32_t mbed::cellular_log_time = 0;

Test_AT_CellularSIM::Test_AT_CellularSIM()
{
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = 0;
}

Test_AT_CellularSIM::~Test_AT_CellularSIM()
{
}

void Test_AT_CellularSIM::test_AT_CellularSIM_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSIM *sim = new AT_CellularSIM(at);

    delete sim;
}

void Test_AT_CellularSIM::test_AT_CellularSIM_set_pin()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin("12"));

    char table2[] = "READY";
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = table2;
    ATHandler_stub::ssize_value = 5;
    CHECK(NSAPI_ERROR_OK == sim.set_pin("12"));
}

void Test_AT_CellularSIM::test_AT_CellularSIM_change_pin()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.change_pin("12", "34"));
}

void Test_AT_CellularSIM::test_AT_CellularSIM_set_pin_query()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin_query("12", true));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin_query("12", false));
}

void Test_AT_CellularSIM::test_AT_CellularSIM_get_sim_state()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSIM sim(at);
    CellularSIM::SimState state;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    ATHandler_stub::ssize_value = -1;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStateUnknown == state);

    char table2[] = "READY";
    ATHandler_stub::read_string_value = table2;
    ATHandler_stub::ssize_value = 5;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStateReady == state);

    char table3[] = "SIM PIN";
    ATHandler_stub::read_string_value = table3;
    ATHandler_stub::ssize_value = 7;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStatePinNeeded == state);

    char table4[] = "SIM PUK";
    ATHandler_stub::read_string_value = table4;
    ATHandler_stub::ssize_value = 7;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStatePukNeeded == state);

    char table5[] = "SOME CRAP";
    ATHandler_stub::read_string_value = table5;
    ATHandler_stub::ssize_value = 9;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStateUnknown == state);
}
