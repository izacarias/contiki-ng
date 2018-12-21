/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A simple Hello SCOTT node.
 * \author
 *         Iulisloi Zacarias (github/izacarias)
 */
#include "contiki.h"
#include <lib/assert.h>
#include "sys/node-id.h"
#include "sys/log.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/sixtop/sixtop.h"
#include "net/routing/routing.h"

#define DEBUG DEBUG_PRINT
#include "net/ipv6/uip-debug.h"

/* Hard-coded MAC address for the TSCH coordinator */
/*   -- MAC: 00:12:4B:00:04:33:EC:A4               */
static linkaddr_t coordinator_addr =  {{ 0x00, 0x12, 0x4b, 0x00,
                                         0x04, 0x33, 0xec, 0xa4 }};

/*---------------------------------------------------------------------------*/
PROCESS(hello_scott_process, "Hello SCOTT process");
AUTOSTART_PROCESSES(&hello_scott_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_scott_process, ev, data)
{
  static struct etimer timer;
  static int is_coordinator;

  PROCESS_BEGIN();

  /* Set the node 0012.4b00.0433.eca4 as Coordinator */
  is_coordinator = linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr);
  

  if(is_coordinator){

    /* Node will act as TSCH Coordinator and RPL Root */
    NETSTACK_ROUTING.root_start();
    NETSTACK_MAC.on();

    /* Loop for coordinator node */
    etimer_set(&timer, CLOCK_SECOND * 10);
    while(1) {
      printf("I'm the coordinator! \n");
      /* Wait for the periodic timer to expire and then restart the timer. */
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
      etimer_reset(&timer);
    }
  } else {

    /* Node will act as an ordinary node */
    NETSTACK_MAC.on();

    /* Loop for ordinary node */
    etimer_set(&timer, CLOCK_SECOND * 10);
    while(1) {
      printf("I'm just a node! \n");
      /* Wait for the periodic timer to expire and then restart the timer. */
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
