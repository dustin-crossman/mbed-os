/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#include "flash_api.h"
#include "flash_data.h"
#include "mbed_critical.h"

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
// NOTE: On ARMv7-M/ARMv8-M, instruction fetches are always little-endian.
static uint32_t FLASH_ALGO[] = {
    0x4603b530, 0x2164460c, 0x4dec2059, 0x20166028, 0xf8c5070d, 0x20880100, 0x0100f8c5, 0xf8d006c0, 
    0xf0000100, 0xb9080001, 0xbd302001, 0x680048e4, 0x0004f040, 0x4580f04f, 0x0200f8c5, 0xf8d04628, 
    0xf0400204, 0xf8c50004, 0xbf000204, 0xf1a11e08, 0xd1fb0101, 0x680048db, 0x0021f040, 0x60284dd9, 
    0x69c04628, 0x0001f040, 0x462861e8, 0xf0006800, 0xb9080001, 0xe7d82001, 0x30fff04f, 0x64684dd1, 
    0x6c404628, 0xb1081c40, 0xe7ce2001, 0x680048cd, 0x0040f040, 0x60284dcb, 0xe7c62000, 0xbf004601, 
    0x690048c8, 0x0001f000, 0xd1f92800, 0x680048c5, 0x0021f020, 0x60104ac3, 0x69c04610, 0x0001f020, 
    0x200061d0, 0x60104abd, 0x46014770, 0x48bdbf00, 0xf0006900, 0x28000001, 0x48bad1f9, 0xf0406800, 
    0x4ab80040, 0x20226010, 0xf02160d0, 0x60500003, 0x61102001, 0x8f60f3bf, 0x48b2bf00, 0xf0006900, 
    0x28000001, 0x48afd1f9, 0xf0006800, 0xb1380040, 0x680048ac, 0x0040f040, 0x60104aaa, 0x47702001, 
    0xe7fc2000, 0x4603b570, 0x2500460c, 0x4629e009, 0xf8531c6d, 0xf7ff0021, 0x1e06ffc8, 0x4630d001, 
    0x42a5bd70, 0x2000d3f3, 0xb570e7fa, 0x460b4604, 0x22004615, 0xf1034629, 0xf020000f, 0xbf00030f, 
    0x69004898, 0x0001f000, 0xd1f92800, 0x68004895, 0x0040f040, 0x60304e93, 0x000ff024, 0x20276070, 
    0x461060f0, 0xf8511c52, 0x4e8e0020, 0x60303680, 0x1c524610, 0x0020f851, 0xf8c64e8a, 0x46100084, 
    0xf8511c52, 0x4e870020, 0x60303688, 0x1c524610, 0x0020f851, 0x60301d36, 0x4e822001, 0x3b106130, 
    0xbf00e02c, 0x30c0487f, 0xf0006800, 0x28000030, 0x4610d1f8, 0xf8511c52, 0x4e7a0020, 0x60303680, 
    0x1c524610, 0x0020f851, 0xf8c64e76, 0xbf000084, 0x30c04874, 0xf0006800, 0x280000c0, 0x4610d1f8, 
    0xf8511c52, 0x4e6f0020, 0x60303688, 0x1c524610, 0x0020f851, 0xf8c64e6b, 0x3b10008c, 0xd1d02b00, 
    0x4868bf00, 0xf0006900, 0x28000001, 0xbd70d1f9, 0x4603b510, 0xf0201cc8, 0xbf000103, 0x69004861, 
    0x0001f000, 0xd1f92800, 0x6800485e, 0x0040f040, 0x60204c5c, 0x60e02021, 0xf023e020, 0x4c590003, 
    0x68106060, 0x200160a0, 0xf3bf6120, 0xbf008f60, 0x69004854, 0x0001f000, 0xd1f92800, 0x68004851, 
    0x0040f000, 0x484fb138, 0xf0406800, 0x4c4d0040, 0x20016020, 0x1d1bbd10, 0x1f091d12, 0xd1dc2900, 
    0xe7f72000, 0x47f0e92d, 0x460c4605, 0xf04f4616, 0x46c20800, 0x4843bf00, 0xf0006900, 0x28000001, 
    0x4840d1f9, 0xf0406800, 0x493e0040, 0x1ce06008, 0x0403f020, 0xf3c5e02f, 0xb9600008, 0x7f00f5b4, 
    0xf44fd309, 0xeb067700, 0x46390208, 0xf7ff4628, 0x4682ff2c, 0xf3c5e016, 0xb9580008, 0xd3092c10, 
    0x070ff024, 0x0208eb06, 0x46284639, 0xff1df7ff, 0xe0074682, 0xeb064627, 0x46390208, 0xf7ff4628, 
    0x4682ff87, 0x44b8443d, 0xf1ba1be4, 0xd0020f00, 0xe8bd2001, 0x2c0087f0, 0x2000d1cd, 0xb510e7f9, 
    0xf0231ccb, 0xbf000103, 0x691b4b1e, 0x0301f003, 0xd1f92b00, 0x681b4b1b, 0x0340f043, 0x60234c19, 
    0x60e32300, 0xf020e025, 0x4c160303, 0x23006063, 0x230160a3, 0xf3bf6123, 0xbf008f60, 0x691b4b11, 
    0x0301f003, 0xd1f92b00, 0x681b4b0e, 0x0340f003, 0x4b0cb133, 0xf043681b, 0x4c0a0340, 0xbd106023, 
    0x689b4b08, 0x42a36814, 0xe7f8d000, 0x1d121d00, 0x29001f09, 0xbf00d1d7, 0x0000e7f1, 0x40000100, 
    0x40000200, 0x4000c000, 0x00000000, 
};

static const flash_algo_t flash_algo_config = {
    .init = 0x00000001,
    .uninit = 0x0000009d,
    .erase_sector = 0x000000cb,
    .program_page = 0x000002a5,
    .static_base = 0x000003c8,
    .algo_blob = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {0x0, 0x800},           // (start, sector size)
};

static const flash_target_config_t flash_target_config = {
    .page_size  = 0x800,    // 2 KB
    .flash_start = 0x0,
    .flash_size = 0x80000,  // 512 KB
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
}
