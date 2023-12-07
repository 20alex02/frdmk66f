/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"
#include <stdio.h>

#if LWIP_UDP

#include "udpecho_raw.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_device_registers.h"

#include "address.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }
#define PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
#define MDIO_OPS enet_ops
#define PHY_OPS phyksz8081_ops
#define CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define NETIF_INIT_FN ethernetif0_init

/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 50
// TODO define last part of IP address
#define configIP_ADDR3 155

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 50
#define configGW_ADDR3 1

#define DATA_BUFFER_SIZE 42

static mdio_handle_t mdioHandle = {.ops = &MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &PHY_OPS};

/*!
 * @brief Interrupt service for SysTick timer.
 */
void SysTick_Handler(void)
{
    time_isr();
}


/*!
 * @brief Main function.
 */
int main(void)
{
    struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw, netif_dst;
    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
    };

    SYSMPU_Type *base = SYSMPU;
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Disable SYSMPU. */
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;
    /* Set RMII clock src. */
    SIM->SOPT2 |= SIM_SOPT2_RMIISRC_MASK;

    mdioHandle.resource.csrClock_Hz = CLOCK_FREQ;

    time_init();

    IP4_ADDR(&netif_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);
    IP4_ADDR(&netif_dst, configSER_ADDR0, configSER_ADDR1, configSER_ADDR2, configSER_ADDR3);

    lwip_init();

    netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, NETIF_INIT_FN, ethernet_input);
    netif_set_default(&netif);
    netif_set_up(&netif);

    udpecho_raw_init();

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" UDP Echo example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_ipaddr)[0], ((u8_t *)&netif_ipaddr)[1],
           ((u8_t *)&netif_ipaddr)[2], ((u8_t *)&netif_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif_netmask)[0], ((u8_t *)&netif_netmask)[1],
           ((u8_t *)&netif_netmask)[2], ((u8_t *)&netif_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_gw)[0], ((u8_t *)&netif_gw)[1],
           ((u8_t *)&netif_gw)[2], ((u8_t *)&netif_gw)[3]);
    PRINTF("************************************************\r\n");

    while (1)
    {
    	char data[DATA_BUFFER_SIZE] = {0};
    	int my_value = 42;

    	snprintf(data, DATA_BUFFER_SIZE, "%i value=%i", configIP_ADDR3, my_value);
    	// USE: udp_send_data;
    	udp_send_data(&netif_dst, DESTINATION_PORT, data);

        /* Poll the driver, get any outstanding frames */
        ethernetif_input(&netif);

        sys_check_timeouts(); /* Handle all system timeouts for all core protocols */

        SDK_DelayAtLeastUs(50000, CLOCK_GetFreq(SYS_CLK));
    }
}
#endif
