/*
 * Copyright (c) 2017, Yasuyuki Tanaka
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
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* Needed for CC2538 platforms only */
/* For TSCH we have to use the more accurate crystal oscillator
 * by default the RC oscillator is activated */
#define SYS_CTRL_CONF_OSC32K_USE_XTAL       1


/*******************************************************/
/******************* Configure TSCH ********************/
/*******************************************************/

/* IEEE802.15.4 PANID */
#define IEEE802154_CONF_PANID               0x5c07

/* Enabling the use of 6top sublayer */
#define TSCH_CONF_WITH_SIXTOP               1
/* Do not start TSCH at init, wait for NETSTACK_MAC.on() */
#define TSCH_CONF_AUTOSTART                 0
#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE  TSCH_HOPPING_SEQUENCE_1_1
/* 6TiSCH minimal schedule length. */
// #define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH   3

/* Enable TSCH statistics */
#define TSCH_STATS_CONF_ON                  1
/* Enable periodic RSSI sampling for TSCH statistics */
#define TSCH_STATS_CONF_SAMPLE_NOISE_RSSI   1
/* Reduce the TSCH stat "decay to normal" period to get printouts more often */
#define TSCH_STATS_CONF_DECAY_INTERVAL      (60 * CLOCK_SECOND)

/* Setting the log levels */
#define LOG_CONF_LEVEL_IPV6                 LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_RPL                  LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_6LOWPAN              LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_6TOP                 LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_TCPIP                LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_MAC                  LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_FRAMER               LOG_LEVEL_DBG
#define TSCH_LOG_CONF_PER_SLOT              1

#endif /* PROJECT_CONF_H_ */
