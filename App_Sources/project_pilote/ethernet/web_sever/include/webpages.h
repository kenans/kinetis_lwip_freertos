/*
 * webpages.h
 *
 *  Created on: 2 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef WEBPAGES_H_
#define WEBPAGES_H_

/**
 *  Standard HTTP response headers
 */
#define HTTP_HTML_OK            "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n"
#define HTTP_IMAGE_OK           "HTTP/1.1 200 OK\r\nContent-type: image\r\n\r\n"
#define HTTP_JS_OK              "HTTP/1.1 200 OK\r\nContent-type: text/javascript\r\n\r\n"
#define HTTP_CSS_OK             "HTTP/1.1 200 OK\r\nContent-type: text/css\r\n\r\n"
#define HTTP_TXT_PLAIN_OK       "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\n"
/**
 *  Web page resources for dynamic pages
 */
#define WEB_HTML_START \
"<html>\
<head>\
<title>Driver Manager</title>\
<style type=\"text/css\">\
body{text-align:center;}\
h1{font-size:30px;color:green;font-family:Arial;}\
p1{font-size:25px;}\
</style>\
</head>\
<body>"
#define WEB_HTML_END \
"</body>\r\n\
</html>"
#define WEB_ERROR_404 \
"<h1>Error</h1>\
<p1>404 - Page not found</p1>"
/**
 *  Web page resources for static pages
 */
#define WEB_PILOTE_HTML_SIZE   7806
extern const char web_pilote_html[WEB_PILOTE_HTML_SIZE];

#endif /* WEBPAGES_H_ */
