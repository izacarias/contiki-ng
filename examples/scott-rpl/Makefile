CONTIKI_PROJECT		= hello-scott

PLATFORMS_EXCLUDE = sky nrf52dk native
BOARDS_EXCLUDE = srf06/cc13x0 launchpad/cc1310 launchpad/cc1350 sensortag/cc2650 sensortag/cc1350

CONTIKI = ../..

# MAKE_ROUTING = MAKE_ROUTING_NULLROUTING
# you can set MAKE_ROUTING = MAKE_ROUTING_NULLROUTING in the Makefile and add routes manually, from the C code by calling uip_ds6_route_add

MAKE_MAC = MAKE_MAC_TSCH
MODULES += os/services/shell
MODULES += os/net/mac/tsch/sixtop

all: $(CONTIKI_PROJECT)         
include $(CONTIKI)/Makefile.include
