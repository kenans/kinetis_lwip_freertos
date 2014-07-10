/*
 * http_server.c
 *
 *  Created on: 2 juin 2014
 *      Author: OrpheoXlite
 */
#include "http_server.h"

/*
 *  Static decelerations
 *      Only used in this file
 */
// Methods
static void Http_SendError404(struct netconn *new_conn);
// Variables
static char _web_page[512];	                                    // Web page buffer < 512o
//static PiloteMessagePackage _mes_pkg_recv;                      // Mes package for recv mbox
//static PiloteMessagePackage _mes_pkg_send;                      // Mes package for send mbox
/**
 *  HttpServer_Task
 *      - Creates a simple HTTP web server
 */
void HttpServer_Task(void* pvParameters)
{
    /**
     *  Unused variable
     */
    (void)pvParameters;
    /**
     *  Local variables
     */
    struct netconn *listener_conn, *new_conn;                               //
    struct netbuf *in_netbuf;                                               //
    char *in_buf_ptr = NULL;
    char http_request[5];
    char http_url[128];
    u16_t len = 0;                                                          // A temporary variable storing length
//    extern xQueueHandle mbox_pilote_recv;                                   // Global queue definition
//    extern xQueueHandle mbox_pilote_send;
    /**
     *  Initialization
     */
    /**
     *  Attention:
     *      direction and mes_type are set here and should never be changed!
     *      Only change operation, target and data.
     */
//    PiloteInitRecvMesPackage(_mes_pkg_recv);                                 // Initialization _mes_pkg_recv
//    _mes_pkg_recv.mes_type = PILOTE_MES_TYPE_HTTP;
//    PiloteInitSendMesPackage(_mes_pkg_send);                                 // Initialization _mes_pkg_send
//    _mes_pkg_recv.mes_type = PILOTE_MES_TYPE_HTTP;
    /**
     *  Web server initialization
     */
    listener_conn = netconn_new(NETCONN_TCP);                       // Create a TCP connection for listening
    netconn_bind(listener_conn, NULL, 80);                          // Bind with port 80
    netconn_listen(listener_conn);                                  // Listen...
    /**
     *  Main loop
     */
    while (1) {
        new_conn = netconn_accept(listener_conn);                   // Wait for a new connection
        if (new_conn != NULL) {                                     // If got a connection
            while (1) {
                in_netbuf =  netconn_recv(new_conn);                // Block and receive data
                if (in_netbuf != NULL) {
                    netbuf_data(in_netbuf, (void**)&in_buf_ptr, &len);     // Get in_netbuf data address
                    sscanf(in_buf_ptr, "%s %s", http_request, http_url);   // Parse http_request and http_url
                    if (!strncmp(http_request, "GET", 3)) {         // If GET request
                        if(!strncmp(http_url, "/", 2)  ||
                           !strncmp(http_url, "/index.html", 11)) { // If / or /index.html
                            // TODO
                            in_buf_ptr = NULL;  // Not used
                            netconn_write(new_conn, HTTP_HTML_OK, (u16_t)strlen(HTTP_HTML_OK), NETCONN_COPY);
                            netconn_write(new_conn, web_pilote_html, (u16_t)sizeof(web_pilote_html), NETCONN_COPY);
                        } else if(!strncmp(http_url, "/orpheo.png", 11)) {
                            in_buf_ptr = NULL;	// Not used
                            netconn_write(new_conn, HTTP_IMAGE_OK, (u16_t)strlen(HTTP_IMAGE_OK), NETCONN_COPY);
                            netconn_write(new_conn, image_orpheo_png, (u16_t)sizeof(image_orpheo_png), NETCONN_COPY);
                        } else if(!strncmp(http_url, "/favicon.ico", 12)) {
                            in_buf_ptr = NULL;	// Not used
                            netconn_write(new_conn, HTTP_IMAGE_OK, (u16_t) strlen(HTTP_IMAGE_OK), NETCONN_COPY);
                            netconn_write(new_conn, image_favicon_ico, (u16_t)sizeof(image_favicon_ico), NETCONN_COPY);
                        } else if (!strncmp(http_url, "/config.txt", 11)) {
//                            char *temp_ptr;
                            // Firstly stop IR
                            // TODO
                            // Then parse message
                            // 1. Got a target
//                            temp_ptr = strtok(&http_url[12], "&");
//                            while (temp_ptr != NULL) {
//                                if (!strncmp(temp_ptr, "rand", 4))     // Not a valid item to parse
//                                    break;
//                                // 2. Create then send a message to config_manager
//                                WebPilote_CreateRecvMessage(temp_ptr);
//                                xQueueSend(mbox_pilote_recv, &_mes_pkg_recv, MBOX_TIMEOUT_INFINIT);
//                                // 3. Wait a reply from config_manager, then unpack it
//                                xQueueReceive(mbox_pilote_send, &_mes_pkg_send, MBOX_TIMEOUT_INFINIT);
//                                WebPilote_UnpackSendMessage(&data);
//                                // 4. Store data, then pass to the next item
//                                strcat(reply, data);
//                                temp_ptr = strtok(NULL, "&");
//                            }
//                            in_buf_ptr = NULL;  // Not used
                            // Create a reply
                            netconn_write(new_conn, HTTP_TXT_PLAIN_OK, (u16_t)strlen(HTTP_TXT_PLAIN_OK), NETCONN_COPY);
                            netconn_write(new_conn, "enable=Enable&mode=Trigger&frame_nums=2&code=3&time_bt_frame=500", 64, NETCONN_COPY);
                        } else {                                    // Other resource, ERROR 404
                            Http_SendError404(new_conn);
                        }
                    } else if (!strncmp(http_request, "POST", 4)) { // POST Request
                        // Parse URL for POST request
                        // TODO
                        // Get post content
                        // TODO
                    } else {                                        // Other requests, ERROR 404
                        Http_SendError404(new_conn);
                    }
                    // Delete in_netbuf
                    netbuf_delete(in_netbuf);
                } else {
                    // Receive timeout, should close new_conn
                    break;
                }
            }
            // Close this connection
            netconn_close(new_conn);
            // Wait until this connection is deleted
            while (netconn_delete(new_conn) != ERR_OK) {
                vTaskDelay(5/portTICK_PERIOD_MS);
            }
        } else {
            // Accept timeout
        }
    }
}

/**
 *  Http_SendError404
 *      - Sends an error404 page
 */
static void Http_SendError404(struct netconn *new_conn)
{
    netconn_write(new_conn, HTTP_HTML_OK, (u16_t)strlen(HTTP_HTML_OK), NETCONN_COPY);
    strcpy(_web_page, WEB_HTML_START);
    strcat(_web_page, WEB_ERROR_404);
    strcat(_web_page, WEB_HTML_END);
    netconn_write(new_conn, _web_page, (u16_t)strlen(_web_page), NETCONN_COPY);
}
