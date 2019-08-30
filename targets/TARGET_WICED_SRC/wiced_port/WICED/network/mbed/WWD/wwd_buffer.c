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
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "wwd_buffer.h"
#include "wwd_constants.h"
#include "wwd_network_constants.h"
#include "wwd_assert.h"
#include "network/wwd_buffer_interface.h"
#include "mbed_critical.h"
#include "pbuf.h"
#define SDIO_BLOCK_SIZE 64u

wwd_result_t host_buffer_check_leaked(void)
{
    /* Nothing to do */
    return WWD_SUCCESS;
}

wwd_result_t internal_host_buffer_get(wiced_buffer_t * buffer, wwd_buffer_dir_t direction, unsigned short size, unsigned long timeout_ms)
{
    assert(core_util_are_interrupts_enabled());
    UNUSED_PARAMETER( direction );
    struct pbuf *p = NULL;

    if ( ( direction == WWD_NETWORK_TX) && ( size <= PBUF_POOL_BUFSIZE ) )
    {
       p = pbuf_alloc(PBUF_RAW, size, PBUF_POOL);
    }
    else
    {
       p = pbuf_alloc(PBUF_RAW, size+SDIO_BLOCK_SIZE, PBUF_RAM);
       if ( p != NULL )
       {
           p->len = size;
           p->tot_len -=  SDIO_BLOCK_SIZE;
       }
    }
    if (p != NULL )
    {
       *buffer = p;
       return WWD_SUCCESS;
    }
    else
    {
       return WWD_BUFFER_ALLOC_FAIL;
    }
}

wwd_result_t host_buffer_get(/*@out@*/ wiced_buffer_t * buffer, wwd_buffer_dir_t direction, unsigned short size, wiced_bool_t wait)
{
    return internal_host_buffer_get(buffer, direction, size, wait);
}

void host_buffer_release(wiced_buffer_t buffer, wwd_buffer_dir_t direction )
{
    assert(buffer != NULL);
    assert(core_util_are_interrupts_enabled());
    (void) pbuf_free( (struct pbuf *)buffer );
}

uint8_t* host_buffer_get_current_piece_data_pointer(wiced_buffer_t buffer )
{
	struct pbuf *pbuffer= (struct pbuf*) buffer;
	return (uint8_t*) pbuffer->payload;
}

uint16_t host_buffer_get_current_piece_size(wiced_buffer_t buffer)
{
    UNUSED_PARAMETER(buffer);
    struct pbuf *pbuffer = (struct pbuf*) buffer;
    return (uint16_t) pbuffer->len;
}

wiced_buffer_t host_buffer_get_next_piece(wiced_buffer_t buffer)
{
    UNUSED_PARAMETER(buffer);
    return NULL;
}

wwd_result_t host_buffer_add_remove_at_front(wiced_buffer_t * buffer, int32_t add_remove_amount)
{
    struct pbuf **pbuffer = (struct pbuf**) buffer;
    if ( (u8_t) 0 != pbuf_header( *pbuffer, ( s16_t )( -add_remove_amount ) ) )
	{
       return WWD_PMK_WRONG_LENGTH;
	}
    return WWD_SUCCESS;
}

wwd_result_t host_buffer_set_size(wiced_buffer_t buffer, unsigned short size)
{
   struct pbuf * pbuffer = (struct pbuf *) buffer;
   if ( size > (unsigned short) WICED_LINK_MTU_ALIGNED  + LWIP_MEM_ALIGN_SIZE(LWIP_MEM_ALIGN_SIZE(sizeof(struct pbuf))) + LWIP_MEM_ALIGN_SIZE(size) )
   {
      return WWD_PMK_WRONG_LENGTH;
   }
   pbuffer->tot_len = size;
   pbuffer->len = size;
   return WWD_SUCCESS;
}
