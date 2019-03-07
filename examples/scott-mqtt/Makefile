CONTIKI_PROJECT = scott-mqtt

PLATFORMS_EXCLUDE = sky nrf52dk native
BOARDS_EXCLUDE = srf06/cc13x0 launchpad/cc1310 launchpad/cc1350 sensortag/cc2650 sensortag/cc1350

CONTIKI = ../..

MAKE_MAC = MAKE_MAC_TSCH
MODULES += os/services/shell
MODULES += os/net/mac/tsch/sixtop
MODULES += os/net/app-layer/mqtt

-include $(CONTIKI)/Makefile.identify-target
MODULES_REL += arch/platform/$(TARGET)

all: $(CONTIKI_PROJECT)
include $(CONTIKI)/Makefile.include
