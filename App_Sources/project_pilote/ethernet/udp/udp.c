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
    uint16_t port = 1234;                   // port: default 1234
    char udp_buf[16];
    char udp_cmd[8], udp_id[8];             // buffer for udp data, should be long enough to hold '\0'
    err_t error;
    /**
     *  Initialization
     */
    conn = netconn_new(NETCONN_UDP);        // Create a new net connection
    netconn_bind(conn, NULL, port);         // Bind with a port (1234 by default)
    /**
     *  Main loop
     */
    while (1) {
        error = netconn_recv(conn, &in_netbuf);     // Receive a message (with a timeout 100ms)
        if (error == ERR_OK) {                      // If ERR_OK with netconn_recv
            netbuf_copy(in_netbuf, udp_buf, in_netbuf->p->tot_len);
            udp_buf[in_netbuf->p->tot_len] = '\0';
            sscanf(udp_buf, "%[^#]#%s", udp_cmd, udp_id);
            if (UDP_Parse(udp_cmd, udp_id) == ERR_MEM) {
                while (1) {
                    // Memory error, should never get here
                }
            }
            netbuf_delete(in_netbuf);       // Finally delete netbuf
        } else {                            // If timeout
            // Do nothing but re-block onto netconn_recv()
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
 *      - ERR_TIMEOUT: error timeout;
 *      - ERR_COMMON: udp command cannot be parsed
 */
static err_t UDP_Parse(const char *udp_cmd, const char *udp_id)
{
    extern QueueHandle_t mbox_pilote_udp_cmd;   // Mailbox to send udp command message
    PiloteUdpCmdMes udp_cmd_mes;                // Message to be sent
    uint8_t i = 0;

    if (udp_cmd==NULL || udp_id==NULL || mbox_pilote_udp_cmd==NULL) // If memory error
        return ERR_MEM;

    if (strlen(udp_id) != PILOTE_UDP_ID_COUNT) {
        return ERR_COMMON;
    }

    for (i = 0; i<PILOTE_UDP_ID_COUNT; i++) {   // Udp ID
        udp_cmd_mes.udp_id[i] = udp_id[i];
    }

    if (!strncmp(udp_cmd, "P", 2)) {            // Play
        udp_cmd_mes.cmd_type = PILOTE_UDP_CMD_PLAY;
    } else if (!strncmp(udp_cmd, "PS", 3)) {    // Play with synchro
        udp_cmd_mes.cmd_type = PILOTE_UDP_CMD_PLAY_SYNCHRO;
    } else if (!strncmp(udp_cmd, "SP", 3)) {    // Stop
        udp_cmd_mes.cmd_type = PILOTE_UDP_CMD_STOP;
    } else if (!strncmp(udp_cmd, "RM", 3)) {    // Resume
        udp_cmd_mes.cmd_type = PILOTE_UDP_CMD_RESUME;
    } else {
        return ERR_COMMON;
    }

    /**
     *  Send udp_cmd message packet to mailbox
     */
    if (xQueueSend(mbox_pilote_udp_cmd, &udp_cmd_mes, MBOX_TIMEOUT_50MS) != pdTRUE) {
        return ERR_TIMEOUT;
    }
    return ERR_OK;
}
