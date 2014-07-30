/*
 * ethernet.c
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */
#include "ethernet.h"

/**
 *  Static declarations
 */
// Methods
static void Ethernet_lwIP_Init(void);
// Variables
static struct netif _eth_netif;
static const uint16_t _eth_polling_ms = 2;
static const uint16_t _eth_time_out_ms = 4000;
/**
 *  Eth_Task
 *      A system level task, called by RunTasks(). Detect Ethernet connection and polling messages.
 */
void Eth_Task(void *pvParameters)
{
    /**
     *  Unused variable
     */
	(void)pvParameters;
    /**
     *  Local variables
     */
    bool eth_is_link_up = TRUE;
    bool http_server_task_suspend = FALSE;
    bool udp_task_suspend = FALSE;
    bool phy_is_power_down = FALSE;
    uint16_t polling_count = 0;
    TaskHandle_t http_server_task_handle;
    TaskHandle_t udp_task_handle;

    /**
     *  Initialize Ethernet
     */
    // Ethernet Auto Negotiation
    Eth_MacInit();
    Eth_Reset();
    vTaskDelay(1000/portTICK_PERIOD_MS); // Wait for reset completing
    while (!Eth_PhyInit(PHY_AUTO_NEG, PHY_NO_LOOPBACK)) {
        // If AUTO Neg not finished or not link up, enters power down mode
        Eth_EnterPowerMode(PHY_POWER_DOWN);
        vTaskDelay(5000/portTICK_PERIOD_MS);
        Eth_EnterPowerMode(PHY_POWER_ON);
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
    // lwIP Initialization
	Ethernet_lwIP_Init();
	/**
	 *  Create HTTPServer_Task
	 */
    if (xTaskCreate(HttpServer_Task, "HTTPServer", configMINIMAL_STACK_SIZE+300, NULL,
        configMAX_PRIORITIES-4, &http_server_task_handle) != pdPASS) {
        while (1) {
        // Error occurs, often out of heap size. Should never get hear.
        }
    }
    /**
     *  Create UDP_Task
     */
    if (xTaskCreate(UDP_Task, "UDP", configMINIMAL_STACK_SIZE+100, NULL,
            configMAX_PRIORITIES-2, &udp_task_handle) != pdPASS) {
        while (1) {
            // Error occurs, often out of heap size. Should never get hear.
        }
    }
    /**
     *  Main loop
     */
	while (1) {
        if (polling_count == _eth_time_out_ms/_eth_polling_ms) {
            // Every 5s, check Ethernet connection
            if (phy_is_power_down) {
                if (Eth_EnterPowerMode(PHY_POWER_ON))
                    phy_is_power_down = FALSE;
            }
            if (!phy_is_power_down && !Eth_IsLinkUp()) {
                eth_is_link_up = FALSE;
            }
            while (!eth_is_link_up) {
                // Should power on PHY to check the connection state
                if (phy_is_power_down) {
                    if (Eth_EnterPowerMode(PHY_POWER_ON)) {
                        phy_is_power_down = FALSE;
                        vTaskDelay(2500/portTICK_PERIOD_MS);
                    }
                }
                if (!phy_is_power_down && Eth_IsLinkUp()) {
                    eth_is_link_up = TRUE;
                } else {
                    if (!phy_is_power_down) {
                        if (Eth_EnterPowerMode(PHY_POWER_DOWN))
                            phy_is_power_down = TRUE;
                    }
                    if (!http_server_task_suspend) {
                        http_server_task_suspend = TRUE;
                        vTaskSuspend(http_server_task_handle);
                    }
                    if (!udp_task_suspend) {
                        udp_task_suspend = TRUE;
                        vTaskSuspend(udp_task_handle);
                    }
                }
                vTaskDelay(_eth_time_out_ms/portTICK_PERIOD_MS);
            }
            // Ethernet is link up
            if (http_server_task_suspend) {
                http_server_task_suspend = FALSE;
                vTaskResume(http_server_task_handle);
            }
            if (udp_task_suspend) {
                udp_task_suspend = FALSE;
                vTaskResume(udp_task_handle);
            }
            polling_count = 0;
        }
		// Polling
        ethernetif_input(&_eth_netif);
        vTaskDelay(_eth_polling_ms/portTICK_PERIOD_MS);
        polling_count++;
	}
}

/**
 * Initialize lwIP
 * 		This is a static function, used only within the file
 */
static void Ethernet_lwIP_Init(void)
{
	/**
	 * Local variables
	 */
	struct ip_addr  xIpAddr, xNetMask, xGateway;
	/**
	 * lwIP initialization
	 */
	tcpip_init(NULL, NULL);
	// Ip : 192.168.1.123
	IP4_ADDR(&xIpAddr, ETH_IP_ADDR_0, ETH_IP_ADDR_1, ETH_IP_ADDR_2, ETH_IP_ADDR_3);
	// NetMask : 255.255.255.0
	IP4_ADDR(&xNetMask, ETH_NET_MASK_0, ETH_NET_MASK_1, ETH_NET_MASK_2, ETH_NET_MASK_3);
	// Gateway : 192.168.1.1
	IP4_ADDR(&xGateway, ETH_GATE_WAY_0, ETH_GATE_WAY_1, ETH_GATE_WAY_2, ETH_GATE_WAY_3);
    // Setup net interface
	netif_add(&_eth_netif, &xIpAddr, &xNetMask, &xGateway, NULL, ethernetif_init, tcpip_input);
	netif_set_default(&_eth_netif);
	netif_set_up(&_eth_netif);
}
