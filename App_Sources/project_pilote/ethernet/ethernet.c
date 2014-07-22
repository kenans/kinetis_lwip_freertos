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
static struct netif _eth_netif;
static const uint16_t _eth_polling_ms = 2;
static const uint16_t _eth_time_out_ms = 5000;
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
    bool http_server_task_suspend = FALSE;
    bool udp_task_suspend = FALSE;
    uint16_t polling_count = 0;
    TaskHandle_t http_server_task_handle;
    TaskHandle_t udp_task_handle;
    /**
     *  Initialize Ethernet
     */
	Eth_NetifInit();
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
        // Check if link is up every 2 seconds
        if (polling_count == _eth_time_out_ms/_eth_polling_ms) {
            while (!Eth_IsLinkUp()) {
                if (!http_server_task_suspend) {
                    http_server_task_suspend = TRUE;
                    vTaskSuspend(http_server_task_handle);
                }
                if (!udp_task_suspend) {
                    udp_task_suspend = TRUE;
                    vTaskSuspend(udp_task_handle);
                }
                vTaskDelay(_eth_time_out_ms/portTICK_PERIOD_MS);
            }
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
