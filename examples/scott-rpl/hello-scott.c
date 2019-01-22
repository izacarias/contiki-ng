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
#include "uip.h"
#include <lib/assert.h>
#include "sys/node-id.h"
#include "sys/log.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-debug.h"
#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/sixtop/sixtop.h"
#include "net/mac/tsch/tsch-stats.h"
#include "net/routing/routing.h"

/* For DEBUG -- LEDS */
#include "dev/leds.h"

#define DEBUG DEBUG_PRINT
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

/* Hard-coded MAC address for the TSCH coordinator */
/*   MAC: 00:12:4B:00:04:33:EC:A4               */
static linkaddr_t coordinator_addr =  {{ 0x00, 0x12, 0x4b, 0x00,
                                         0x04, 0x33, 0xec, 0xa4 }};

/* For UDP Connection */
static struct simple_udp_connection udp_conn;

/*---------------------------------------------------------------------------*/
typedef struct ch_stat {
  uint8_t ch;
  tsch_stat_t rssi;
  tsch_stat_t ewma;
} ch_stat_t;

static ch_stat_t ch_stats[TSCH_STATS_NUM_CHANNELS];

/*----------------------------------------------------------------------------*/
PROCESS(hello_scott_process, "Hello SCOTT process");
AUTOSTART_PROCESSES(&hello_scott_process);
/*----------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  memcpy(&ch_stats, data, sizeof(ch_stats));
  LOG_INFO("Received stats from ");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_(":\n");
  
  /* Print TSCH stats */
  for(uint8_t i = 0; i < TSCH_STATS_NUM_CHANNELS; ++i) {
    LOG_INFO("  ch %u: ", ch_stats[i].ch);
    LOG_INFO_("%d rssi, ", ch_stats[i].rssi / TSCH_STATS_RSSI_SCALING_FACTOR);
    LOG_INFO_("%u/%u free", ch_stats[i].ewma, TSCH_STATS_BINARY_SCALING_FACTOR);
    LOG_INFO_("\n");
  }
  
}
/*----------------------------------------------------------------------------*/
static void
blink_led(leds_num_t leds){
  leds_single_on(leds);
  clock_delay_usec(250);
  leds_single_off(leds);
}
/*----------------------------------------------------------------------------*/
PROCESS_THREAD(hello_scott_process, ev, data)
{
  
  uint8_t i;
  static struct etimer timer;
  static int is_coordinator;
  // static char msg[32];
  static unsigned msg_sent_count;
  uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

  /* Set the node 0012.4b00.0433.eca4 as Coordinator */
  is_coordinator = linkaddr_cmp(&coordinator_addr, 
                                &linkaddr_node_addr);

  if(is_coordinator){

    /* Node will act as TSCH Coordinator and RPL Root */
    NETSTACK_ROUTING.root_start();
    NETSTACK_MAC.on();

    /* Registering the UDP connection */
    simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                        UDP_CLIENT_PORT, udp_rx_callback);
    
    /* Loop for coordinator node */
    etimer_set(&timer, CLOCK_SECOND * 10);
    
    while(1) {

      LOG_INFO("DAG ROOT \n");
      /* Wait for the periodic timer to expire and then restart the timer. */
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
      etimer_reset(&timer);
    
    }

  } else {

    /* Node will act as an ordinary node */
    NETSTACK_MAC.on();

    /* Registering the UDP connection */
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                        UDP_SERVER_PORT, udp_rx_callback);

    /* Loop for ordinary node */
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    while(1) {
      
      /* Send TSCH Stats */
      for(i = 0; i < TSCH_STATS_NUM_CHANNELS; ++i) {

        ch_stats[i].ch = TSCH_STATS_FIRST_CHANNEL + i;
        ch_stats[i].rssi = tsch_stats.noise_rssi[i];
        ch_stats[i].ewma = tsch_stats.channel_free_ewma[i];

        LOG_INFO("  ch %u: ", ch_stats[i].ch);
        LOG_INFO_("%d rssi, ", 
            ch_stats[i].rssi / TSCH_STATS_RSSI_SCALING_FACTOR);
        LOG_INFO_("%u/%u free", 
            ch_stats[i].ewma, TSCH_STATS_BINARY_SCALING_FACTOR);
        LOG_INFO_("\n");
      }

      /* Send stats to the DAG Root */
      if(NETSTACK_ROUTING.node_is_reachable() && 
          NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

        /* Send stats to the DAG Root */
        LOG_INFO("Sending stats msg no. %u to ", msg_sent_count);
        LOG_INFO_6ADDR(&dest_ipaddr);
        LOG_INFO_("\n");
        // snprintf(msg, sizeof(msg), "message %d", msg_sent_count);
        simple_udp_sendto(&udp_conn, ch_stats, sizeof(ch_stats), &dest_ipaddr);
        msg_sent_count++;
        blink_led(LEDS_LED2);
      
      } else {

        LOG_INFO("Not reachable yet\n");

      }
      /* Wait for the periodic timer to expire and then restart the timer. */
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}
/*----------------------------------------------------------------------------*/
