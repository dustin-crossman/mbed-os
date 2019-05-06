/* mbed Microcontroller Library
 *
 * $ Copyright Cypress Semiconductor Apache2 $
 *
 */
#include <mbed.h>
#include <string.h>
#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_buf.h"
#include "wsf_cs.h"
#include "hci_api.h"
#include "CyVscCmd.h"

uint16_t vsc_event_mask = 0;
wsfQueue_t      vscCmdQueue;
wsfQueue_t      vscRspQueue;
static volatile int number_of_msg = 0;
static int cy_vsc_init = 0;

typedef struct {
    void        *pNext;
    uint16_t opcode;
    cy_vsc_callback_t *vsc_cb;
} cy_vendor_specific_cmd;

typedef struct {
    void        *pNext;
    uint16_t    opcode;
    uint8_t     status;
    uint8_t     param_len;
    uint8_t     *param;
} cy_vendor_specific_rsp;

Thread threadVsc(osPriorityNormal, OS_STACK_SIZE, NULL, "VSC_CMD");

static cy_vendor_specific_cmd *cy_find_vsc_form_opcode (uint16_t opcode)
{
    if(!(WsfQueueEmpty(&vscCmdQueue)))
    {
        cy_vendor_specific_cmd *cy_cmd = (cy_vendor_specific_cmd *)WsfQueueDeq(&vscCmdQueue);
        if( cy_cmd != NULL )
        {
            if(opcode == cy_cmd->opcode)
            {
                return cy_cmd;
            }
        }
    }
    return NULL;
}

void vscThread(void)
{
    while(1)
    {
        if(number_of_msg)
        {
            WSF_CS_INIT();
            WSF_CS_ENTER();
            number_of_msg--;
            WSF_CS_EXIT();
            cy_vendor_specific_rsp *cy_rsp = (cy_vendor_specific_rsp *)WsfQueueDeq(&vscRspQueue);
            if( cy_rsp != NULL )
            {
                cy_vendor_specific_cmd *cy_cmd = cy_find_vsc_form_opcode(cy_rsp->opcode);
                if (cy_cmd != NULL)
                {
                    cy_rsp->opcode &= (~(0x3F << 10));
                    (*(cy_cmd->vsc_cb))(cy_rsp->opcode, cy_rsp->status, cy_rsp->param_len, cy_rsp->param);
                    WsfBufFree(cy_cmd);
                }
                WsfBufFree(cy_rsp);
            }
        }
    }
}

bool cy_send_vendor_specific_cmd(uint16_t opcode, uint8_t param_len, uint8_t *param, cy_vsc_callback_t *vsc_cb)
{
    if (cy_vsc_init == 0)
    {
        cy_vsc_init = 1;
        // Create Thread to send the callbacks
        threadVsc.start(vscThread);

        //Create Queue to store callbacks
        WSF_QUEUE_INIT(&vscCmdQueue);
        WSF_QUEUE_INIT(&vscRspQueue);
    }
    opcode |= (0x3F << 10);

    cy_vendor_specific_cmd *vsc_cmd = (cy_vendor_specific_cmd *)WsfBufAlloc(sizeof(cy_vendor_specific_cmd));
    if (vsc_cmd != NULL )
    {
        vsc_cmd->opcode = opcode;
        vsc_cmd->vsc_cb = vsc_cb;
        WsfQueueEnq(&vscCmdQueue, vsc_cmd);
        HciVendorSpecificCmd(opcode, param_len, param);
        return true;
    }
    return false;
}

extern "C" {
void cy_post_vsc_complete_event(uint16_t opcode, uint8_t status, uint16_t data_len, uint8_t* data)
{
    if(cy_vsc_init)
    {
        cy_vendor_specific_rsp *cmd_rsp = (cy_vendor_specific_rsp *)WsfBufAlloc(sizeof(cy_vendor_specific_rsp) + data_len);
        if(cmd_rsp != NULL)
        {

            cmd_rsp->opcode = opcode;
            cmd_rsp->status = status;
            cmd_rsp->param_len = data_len;
            cmd_rsp->param  = (uint8_t *)(cmd_rsp + 1);

            memcpy((void *)cmd_rsp->param, data, data_len);
            WsfQueueEnq(&vscRspQueue, cmd_rsp);

            WSF_CS_INIT();
            WSF_CS_ENTER();
            number_of_msg++;
            WSF_CS_EXIT();
        }
    }
}
}