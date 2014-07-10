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
static void Eth_NetifInit(void);
// Variables
struct netif _eth_netif;

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
//    bool link_up = FALSE;
//	uint16_t count = 0;
    /**
     *  Initialize Ethernet
     */
	Eth_NetifInit();
	/**
	 *  Create HTTPServer_Task
	 */
    if (xTaskCreate(HttpServer_Task, "HTTPServer", configMINIMAL_STACK_SIZE+150, NULL, configMAX_PRIORITIES-3, NULL) != pdPASS) {
        while (1) {
        // Error occurs, often out of heap size. Should never get hear.
        }
    }
    /**
     *  Main loop
     */
	while (1) {
        // Check if link is up every 2 seconds
//        if (count == 200 || !link_up) {
//    	    if (!Eth_IsLinkUp()) {
//    	        link_up = FALSE;
//    	    } else {
//                count = 0;
//    	        link_up = TRUE;
//    	    }
//        }
//		// Polling
//        if (link_up) {
            ethernetif_input(&_eth_netif);
            vTaskDelay(2/portTICK_PERIOD_MS);
//            count++;
//        } else {
//            vTaskDelay(2000/portTICK_PERIOD_MS);
//        }
	}
}

/**
 * Initialize lwIP
 * 		This is a static function, used only within the file
 */
static void Eth_NetifInit(void)
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
	IP4_ADDR(&xIpAddr, 192, 168, 1, 59);
	// NetMask : 255.255.255.0
	IP4_ADDR(&xNetMask, 255, 255, 255, 0);
	// Gateway : 192.168.1.1
	IP4_ADDR(&xGateway, 192, 168, 1, 1);
    // Setup net interface
	netif_add(&_eth_netif, &xIpAddr, &xNetMask, &xGateway, NULL, ethernetif_init, tcpip_input);
	netif_set_default(&_eth_netif);
	netif_set_up(&_eth_netif);
}
