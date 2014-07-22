/*
 * udp.c
 *
 *  Created on: 22 juil. 2014
 *      Author: OrpheoXlite
 */

#include "udp.h"
/*
 *  Macros and types
 */
/*
 *  Static decelerations
 *      Only used in this file
 */
static err_t UDP_Parse(const char *udp_cmd, const char *udp_id);
/**
 *  Method: UDP_Task()
 *      - An udp task, called by Eth_Task()
 */
void UDP_Task(void *pvParameters)
{
    /**
     *  Unused variable
     */
    (void)pvParameters;
    /**
     *  Local variables
     */
    struct netconn *conn;
    struct netbuf *in_netbuf;
    char *in_buf_ptr;
    uint16_t port = 1234;                   // port: default 1234
    uint16_t len = 0;                       // len: temporary variable for length
    char udp_cmd[8], udp_id[8];             // buffer for udp data, should be long enough to hold '\0'
    /**
     *  Initialization
     */
    conn = netconn_new(NETCONN_UDP);        // Create a new net connection
    netconn_bind(conn, NULL, port);         // Bind with a port (1234 by default)
    /**
     *  Main loop
     */
    while (1) {
        in_netbuf = netconn_recv(conn);     // Receive a message (with a timeout of 100ms,
                                            //        see conn->recv_timeout in api_msg.c)
        if (in_netbuf != NULL) {            // If got a message
            netbuf_data(in_netbuf, (void**)&in_buf_ptr, &len);
            sscanf(in_buf_ptr, "%[^#]#%s", udp_cmd, udp_id);
            if (UDP_Parse(udp_cmd, udp_id) == ERR_MEM) {
                while (1) {
                    // Memory error, should never get here
                }
            }
            netbuf_delete(in_netbuf);       // Finally delete netbuf
        } else {                            // If timeout
            // Do nothing but re-block to netconn_recv()
        }
    }
}

/**
 *  Method: UDP_Parse
 *      - Parse udp_cmd then send a message to IR_Transmitter()
 *  @param
 *      - udp_cmd: string of udp command;
 *      - udp_id: string containing udp id
 *  @return
 *      - ERR_OK: no error occurs;
 *      - ERR_MEM: error of NULL pointer;
 *      - ERR_COMMON: udp command cannot be parsed
 */
static err_t UDP_Parse(const char *udp_cmd, const char *udp_id)
{
    if (udp_cmd==NULL || udp_id==NULL)
        return ERR_MEM;
    if (!strncmp(udp_cmd, "P", 2)) {            // Play
    } else if (!strncmp(udp_cmd, "PS", 3)) {    // Play with synchro
    } else if (!strncmp(udp_cmd, "SP", 3)) {    // Stop
    } else if (!strncmp(udp_cmd, "RM", 3)) {    // Resume
    } else {
        return ERR_COMMON;
    }
    return ERR_OK;
}
