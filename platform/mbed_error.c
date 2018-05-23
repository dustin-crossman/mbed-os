/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <stdlib.h>
#include <stdarg.h>
#include "device.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "platform/mbed_error_log.h"
#include "platform/mbed_error_report.h"
#include "platform/mbed_interface.h"

#if DEVICE_STDIO_MESSAGES
#include <stdio.h>
#endif

static uint8_t error_in_progress = 0;
static int error_count = 0;
static mbed_error_ctx first_error_ctx = {0};
static mbed_error_ctx last_error_ctx = {0};
static mbed_error_hook_t error_hook = NULL;

//Helper function to halt the system
static void mbed_halt_system(void)
{
    mbed_error_print("\nFATAL ERROR: Halting System...\n", NULL);
    
    //If not in ISR context exit, otherwise spin on WFI
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        for(;;) {
           __WFI();
        }
    } else {
        //exit eventually calls mbed_die
        exit(1);
    }
}

WEAK void error(const char* format, ...) {

    // Prevent recursion if error is called again
    if (error_in_progress) {
        return;
    }
    error_in_progress = 1;

#ifndef NDEBUG
    va_list arg;
    va_start(arg, format);
    mbed_error_vfprintf(format, arg);
    va_end(arg);
#endif
    exit(1);
}

//Set an error status with the error handling system
mbed_error_status_t handle_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number) 
{
    mbed_error_ctx current_error_ctx;
    
    //Error status should always be < 0
    if(error_status >= 0) {
        //This is a weird situation, someone called mbed_error with invalid error code.
        //We will still handle the situation but change the error code to ERROR_INVALID_ARGUMENT, atleast the context will have info on who called it
        error_status = MBED_ERROR_INVALID_ARGUMENT;
    }
    
    //Prevent corruption by holding out other callers
    //and we also need this until we remove the error call completely
    while (error_in_progress == 1);
    
    //Use critsect here, as we don't want inadvertant modification of this global variable
    core_util_critical_section_enter();
    error_in_progress = 1;
    core_util_critical_section_exit();
    
    //Increment error count
    error_count++;
    
    //Clear the context capturing buffer
    memset(&current_error_ctx, sizeof(mbed_error_ctx), 0);
    //Capture error information
    current_error_ctx.error_status = error_status;
    current_error_ctx.error_address = (uint32_t)MBED_CALLER_ADDR();
    current_error_ctx.error_value = error_value;

#ifdef MBED_CONF_ERROR_FILENAME_CAPTURE_ENABLED
    //Capture filename/linenumber if provided
    //Index for tracking error_filename 
    int idx = 0;
    
    if(NULL != filename) {
        while(idx < MBED_CONF_MAX_ERROR_FILENAME_LEN && (filename[idx] != '\0')) {
            current_error_ctx.error_filename[idx] = filename[idx];
            idx++;
        }
        current_error_ctx.error_line_number = line_number;
    }
#endif
    
    //Report the error
    mbed_report_error(&current_error_ctx, (char *)error_msg);
    
    //Capture the fist system error and store it
    if(error_count == 1) { //first error
        memcpy(&first_error_ctx, &current_error_ctx, sizeof(mbed_error_ctx));
    }
    
    //copy this error to last error
    memcpy(&last_error_ctx, &current_error_ctx, sizeof(mbed_error_ctx));
    
#ifndef MBED_CONF_ERROR_LOG_DISABLED    
    //Log the error with error log
    mbed_log_put_error(&current_error_ctx);
#endif    
    
    //Call the error hook if available
    if(error_hook != NULL) {
        error_hook(&last_error_ctx);
    }
    
    error_in_progress = 0;
    
    return MBED_SUCCESS;
}

//Return the first error
mbed_error_status_t get_first_error(void) 
{
    //return the first error recorded
    return first_error_ctx.error_status;
}

//Return the last error
mbed_error_status_t get_last_error(void) 
{
    //return the last error recorded
    return last_error_ctx.error_status;
}

//Gets the current error count
int get_error_count(void) 
{
    //return the current error count
    return error_count;
}

//Sets a fatal error 
mbed_error_status_t mbed_warning(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number) 
{
    return handle_error(error_status, error_msg, error_value, filename, line_number);
}

//Sets a fatal error 
WEAK mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number) 
{
    //set the error reported and then halt the system
    if( MBED_SUCCESS != handle_error(error_status, error_msg, error_value, filename, line_number) )
        return MBED_ERROR_FAILED_OPERATION;
    mbed_halt_system();
    
    return MBED_ERROR_FAILED_OPERATION;
}

//Register an application defined callback with error handling
mbed_error_status_t mbed_set_error_hook(mbed_error_hook_t error_hook_in) 
{
    //register the new hook/callback
    if( error_hook_in != NULL )  {
        error_hook = error_hook_in;
        return MBED_SUCCESS;
    } 
    
    return MBED_ERROR_INVALID_ARGUMENT;
}

