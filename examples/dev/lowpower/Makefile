CONTIKI_PROJECT = lowpower
all: $(CONTIKI_PROJECT)

CONTIKI = ../../..

# Platforms missing gpio-hal.h
PLATFORMS_EXCLUDE += sky z1

# Platforms missing US_TO_RTIMERTICKS and RTIMERTICKS_TO_US
PLATFORMS_EXCLUDE += native 

NRF52_SDK_C_SRCS += components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
					components/libraries/experimental_section_vars/nrf_section_iter.c

NRF52_SDK_INC_PATHS += components/libraries/pwr_mgmt \
					   components/libraries/mutex  \
					   components/libraries/experimental_section_vars

MODULES += $(NRF5_SDK_ROOT)/components/libraries/pwr_mgmt

# CFLAGS += -DNRF_PWR_MGMT=1


include $(CONTIKI)/Makefile.include