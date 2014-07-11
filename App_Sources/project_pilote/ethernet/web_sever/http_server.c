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
static err_t WebPilote_CreateRecvMessage(char *);
static err_t WebPilote_UnpackSendMessage(char *);
static err_t WebPilote_GetPostContent(struct netbuf *, char *);
//static void WebPilote_StopIR();               // This method is implemented as macro
#define WebPilote_StopIR()  do{_mes_pkg_recv.operation = PILOTE_MES_OPERATION_START;\
    xQueueSend(mbox_pilote_recv, &_mes_pkg_recv, 0);}while(0)
//static void WebPilote_StartIR();              // This method is implemented as macro
#define WebPilote_RestartIR()  do{_mes_pkg_recv.operation = PILOTE_MES_OPERATION_STOP;\
    xQueueSend(mbox_pilote_recv, &_mes_pkg_recv, 0);}while(0)
// Variables
static char _web_buf[256];	                    // Web page buffer < 256o
static PiloteMessagePackage _mes_pkg_recv;      // Mes package for recv mbox
static PiloteMessagePackage _mes_pkg_send;      // Mes package for send mbox
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
    extern xQueueHandle mbox_pilote_recv;                                   // Global queue definition
    extern xQueueHandle mbox_pilote_send;
    /**
     *  Initialization
     */
    /**
     *  Attention:
     *      direction and mes_type are set here and should never be changed!
     *      Only change operation, target and data.
     */
    PiloteInitRecvMesPackage(_mes_pkg_recv);                                 // Initialization _mes_pkg_recv
    _mes_pkg_recv.mes_type = PILOTE_MES_TYPE_HTTP;
    PiloteInitSendMesPackage(_mes_pkg_send);                                 // Initialization _mes_pkg_send
    _mes_pkg_recv.mes_type = PILOTE_MES_TYPE_HTTP;
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
                            netconn_write(new_conn, web_pilote_html, (u16_t)WEB_PILOTE_HTML_SIZE, NETCONN_COPY);
                        } else if(!strncmp(http_url, "/orpheo.png", 11)) {
                            in_buf_ptr = NULL;	// Not used
                            netconn_write(new_conn, HTTP_IMAGE_OK, (u16_t)strlen(HTTP_IMAGE_OK), NETCONN_COPY);
                            netconn_write(new_conn, image_orpheo_png, (u16_t)IMAGE_SIZE_ORPHEO_PNG, NETCONN_COPY);
                        } else if(!strncmp(http_url, "/favicon.ico", 12)) {
                            in_buf_ptr = NULL;	// Not used
                            netconn_write(new_conn, HTTP_IMAGE_OK, (u16_t) strlen(HTTP_IMAGE_OK), NETCONN_COPY);
                            netconn_write(new_conn, image_favicon_ico, (u16_t)IMAGE_SIZE_FAVICON_ICO, NETCONN_COPY);
                        } else if (!strncmp(http_url, "/config.txt", 11)) {
                            char *target_ptr;
                            char data[16];
                            // Firstly stop IR
                            WebPilote_StopIR();
                            // Then parse message
                            // 1. Get a target
                            target_ptr = strtok(&http_url[12], "&");        // After "/config.txt?" in url
                            strcpy(_web_buf,"");                            // Clean _web_buf
                            while (target_ptr != NULL) {
                                if (!strncmp(target_ptr, "rand", 4))        // Not a valid item to parse
                                    break;
                                // 2. Create then send a message to config_manager
                                if (WebPilote_CreateRecvMessage(target_ptr) != ERR_OK) {
                                    while (1) {
                                        // Create receive message error, should never get here
                                    }
                                }
                                xQueueSend(mbox_pilote_recv, &_mes_pkg_recv, MBOX_TIMEOUT_INFINIT);
                                // 3. Wait a reply from config_manager, then unpack it
                                xQueueReceive(mbox_pilote_send, &_mes_pkg_send, MBOX_TIMEOUT_INFINIT);
                                if (WebPilote_UnpackSendMessage(data) != ERR_OK) {
                                    while (1) {
                                        // Unpack send message error, should never get here
                                    }
                                }
                                // 4. Create reply message
                                strcat(_web_buf, target_ptr);
                                strcat(_web_buf, "=");
                                strcat(_web_buf, data);
                                strcat(_web_buf, "&");
                                // 5. Next item
                                target_ptr = strtok(NULL, "&");
                            }
                            in_buf_ptr = NULL;  // Not used
                            _web_buf[strlen(_web_buf)-1] = '\0';// Remove the last '&', should be done later in JS
                            netconn_write(new_conn, HTTP_TXT_PLAIN_OK, (u16_t)strlen(HTTP_TXT_PLAIN_OK), NETCONN_COPY);
                            //netconn_write(new_conn, "enable=1&mode=0&frame_nums=2&code=3&time_bt_frame=500", 53, NETCONN_COPY);
                            netconn_write(new_conn, _web_buf, strlen(_web_buf), NETCONN_COPY);
                            // Restart IR
                            WebPilote_RestartIR();
                        } else {                                            // Other resource, ERROR 404
                            Http_SendError404(new_conn);
                        }
                    } else if (!strncmp(http_request, "POST", 4)) { // POST Request
                        if (!strncmp(http_url, "/config.txt", 11)) {
                        // Parse URL for POST request
                            // TODO
                            char post_buf[32];
                            char target[16], value[16];
                            bool flag = 0; // 0:'\r'; 1:'\n'
                            uint8_t count = 0;
                            uint16_t i = 0, j = 0;
                            uint16_t value_int = 0;
                            do {
                                netbuf_data(pxRxBuffer, (void**)&pcRxString, &len);
                                for (i=0 ; i<len ; i++, pcRxString++) {
                                    if (count==4) {
                                        post_buf[j++] = *pcRxString;
                                    } else {
                                        if (*pcRxString=='\r'&&flag==0) {
                                            count++; flag = 1;
                                        } else if (*pcRxString=='\n'&&flag==1) {
                                            count++; flag = 0;
                                        } else {
                                            count = 0; flag = 0;
                                        }
                                    }
                                }
                            } while (netbuf_next(pxRxBuffer)!=-1);
                            pcRxString=NULL;    // Not used
                            post_buf[j] = '\0';
                            // Get post content
                        // TODO
                        } else {
                            Http_SendError404(new_conn);
                        }
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
    netconn_write(new_conn, WEB_HTML_START, (u16_t)strlen(WEB_HTML_START), NETCONN_COPY);
    netconn_write(new_conn, WEB_ERROR_404, (u16_t)strlen(WEB_ERROR_404), NETCONN_COPY);
    netconn_write(new_conn, WEB_HTML_END, (u16_t)strlen(WEB_HTML_END), NETCONN_COPY);
}

