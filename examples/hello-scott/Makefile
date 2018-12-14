CONTIKI_PROJECT		= hello-scott


PLATFORMS_EXCLUDE = sky nrf52dk native simplelink

CONTIKI = ../..

MAKE_MAC = MAKE_MAC_TSCH
# MAKE_ROUTING = MAKE_ROUTING_NULLROUTING

MODULES += os/services/shell
MODULES += os/net/mac/tsch/sixtop

all: $(CONTIKI_PROJECT)         
include $(CONTIKI)/Makefile.include
