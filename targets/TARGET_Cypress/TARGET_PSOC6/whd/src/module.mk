#
# $ Copyright Cypress Semiconductor Apache2 $
#

ifeq ($(WHICHFILE),true)
$(info Processing $(lastword $(MAKEFILE_LIST)) file from directory $(PWD))

endif

# =====  Items below used for IDE only  =====
#
# If we are processing the describe goal, we need to get the
# software components definition file.  If we are building, this is
# not necessary as it has already been included.
#
ifeq ($(MAKECMDGOALS),describe)
ifndef PLATFORMS_VERSION
PLATFORMS_VERSION=1.0
endif
include $(CYSDK)/libraries/platforms-$(PLATFORMS_VERSION)/common/swcomps.mk
endif

#
# The internal tag name of the software component.  Generally not
# seen by customer, but is used in the name of the generated library.
# used for internal make variables by appending to this name to keep
# make variables unique across all components.
#
LIB_BASE_WHD_NAME=LIB_BASE_WHD

#
# Used by the IDE to group and categorize components.
#
CY_COMP_CATEGORY=Base

#
# The displayed human readable name of the component.
#
CY_COMP_NAME_DISPLAY=WHD Source

#
# The name in the form of an identifier ([a-z_][a-z0-9_]*).
# Used to generate directories in the IDE.
#
CY_COMP_NAME_ID=WHD

#
# The human readable description of the component
#
CY_COMP_DESCR=This is WHD Source code

#
# CY_VISIBLE will decide whether to show the component in middleware configurator or not.
# If CY_VISIBLE set to true then it will be shown up in middleware. If CY_VISIBLE set to false then
# component will not show up in middleware.
#
CY_VISIBLE=false

#
# The type of component ...
#   link - means link the source code from the IDE project to the SDK.
#   copy - means copy the source code into the IDE project.
#
CY_COMP_TYPE=link

# =====  Items below used for all builds  =====
#
# If defined, the space separated list of legal OSNS values for
# this component.  If not defined, this component is valid for all
# values of OSNS.
#
#CY_SUPPORTED_OSNS_LIST=

#
# If defined, the space separated the list of legal PLATFORM values for this component.
# If not defined, this component is valid for all values of PLATFORM
#
CY_SUPPORTED_PLATFORM_LIST=$(WHD_STANDARD_WIFI_PLATFORMS)

#
# If defined, the list of legal TOOLCHAIN values for this component.
# If not defined, this component is valid for all values of TOOLCHAIN.
#
#CY_SUPPORTED_TOOLCHAIN_LIST=

#
# The list of components this component is dependent upon.
# The path is relative to the SDK's libraries folder.
#
CY_COMP_DEPS= \
    $(CY_WHD_LIB_COMP_BASE)/WIFI-SDK/WHD/libraries/utilities

#
# Used by the build recipe for an ELF file to add this component
# to the list of components that must be included
#
$(CYARTIFACT)_OBJ_COMP_TAG_LIST += $(LIB_BASE_WHD_NAME)

#
# Includes for this module
#
$(LIB_BASE_WHD_NAME)_INCLUDES= \
    -I$(CY_WHD_LIB_COMP_FULL)/WIFI-SDK/WHD/WHD/internal/bus_protocols/$(subst .,/,$(BUS)) \
    -I$(CY_WHD_LIB_COMP_FULL)/WIFI-SDK/WHD/WHD/internal \
    -I$(CY_WHD_LIB_COMP_FULL)/WIFI-SDK/libraries/utilities/wifi

#
# Extra sources for this module
#
$(LIB_BASE_WHD_NAME)_EXTRA_SOURCES=

#
# Defines the series of needed make variables that include this component in the
# build process.  Also defines the describe target if we are describing a component

$(eval $(call \
	CY_DECLARE_SWCOMP_OBJECT,$(LIB_BASE_WHD_NAME),\
	$(lastword $(MAKEFILE_LIST)),  \
    internal/bus_protocols/whd_bus_common.c \
    internal/whd_chip_specific_functions.c \
    internal/whd_ap_common.c \
    internal/whd_chip_constants.c \
    internal/whd_debug.c \
    internal/whd_eapol.c \
    internal/whd_internal.c \
    internal/whd_logging.c \
    internal/whd_management.c \
    internal/whd_rtos_interface.c \
    internal/whd_sdpcm.c \
    internal/whd_thread.c \
    internal/whd_wifi_sleep.c \
    ../platform/MCU/whd_resources.c \
    internal/whd_chip_constants.h \
    internal/bus_protocols/whd_sdio.h \
    internal/bus_protocols/whd_spi.h \
    internal/bus_protocols/whd_chip_reg.h \
  	include/whd_constants.h\
  	include/whd_stuctures.h\
    include/wiced_network_api.h \
    $($(LIB_BASE_WHD_NAME)_EXTRA_SOURCES)))