//Retrieve the first error context from error log 
mbed_error_status_t mbed_get_first_error_log_info (mbed_error_ctx *error_info) 
{
    memcpy(error_info, &first_error_ctx, sizeof(first_error_ctx));
    return MBED_SUCCESS;
}

//Retrieve the last error context from error log 
mbed_error_status_t mbed_get_last_error_log_info (mbed_error_ctx *error_info) 
{
    memcpy(error_info, &last_error_ctx, sizeof(mbed_error_ctx));
    return MBED_SUCCESS;
}

//Makes an mbed_error_status_t value
mbed_error_status_t mbed_make_error(mbed_error_type_t error_type, mbed_module_type_t entity, mbed_error_code_t error_code) 
{
    switch(error_type)
    {
        case MBED_ERROR_TYPE_POSIX:
            if(error_code >= MBED_POSIX_ERROR_BASE && error_code <= MBED_SYSTEM_ERROR_BASE)
                return -error_code;
            break;
            
        case MBED_ERROR_TYPE_SYSTEM:
            if(error_code >= MBED_SYSTEM_ERROR_BASE && error_code <= MBED_CUSTOM_ERROR_BASE)
                return MAKE_MBED_ERROR(MBED_ERROR_TYPE_SYSTEM, entity, error_code);
            break;
            
        case MBED_ERROR_TYPE_CUSTOM:
            if(error_code >= MBED_CUSTOM_ERROR_BASE)
                return MAKE_MBED_ERROR(MBED_ERROR_TYPE_CUSTOM, entity, error_code);
            break;
            
        default:
            break;
    }
    
    //If we are passed incorrect values return a generic system error
    return MAKE_MBED_ERROR(MBED_ERROR_TYPE_SYSTEM, MBED_MODULE_UNKNOWN, MBED_ERROR_CODE_UNKNOWN);
}

/**
 * Clears all the last error, error count and all entries in the error log.
 * @return                      0 or MBED_SUCCESS on success.
 *
 */
mbed_error_status_t mbed_clear_all_errors(void) 
{
    mbed_error_status_t status = MBED_SUCCESS;
    
    //Make sure we dont multiple clients resetting
    core_util_critical_section_enter();
    //Clear the error and context capturing buffer
    memset(&last_error_ctx, sizeof(mbed_error_ctx), 0);
    //reset error count to 0
    error_count = 0;
#ifndef MBED_CONF_ERROR_LOG_DISABLED    
    status = mbed_log_reset();
#endif
    core_util_critical_section_exit();
    
    return status;
}

#ifndef MBED_CONF_ERROR_LOG_DISABLED
//Retrieve the error context from error log at the specified index
mbed_error_status_t mbed_get_error_log_info (int index, mbed_error_ctx *error_info) 
{
    return mbed_log_get_error(index, error_info);
}

//Retrieve the error log count
int mbed_get_error_log_count(void) 
{
    return mbed_log_get_error_log_count();
}

mbed_error_status_t mbed_save_error_log(const char *path)
{
    mbed_error_status_t ret = MBED_SUCCESS;
    mbed_error_ctx ctx = {0};
    int log_count = mbed_log_get_error_log_count();
    FILE *error_log_file = NULL;
    
    //Ensure path is valid
    if(path==NULL) {
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_ARGUMENT);
        goto exit;
    }
    
    //Open the file for saving the error log info
    if((error_log_file = fopen( path, "w" ) ) == NULL){
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OPEN_FAILED);
        goto exit;
    }
    
    //First store the first and last errors
    if(fprintf(error_log_file, "\nFirst Error: Status:0x%x ThreadId:0x%x Address:0x%x Value:0x%x\n", 
        (unsigned int)first_error_ctx.error_status, 
        (unsigned int)first_error_ctx.thread_id, 
        (unsigned int)first_error_ctx.error_address, 
        (unsigned int)first_error_ctx.error_value) <= 0) {
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED);
        goto exit;
    }
    
    if(fprintf(error_log_file, "\nLast Error: Status:0x%x ThreadId:0x%x Address:0x%x Value:0x%x\n", 
        (unsigned int)last_error_ctx.error_status, 
        (unsigned int)last_error_ctx.thread_id, 
        (unsigned int)last_error_ctx.error_address, 
        (unsigned int)last_error_ctx.error_value) <= 0) {
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED);
        goto exit;
    }
    
    //Update with error log info
    while(--log_count >= 0) {
        mbed_log_get_error(log_count, &ctx);
        //first line of file will be error log count
        if(fprintf(error_log_file, "\n%d: Status:0x%x ThreadId:0x%x Address:0x%x Value:0x%x\n", 
            log_count, 
            (unsigned int)ctx.error_status, 
            (unsigned int)ctx.thread_id, 
            (unsigned int)ctx.error_address, 
            (unsigned int)ctx.error_value) <= 0) {
            ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED);
            goto exit;
        }
    }
    
exit:
    fclose(error_log_file);
        
    return ret;
}


#endif

