/*
 * cy_policy.h
 */

#ifndef CY_FLASH_MAP_H_
#define CY_FLASH_MAP_H_

#if defined(__cplusplus)
extern "C" {
#endif

#define MCUBOOT_POLICY_HDR (0)
#define MCUBOOT_POLICY_JWT (1)

#define MCUBOOT_POLICY_FLASH_AREA_0_START   (0x10000000)
#define MCUBOOT_POLICY_FLASH_AREA_1_START   (0x10080000)
#define MCUBOOT_POLICY_FLASH_AREA_SIZE      (0x80000)

#define MCUBOOT_POLICY_BOOT_AUTH       (8)
#define MCUBOOT_POLICY_UPGRADE_AUTH    (8)

#define MCUBOOT_POLICY_IMG_ID          (5)
#define MCUBOOT_POLICY_UPGRADE         (1)

#if defined(__cplusplus)
}
#endif

#endif /* CY_FLASH_MAP_H_ */
