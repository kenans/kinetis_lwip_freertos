/*
 * http_server.c
 *
 *  Created on: 2 juin 2014
 *      Author: OrpheoXlite
 */
#include "http_server.h"
/*
 *  Macros and types
 */
typedef enum {
    WEB_PILOTE_READ_MESSAGE,
    WEB_PILOTE_MODIFY_MESSAGE
} WebPiloteSubmitType;
/*
 *  Static decelerations
 *      Only used in this file
 */
// Methods
static void Http_SendError404(struct netconn *new_conn);
static err_t WebPilote_SubmitToConfigManager(char *target, char *data, const WebPiloteSubmitType type);
static err_t WebPilote_CreateRecvMessage(char *target, char *data);
static err_t WebPilote_UnpackSendMessage(char *data);
static err_t WebPilote_GetPostBody(struct netbuf *, struct netconn *, char *);
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
 *      - An http server, doing configuration from a web page.
 *      - This task is called by Eth_Task();
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
//            WebPilote_StopIR();
            while (1) {
                in_netbuf =  netconn_recv(new_conn);                // Block and receive data
                if (in_netbuf != NULL) {
                    netbuf_data(in_netbuf, (void**)&in_buf_ptr, &len);     // Get in_netbuf data address
                    sscanf(in_buf_ptr, "%s %s", http_request, http_url);   // Parse http_request and http_url
                    if (!strncmp(http_request, "GET", 3)) {         // If GET request
                        if(!strncmp(http_url, "/", 2)  ||
                           !strncmp(http_url, "/index.html", 11)) { // If / or /index.html
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
                            char *target;
                            char data[16];
                            err_t error;
                            // Firstly stop IR
                            WebPilote_StopIR();
                            // Then parse message
                            // 1. Get a target
                            if (strlen(http_url) > 12) {          // If not contains data in url, do nothing
                                target = strtok(&http_url[12], "&");        // After "/config.txt?" in url
                                strcpy(_web_buf,"");                        // Clean _web_buf
                                while (target != NULL) {
                                    if (!strncmp(target, "rand", 4))        // Not a valid item to parse
                                        break;
                                    // 2. Submit message to config_manager
                                    error = WebPilote_SubmitToConfigManager(target, data, WEB_PILOTE_READ_MESSAGE);
                                    if (error == ERR_MEM) {
                                        while (1) {
                                            // Memory error, should never get here
                                        }
                                    }
                                    if (error == ERR_OK) {  // For unknown items, ignore them
                                        // 3. Create reply message
                                        strcat(_web_buf, target);
                                        strcat(_web_buf, "=");
                                        strcat(_web_buf, data);
                                        strcat(_web_buf, "&");
                                    }
                                    // 4. Next item
                                    target = strtok(NULL, "&");
                                }
                                in_buf_ptr = NULL;  // Not used
                                if (_web_buf[0] != '\0') {
                                    _web_buf[strlen(_web_buf)-1] = '\0';// Remove the last '&'
                                }
                                netconn_write(new_conn, HTTP_TXT_PLAIN_OK, (u16_t)strlen(HTTP_TXT_PLAIN_OK), NETCONN_COPY);
                                netconn_write(new_conn, _web_buf, strlen(_web_buf), NETCONN_COPY);
                            }
                            // Restart IR
                            WebPilote_RestartIR();
                        } else {                                            // Other resource, ERROR 404
                            Http_SendError404(new_conn);
                        }
                    } else if (!strncmp(http_request, "POST", 4)) { // POST Request
                        if (!strncmp(http_url, "/config.txt", 11)) {
                            // Firstly stop IR
                            WebPilote_StopIR();
                            // Parse URL for POST request
                            char post_body_buf[32];
                            char target[16], data[16];
                            // 1. Get post content
                            if (WebPilote_GetPostBody(in_netbuf, new_conn, post_body_buf) == ERR_OK) {
                                // 2. Parse target and value
                                sscanf(post_body_buf, "%[^=]=%s", target, data);
                                // 3. Submit message to config_messager
                                if (WebPilote_SubmitToConfigManager(target, data, WEB_PILOTE_MODIFY_MESSAGE)
                                        == ERR_OK) {
                                // 4. Send back to web page
                                    strcpy(_web_buf, HTTP_TXT_PLAIN_OK);
                                    strcat(_web_buf, target);
                                    strcat(_web_buf, "=");
                                    strcat(_web_buf, data);
                                    netconn_write(new_conn, _web_buf, strlen(_web_buf), NETCONN_COPY);
                                }
                            }
                            // Restart IR
                            WebPilote_RestartIR();
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
//            WebPilote_RestartIR();
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
 *
 */
static err_t WebPilote_SubmitToConfigManager(char *target, char *data,  const WebPiloteSubmitType type)
{
    err_t error = ERR_OK;
    extern xQueueHandle mbox_pilote_recv;                                   // Global queue definition
    extern xQueueHandle mbox_pilote_send;

    if (target == NULL || data == NULL) {
        return ERR_MEM;
    }
    // Create a message
    if (type == WEB_PILOTE_READ_MESSAGE) {
        error = WebPilote_CreateRecvMessage(target, NULL);
    } else if (type == WEB_PILOTE_MODIFY_MESSAGE) {
        error = WebPilote_CreateRecvMessage(target, data);
    } else {
        return ERR_COMMON;
    }
    if (error != ERR_OK) {
        return error;
    }
    // Send message to config_manager
    xQueueSend(mbox_pilote_recv, &_mes_pkg_recv, MBOX_TIMEOUT_INFINIT);
    // Wait for an HTTP message packet
    do {
        xQueueReceive(mbox_pilote_send, &_mes_pkg_send, MBOX_TIMEOUT_INFINIT);
        if (_mes_pkg_send.mes_type!=PILOTE_MES_TYPE_HTTP) {
            // If not an HTTP packet, send it back to the front (still might be disordered)
            xQueueSendToFront(mbox_pilote_send, &_mes_pkg_send, MBOX_TIMEOUT_INFINIT);
        } else {
            // If got an HTTP packet, then unpack it.
            break;
        }
    } while (1);
    // Unpack the message
    error = WebPilote_UnpackSendMessage(data);
    if (error != ERR_OK) {
        return error;
    }
    return ERR_OK;
}
/**
 *  WebPilote_CreateRecvMessage
 *      - Creates a recv message
 *      - If an unknown item, returns error; returns ERR_OK otherwise
 *  @param
 *      - *target: a char buffer containing target from the web page
 *      - *data: a char buffer containing data from the web page; should be NULL in case no data (read a config)
 *  @return
 *      - ERR_OK: If no error occurs
 *      - ERR_MEM: If error of NULL pointer
 *      - ERR_COMMON: If error of an unknown item
 */
static err_t WebPilote_CreateRecvMessage(char *target, char *data)
{
    if (target == NULL) {
        return ERR_MEM;
    }
    if (data == NULL) {
        _mes_pkg_recv.data = 0;
        _mes_pkg_recv.operation = PILOTE_MES_OPERATION_READ_CONFIG;
    } else {
        _mes_pkg_recv.data = atoi(data);
        _mes_pkg_recv.operation = PILOTE_MES_OPERATION_MODIFY;
    }
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
    } else if (!strncmp(target, "output_mode", 11)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_OUTPUT_MODE;
    } else if (!strncmp(target, "source_mode", 11)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_SOURCE_MODE;
    } else if (!strncmp(target, "video_mode", 10)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_VIDEO_MODE;
    } else if (!strncmp(target, "token_ring", 10)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_TOKEN_RING;
    } else if (!strncmp(target, "group", 5)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_GROUP;
    } else if (!strncmp(target, "udp_id", 6)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_UDP_ID;
    } else if (!strncmp(target, "udp_data", 8)) {
        // TODO
    } else if (!strncmp(target, "command_param", 13)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_COMMAND_PARAM;
    } else if (!strncmp(target, "command_data", 12)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_COMMAND_DATA;
    } else if (!strncmp(target, "delay_source", 12)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_DELAY_SOURCE;
    } else if (!strncmp(target, "delay_output", 12)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_DELAY_OUTPUT;
    } else if (!strncmp(target, "start_time", 10)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_START_TIME;
    } else if (!strncmp(target, "end_time", 8)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_END_TIME;
    } else if (!strncmp(target, "weekday", 8)) {
        _mes_pkg_recv.target = PILOTE_MES_TARGET_WEEKDAY;
    } else {
        return ERR_COMMON;
    }
    return ERR_OK;
}

/**
 *  WebPilote_UnpackSendMessage
 *      - Unpack a send message to the item just required
 *      - If not the item required, returns error; returns ERR_OK otherwise
 *  @param
 *      - *data: a char buffer containing data from the web page; should never be NULL
 *  @return
 *      - ERR_OK: If no error occurs
 *      - ERR_MEM: If error of NULL pointer
 *      - ERR_COMMON: If error of an unknown item
 */
static err_t WebPilote_UnpackSendMessage(char *data)
{
    if (data == NULL)
        return ERR_MEM;
    if (_mes_pkg_send.mes_type  != PILOTE_MES_TYPE_HTTP ||
        _mes_pkg_send.direction != PILOTE_MES_SEND) {           // If not a http mes or a send mes
        return ERR_COMMON;
    }
    if (_mes_pkg_send.target != _mes_pkg_recv.target) {         // If not the item required
        return ERR_COMMON;
    }
    if (_mes_pkg_send.operation != PILOTE_MES_OPERATION_REPLY_CONFIG &&
        _mes_pkg_send.operation != PILOTE_MES_OPERATION_REPLY_MODIFY) {
        return ERR_COMMON;
    }
    UTIL_Num32uToStr((uint8_t*)(data), 16, _mes_pkg_send.data);
    return ERR_OK;
}

/**
 *  WebPilote_GetPostBody
 *      - Gets the body of a POST request in the netbuf structure. In a POST request, the header part and
 *        the body is separated by "\r\n\r\n". So firstly find this separator, then the remaining text is
 *        the post content.
 *        Attention: For some browsers, like IEs, a POST request is sent by two TCP packets: the first of
 *        which contains the request header while the second contains the corresponding body. In this case,
 *        we should receive the following TCP packet to get the post content.
 *      - If no error, returns ERR_OK; otherwise returns ERR_MEM
 */
static err_t WebPilote_GetPostBody(struct netbuf *in_netbuf,
        struct netconn *new_conn, char *post_body_buf)
{
    char *in_buf_ptr = NULL;
    uint16_t len = 0;
    uint16_t i = 0;
    uint16_t post_body_count = 0;
    uint8_t r_and_n_count = 0;
    bool find_next_n = FALSE;      // TRUE: should find a '\n' next; FALSE: should find a '\r' next
    if (in_netbuf==NULL || post_body_buf==NULL) {
        return ERR_MEM;
    }
    do {
        netbuf_data(in_netbuf, (void**)&in_buf_ptr, &len);
        for (i=0 ; i<len ; i++, in_buf_ptr++) {
            if (r_and_n_count==4) {
                post_body_buf[post_body_count++] = *in_buf_ptr;
            } else {
                if (*in_buf_ptr=='\r' && !find_next_n) {
                    r_and_n_count++; find_next_n = TRUE;          // Found a '\r', look for the next '\n'
                } else if (*in_buf_ptr=='\n' && find_next_n) {
                    r_and_n_count++; find_next_n = FALSE;          // Found a '\n' after '\r', look for the next '\r'
                } else {
                    r_and_n_count = 0; find_next_n = FALSE;        // Otherwise, restart the research of the first '\r'
                }
            }
        }
    } while (netbuf_next(in_netbuf)!=-1);
    in_buf_ptr=NULL;    // Not used
    // Should receive the next TCP packet in case post_body_count==0
    if (post_body_count==0) {
        struct netbuf *temp_netbuf;
        uint8_t time_out_count = 3;
        for (;time_out_count>0;time_out_count--) {
            temp_netbuf =  netconn_recv(new_conn);
            if (temp_netbuf != NULL) {
                netbuf_data(temp_netbuf, (void**)&in_buf_ptr, &len);
                for (i=0;i<len;i++, in_buf_ptr++) {
                    post_body_buf[post_body_count++] = *in_buf_ptr;
                }
                // Delete temp_netbuf
                netbuf_delete(temp_netbuf);
            }
            if (post_body_count != 0) break;
        }
    }
    in_buf_ptr=NULL;    // Not used
    // If still post_body_count==0, POST message cannot be parsed.
    if (post_body_count==0)
        return ERR_COMMON;
    post_body_buf[post_body_count] = '\0';

    return ERR_OK;
}



