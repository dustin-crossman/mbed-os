#
# $ Copyright Cypress Semiconductor Apache2 $
#

NAME := WHD_for_$(RTOS)

GLOBAL_INCLUDES := . \
                   include \
                   ../inc/WHD \
                   include/network \
                   include/RTOS \
                   bus_protocols \
                   ../../libraries/utilities/linked_list \
                   ../RTOS/$(RTOS)/WHD \
                   ../$(OUTPUT_DIR)/


$(NAME)_CHECK_HEADERS := \
                         whd.h \
                         whd_int.h \
                         whd_ap.h \
                         whd_endian.h \
                         whd_chip.h \
                         whd_sdpcm.h \
                         whd_thread.h \
                         whd_thread_internal.h \
                         whd_chip_constants.h \
                         whd_types_int.h \
                         bus_protocols/whd_bus_common.h \
                         bus_protocols/whd_bus_protocol_interface.h \
                         bus_protocols/whd_sdio.h \
                         bus_protocols/whd_spi.h \
                         bus_protocols/whd_chip_reg.h \
                         bus_protocols/whd_bus_sdio_protocol.h \
                         bus_protocols/whd_bus_spi_protocol.h \
                         include/whd_debug.h \
                         include/whd_events_int.h \
                         include/whd_poll.h \
                         include/whd_wifi_p2p.h \
                         include/whd_utils.h \
                         include/whd_wlioctl.h \
                         include/whd_network_if.h \
                         include/platform/whd_bus_interface.h \
                         include/platform/whd_platform_interface.h \
                         include/platform/whd_resource_interface.h \
                         include/platform/whd_sdio_interface.h \
                         include/platform/whd_spi_interface.h \
                         include/RTOS/whd_rtos_interface.h


ifdef WHD_USE_WIFI_P2P_INTERFACE
GLOBAL_DEFINES += WHD_USE_WIFI_P2P_INTERFACE
ifeq ($(WLAN_CHIP),43362)
WLAN_CHIP_BIN_TYPE ?=-p2p
else
WLAN_CHIP_BIN_TYPE ?=
endif
else
WLAN_CHIP_BIN_TYPE ?=
endif

#Enable special FW with ampdu dump
ifdef WHD_ENABLE_AMPDU_TINY_DUMP_FW
WLAN_CHIP_BIN_TYPE :=-ampdutinydmp
endif

ifndef NO_WIFI_FIRMWARE
# WIFI_FIRMWARE_LOCATION (defaults to WIFI_FIRMWARE_IN_RESOURCES)
#   WIFI_FIRMWARE_IN_RESOURCES
#     WIFI_FIRMWARE_BIN (defaults to firmware/$(WLAN_CHIP)/$(WLAN_CHIP)$(WLAN_CHIP_REVISION)$(WLAN_CHIP_BIN_TYPE).bin)
#     WIFI_FIRMWARE_CLM_BLOB (defaults to $(WIFI_FIRMWARE).clm_blob)
#   WIFI_FIRMWARE_IN_MULTI_APP
#     Wifi firmware is put in the WIFI_FIRMWARE LUT index ... outside of this makefile
#     WIFI_FIRMWARE_CLM_BLOB - not supported
#   WIFI_FIRMWARE_IN_EXTERNAL_STORAGE
#     Wifi firmware ?
#     WIFI_FIRMWARE_CLM_BLOB - not supported
#
#  Note: These make variables (and c define) are not used by WHD but rather higher level
#        parent components and their adaptation code for WHD.  Thus they are not used here.
#           NO_WIFI
#
#$(info WIFI_FIRMWARE_LOCATION $(WIFI_FIRMWARE_LOCATION))
#$(info WIFI_FIRMWARE_BIN $(WIFI_FIRMWARE_BIN))
#$(info WIFI_FIRMWARE_CLM_BLOB $(WIFI_FIRMWARE_CLM_BLOB))
#
# Make wifi firmware a resource by default
WIFI_FIRMWARE_LOCATION ?= WIFI_FIRMWARE_IN_RESOURCES
ifeq ($(WIFI_FIRMWARE_LOCATION), WIFI_FIRMWARE_IN_RESOURCES)

# wifi firmware processing/setup
# Use default chip firwmare if not specified
WIFI_FIRMWARE_BIN ?= firmware/$(WLAN_CHIP)/$(WLAN_CHIP)$(WLAN_CHIP_REVISION)$(WLAN_CHIP_BIN_TYPE).bin
#$(info WIFI_FIRMWARE_BIN $(WIFI_FIRMWARE_BIN))
# Create resource for it
$(NAME)_RESOURCES += $(WIFI_FIRMWARE_BIN)
# The wifi firmware resource variable named based on resource file system specification
WIFI_FIRMWARE_RESOURCE_VARIABLE_NAME = $(addprefix resources_,$(subst /,_DIR_,$(subst -,_,$(subst .,_,$(subst $(SOURCE_ROOT)resources/,,$(WIFI_FIRMWARE_BIN))))))
# Add a c define such that the fixed symbol, wifi_firmwage_image, becomes the actual resource variable name.
GLOBAL_DEFINES += wifi_firmware_image=$(WIFI_FIRMWARE_RESOURCE_VARIABLE_NAME)

# wifi firmware clm blob processing/setup

# clm blob file name is same as firmware but wiht .clm_blob suffix ... unless specified
WIFI_FIRMWARE_CLM_BLOB ?= $(subst .bin,.clm_blob,$(WIFI_FIRMWARE_BIN))
#$(info WIFI_FIRMWARE_CLM_BLOB $(WIFI_FIRMWARE_CLM_BLOB))
# if the clm blob file exists then enable the feature to use it
ifneq ($(wildcard $(addprefix $(SOURCE_ROOT)resources/,$(WIFI_FIRMWARE_CLM_BLOB))),)
# Create resource for it
$(NAME)_RESOURCES += $(WIFI_FIRMWARE_CLM_BLOB)
# The clm blob resource variable named based on resource file system specification
WIFI_FIRMWARE__CLM_BLOB_RESOURCE_VARIABLE_NAME = $(addprefix resources_,$(subst /,_DIR_,$(subst -,_,$(subst .,_,$(subst $(SOURCE_ROOT)resources/,,$(WIFI_FIRMWARE_CLM_BLOB))))))
# Add a c define such that the fixed symbol, wifi_firmwage_clm_blob, becomes the actual resource variable name.
GLOBAL_DEFINES += wifi_firmware_clm_blob=$(WIFI_FIRMWARE__CLM_BLOB_RESOURCE_VARIABLE_NAME)
# Add a c define to enable using/downloading the resource at WLAN power up.
GLOBAL_DEFINES += WHD_DOWNLOAD_CLM_BLOB
#$(NAME)_SOURCES += whd_clm.c
endif
endif
endif

ifeq ($(WLAN_CHIP),)
$(error ERROR: WLAN_CHIP must be defined in your platform makefile)
endif

ifeq ($(WLAN_CHIP_REVISION),)
$(error ERROR: WLAN_CHIP_REVISION must be defined in your platform makefile)
endif

ifeq ($(WLAN_CHIP_FAMILY),)
$(error ERROR: WLAN_CHIP_FAMILY must be defined in your platform makefile)
endif

ifeq ($(HOST_OPENOCD),)
$(error ERROR: HOST_OPENOCD must be defined in your platform makefile)
endif

$(NAME)_CFLAGS  = $(COMPILER_SPECIFIC_PEDANTIC_CFLAGS)