/**
 *  WebPilote_CreateRecvMessage
 *      - Creates a recv message
 *      - If an unknown item, returns error; returns ERR_OK otherwise
 */
static err_t WebPilote_CreateRecvMessage(char *target)
{
    _mes_pkg_recv.operation = PILOTE_MES_OPERATION_READ_CONFIG;
    _mes_pkg_recv.data = 0;
    if (!strncmp(target, "enable", 6)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_ENABLE;
    } else if (!strncmp(target, "mode", 4)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_MODE;
    } else if (!strncmp(target, "frame_nums", 10)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_NUMS_OF_FRAMES;
    } else if (!strncmp(target, "code", 4)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_CODE;
    } else if (!strncmp(target, "time_bt", 7)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_TIME_BT_FRAMES;
    } else {
        return ERR_COMMON;
    }
    return ERR_OK;
}

/**
 *  WebPilote_UnpackSendMessage
 *      - Unpack a send message to the item just required
 *      - If not the item required, returns error; returns ERR_OK otherwise
 */
static err_t WebPilote_UnpackSendMessage(char *data)
{
    if (_mes_pkg_send.mes_type  != PILOTE_MES_TYPE_HTTP ||
        _mes_pkg_send.direction != PILOTE_MES_SEND) {           // If not a http mes or a send mes
        return ERR_COMMON;
    }
    if (_mes_pkg_send.target != _mes_pkg_recv.target) {         // If not the item required
        return ERR_COMMON;
    }
    if (_mes_pkg_send.operation != PILOTE_MES_OPERATION_REPLY_CONFIG) {
        return ERR_COMMON;
    }
    UTIL_Num32uToStr((uint8_t*)(data), 16, _mes_pkg_send.data);
    return ERR_OK;
}

/**
 *
 */
static err_t WebPilote_GetPostContent(struct netbuf *, char *)
{

}
