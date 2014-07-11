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
static err_t WebPilote_SubmitToConfigManager(char *target, char *data, WebPiloteSubmitType type);
static err_t WebPilote_CreateRecvMessage(char *target, char *data);
static err_t WebPilote_UnpackSendMessage(char *data);
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
                            // Firstly stop IR
                            WebPilote_StopIR();
                            // Then parse message
                            // 1. Get a target
                            target = strtok(&http_url[12], "&");        // After "/config.txt?" in url
                            strcpy(_web_buf,"");                            // Clean _web_buf
                            while (target != NULL) {
                                if (!strncmp(target, "rand", 4))        // Not a valid item to parse
                                    break;
                                // 2. Submit message to config_manager
                                WebPilote_SubmitToConfigManager(target, data, WEB_PILOTE_READ_MESSAGE);
                                // 3. Create reply message
                                strcat(_web_buf, target);
                                strcat(_web_buf, "=");
                                strcat(_web_buf, data);
                                strcat(_web_buf, "&");
                                // 4. Next item
                                target = strtok(NULL, "&");
                            }
                            in_buf_ptr = NULL;  // Not used
                            _web_buf[strlen(_web_buf)-1] = '\0';// Remove the last '&', should be done later in JS
                            netconn_write(new_conn, HTTP_TXT_PLAIN_OK, (u16_t)strlen(HTTP_TXT_PLAIN_OK), NETCONN_COPY);
                            netconn_write(new_conn, _web_buf, strlen(_web_buf), NETCONN_COPY);
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
                            char post_content[32];
                            char target[16], data[16];
                            // 1. Get post content
                            WebPilote_GetPostContent(in_netbuf, post_content);
                            // 2. Parse target and value
                            sscanf(post_content, "%[^=]=%s", target, data);
                            // 3. Submit message to config_messager
                            WebPilote_SubmitToConfigManager(target, data, WEB_PILOTE_MODIFY_MESSAGE);
                            // 4. Send back to web page
                            strcpy(_web_buf, target);
                            strcat(_web_buf, "=");
                            strcat(_web_buf, data);
                            netconn_write(new_conn, _web_buf, strlen(_web_buf), NETCONN_COPY);
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
static err_t WebPilote_SubmitToConfigManager(char *target, char *data,  WebPiloteSubmitType type)
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
    // Wait a reply from config_manager
    xQueueReceive(mbox_pilote_send, &_mes_pkg_send, MBOX_TIMEOUT_INFINIT);
    // Unpach the message
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
 *  WebPilote_GetPostContent
 *      - Gets a post line in the netbuf structure
 *      - If no error, returns ERR_OK; otherwise returns ERR_MEM
 */
static err_t WebPilote_GetPostContent(struct netbuf *in_netbuf, char *post_content)
{
    char *in_buf_ptr = NULL;
    uint16_t len = 0;
    uint16_t i = 0, j = 0;
    uint8_t count = 0;
    bool flag = 0;      // 0 for '\r'; 1 for '\n'
    if (in_netbuf==NULL || post_content==NULL) {
        return ERR_MEM;
    }
    do {
        netbuf_data(in_netbuf, (void**)&in_buf_ptr, &len);
        for (i=0 ; i<len ; i++, in_buf_ptr++) {
            if (count==4) {
                post_content[j++] = *in_buf_ptr;
            } else {
                if (*in_buf_ptr=='\r'&&flag==0) {
                    count++; flag = 1;          // Found a '\r', look for the next '\n'
                } else if (*in_buf_ptr=='\n'&&flag==1) {
                    count++; flag = 0;          // Found a '\n' after '\r', look for the next '\r'
                } else {
                    count = 0; flag = 0;        // Otherwise, restart the research of the first '\r'
                }
            }
        }
    } while (netbuf_next(in_netbuf)!=-1);
    in_buf_ptr=NULL;    // Not used
    post_content[j] = '\0';
    return ERR_OK;
}



