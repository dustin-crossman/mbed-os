/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "cmsis.h"
#include <stdlib.h>

#include "mpu_api.h"
#include "mpu_test.h"

#if !DEVICE_MPU
#error [NOT_SUPPORTED] MPU API not supported for this target
#endif

using namespace utest::v1;

#define HARDFAULT_IRQn   ((IRQn_Type)-13)

// Assembly return instruction: bx lr
#define ASM_BX_LR 0x4770

volatile uint32_t fault_count;
uint32_t real_hard_fault_handler;

static volatile uint16_t data_function = ASM_BX_LR;
static volatile uint16_t bss_function;

static void call_mem(const volatile uint16_t *mem_function)
{
    // or the address with 1 to ensure the thumb bit is set
    ((void (*)()) ((uint32_t)mem_function | 1))();
}

static void hard_fault_handler_test()
{
    fault_count++;
    mbed_mpu_enable_ram_xn(false);
}

static void mpu_fault_test(const volatile uint16_t *mem_function)
{
    mbed_mpu_init();

    // Verify that the mpu causes faults when executing ram
    fault_count = 0;
    mbed_mpu_enable_ram_xn(true);
    call_mem(mem_function);
    TEST_ASSERT_EQUAL(1, fault_count);

    // Verify that the mpu can be turned off
    fault_count = 0;
    mbed_mpu_enable_ram_xn(false);
    call_mem(mem_function);
    TEST_ASSERT_EQUAL(0, fault_count);

    // Verify that the mpu causes faults when executing ram
    fault_count = 0;
    mbed_mpu_enable_ram_xn(true);
    call_mem(mem_function);
    TEST_ASSERT_EQUAL(1, fault_count);

    // Verify that free turns off the mpu
    fault_count = 0;
    mbed_mpu_free();
    call_mem(mem_function);
    TEST_ASSERT_EQUAL(0, fault_count);
}

void mpu_init_test()
{
    for (int i = 0; i < 10; i++) {
        mbed_mpu_init();
    }

    mbed_mpu_free();
}

void mpu_free_test()
{
    mbed_mpu_init();

    // Enable the MPU
    mbed_mpu_enable_ram_xn(true);

    // Free and ensure execution from RAM is allowed
    mbed_mpu_free();

    call_mem(&data_function);
}

void mpu_fault_test_data()
{
    mpu_fault_test(&data_function);
}

void mpu_fault_test_bss()
{
    bss_function = ASM_BX_LR;
    mpu_fault_test(&bss_function);
}

void mpu_fault_test_stack()
{
    uint16_t stack_function;

    stack_function = ASM_BX_LR;
    mpu_fault_test(&stack_function);
}

void mpu_fault_test_heap()
{
    uint16_t *heap_function = (uint16_t*)malloc(2);

    TEST_ASSERT_NOT_EQUAL(NULL, heap_function);
    *heap_function = ASM_BX_LR;
    mpu_fault_test(heap_function);

    free(heap_function);
}

utest::v1::status_t fault_override_setup(const Case *const source, const size_t index_of_case)
{
    // Save old hard fault handler and replace it with a new one
    real_hard_fault_handler = NVIC_GetVector(HARDFAULT_IRQn);
    NVIC_SetVector(HARDFAULT_IRQn, (uint32_t)&hard_fault_handler_test);

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t fault_override_teardown(const Case *const source, const size_t passed, const size_t failed,
                                       const failure_t reason)
{
    // Restore real hard fault handler
    NVIC_SetVector(HARDFAULT_IRQn, real_hard_fault_handler);

    return greentea_case_teardown_handler(source, passed, failed, reason);
}

Case cases[] = {
    Case("MPU - init", fault_override_setup, mpu_init_test, fault_override_teardown),
    Case("MPU - free", fault_override_setup, mpu_free_test, fault_override_teardown),
    Case("MPU - data fault", fault_override_setup, mpu_fault_test_data, fault_override_teardown),
    Case("MPU - bss fault", fault_override_setup, mpu_fault_test_bss, fault_override_teardown),
    Case("MPU - stack fault", fault_override_setup, mpu_fault_test_stack, fault_override_teardown),
    Case("MPU - heap fault", fault_override_setup, mpu_fault_test_heap, fault_override_teardown)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
