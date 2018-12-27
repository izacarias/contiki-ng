CONTIKI_PROJECT		= hello-scott


PLATFORMS_EXCLUDE = sky nrf52dk native simplelink

CONTIKI = ../..

MAKE_MAC = MAKE_MAC_TSCH
# MAKE_ROUTING = MAKE_ROUTING_NULLROUTING
# you can set MAKE_ROUTING = MAKE_ROUTING_NULLROUTING in the Makefile and add routes manually, from the C code by calling uip_ds6_route_add

MODULES += os/services/shell
MODULES += os/net/mac/tsch/sixtop

all: $(CONTIKI_PROJECT)         
include $(CONTIKI)/Makefile.include
