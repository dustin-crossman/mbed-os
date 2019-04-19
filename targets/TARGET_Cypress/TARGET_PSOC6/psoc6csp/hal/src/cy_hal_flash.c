/***************************************************************************//**
* \file cy_hal_flash.c
*
* Description:
* Provides a high level interface for interacting with the Cypress Flash. This 
* is wrapper around the lower level PDL API.
* 
********************************************************************************
* Copyright (c) 2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include "cy_hal_hwmgr.h"
#include "cy_hal_hw_types.h"
#include "cy_hal_flash.h"
#include "cy_flash.h"


cy_rslt_t cy_flash_init(cy_flash_t *obj)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    bool isConfigured = true;
    cy_resource_inst_t flashRsc=obj->resource;

    obj->flashBaseAddr = CY_FLASH_BASE;
    obj->flashSize = CY_FLASH_SIZE;
    obj->sectorSize = CY_FLASH_SIZEOF_ROW;
    obj->pageSize = CY_FLASH_SIZEOF_ROW;
    obj->eraseVal = 0x0U;
    
    status = cy_hwmgr_is_configured(flashRsc.type, flashRsc.block_num, flashRsc.channel_num, &isConfigured);
    
    if ((status == CY_RSLT_SUCCESS) && (true != isConfigured))
    {
        status = cy_hwmgr_reserve(&flashRsc);
        
        if (status == CY_RSLT_SUCCESS)
        {
            status = cy_hwmgr_set_configured(flashRsc.type, flashRsc.block_num, flashRsc.channel_num);
        }
    }

    return (status);
}

cy_rslt_t cy_flash_free(cy_flash_t *obj)
{
    bool isConfigured = false;
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    cy_resource_inst_t flashRsc=obj->resource;
    
    obj->flashBaseAddr = 0UL;
    obj->flashSize = 0UL;
    obj->sectorSize = 0UL;
    obj->pageSize = 0UL;
    obj->eraseVal = 0U;    
    
    status = cy_hwmgr_is_configured(flashRsc.type, flashRsc.block_num, flashRsc.channel_num, &isConfigured);
    
    if (true == isConfigured)
    {
        {
            status = cy_hwmgr_set_unconfigured(flashRsc.type, flashRsc.block_num, flashRsc.channel_num);
        }
    }

    return (status);
}

cy_rslt_t cy_flash_get_info(const cy_flash_t *obj, cy_flash_info_t *info)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    
    info->start_address = obj->flashBaseAddr;
    info->size = obj->flashSize;
    info->sector_size = obj->sectorSize;
    info->page_size = obj->pageSize;
    info->erase_value = obj->eraseVal;
    
    status = CY_RSLT_SUCCESS;

    return (status);
}

cy_rslt_t cy_flash_read(cy_flash_t *obj, uint32_t address, uint8_t *data, size_t size)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    uint32_t dataRead = size;
    
    if ((address >= obj->flashBaseAddr) && (address <= (obj->flashBaseAddr + obj->flashSize)))
    {
        while (dataRead > 0)
        {
            data[dataRead] = (*((uint8 *) (address + dataRead)));
            dataRead --;
        }
        
        status = CY_RSLT_SUCCESS;
    }
    
    return (status);
}

cy_rslt_t cy_flash_erase(cy_flash_t *obj, uint32_t address)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    
    if ((address >= obj->flashBaseAddr) && (address <= (obj->flashBaseAddr + obj->flashSize)))
    {
        status = Cy_Flash_EraseRow(address);
        Cy_SysLib_ClearFlashCacheAndBuffer();
    }
    
    return (status);
}

cy_rslt_t cy_flash_write(cy_flash_t *obj, uint32_t address, const uint32_t* data)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    
    if (((uint32_t)data) > obj->flashBaseAddr)
    {
        static uint8_t writeBuf[CY_FLASH_SIZEOF_ROW];
        memcpy(writeBuf, (const void *)data, CY_FLASH_SIZEOF_ROW);
        status = Cy_Flash_WriteRow(address, (const uint32_t *) writeBuf);
    }
    else
    {
        status = Cy_Flash_WriteRow(address, data);
    }
    
    return (status);
}

cy_rslt_t cy_flash_program(cy_flash_t *obj, uint32_t address, const uint32_t *data)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    
    if (((uint32_t)data) > obj->flashBaseAddr)
    {
        static uint8_t writeBuf[CY_FLASH_SIZEOF_ROW];
        memcpy(writeBuf, (const void *)data, CY_FLASH_SIZEOF_ROW);
        status = Cy_Flash_ProgramRow(address, (const uint32_t *) writeBuf);
    }
    else
    {
        status = Cy_Flash_ProgramRow(address, data);
    }
    
    Cy_SysLib_ClearFlashCacheAndBuffer();

    return (status);
}

cy_rslt_t cy_flash_start_erase(cy_flash_t *obj, uint32_t address)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    
    if ((address >= obj->flashBaseAddr) && (address <= (obj->flashBaseAddr + obj->flashSize)))
    {
        status = Cy_Flash_StartEraseRow(address);
    }
    
    return (status);
}


cy_rslt_t cy_flash_start_write(cy_flash_t *obj, uint32_t address, const uint32_t* data)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;

    if (((uint32_t)data) > obj->flashBaseAddr)
    {
        static uint8_t writeBuf[CY_FLASH_SIZEOF_ROW];
        memcpy(writeBuf, (const void *)data, CY_FLASH_SIZEOF_ROW);
        status = Cy_Flash_StartWrite(address, (const uint32_t *) writeBuf);
    }
    else
    {
      status = Cy_Flash_StartWrite(address, data);
    }

    return (status);
}

cy_rslt_t cy_flash_start_program(cy_flash_t *obj, uint32_t address, const uint32_t* data)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    
    if (((uint32_t)data) > obj->flashBaseAddr)
    {
        static uint8_t writeBuf[CY_FLASH_SIZEOF_ROW];
        memcpy(writeBuf, (const void *)data, CY_FLASH_SIZEOF_ROW);
        status = Cy_Flash_StartProgram(address, (const uint32_t *) writeBuf);
    }
    else
    {
      status = Cy_Flash_StartProgram(address, data);
    }
    
    return (status);
}

cy_rslt_t cy_flash_is_operation_complete(cy_flash_t *obj, bool *complete)
{
    cy_rslt_t status = CY_RSLT_MODULE_FLASH;
    
    status = Cy_Flash_IsOperationComplete();
    
    if (CY_FLASH_DRV_SUCCESS == status)
    {
        Cy_SysLib_ClearFlashCacheAndBuffer();
        *complete = true;
    }
    else
    {
        *complete = false;
    }

    return (status);
}


#if defined(__cplusplus)
}
#endif /* __cplusplus */
