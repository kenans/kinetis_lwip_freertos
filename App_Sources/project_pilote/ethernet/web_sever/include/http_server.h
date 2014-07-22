/*
 * http_server.h
 *
 *  Created on: 2 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

/**
 * -------------------------------------------------------------------------------------------
 *                                          Includes
 * -------------------------------------------------------------------------------------------
 */
// System includes
#include "common.h"
// Project pilote includes
#include "pilote_config.h"
#include "pilote_mes.h"
// Web server includes
#include "webpages.h"
#include "image.h"
/**
 * -------------------------------------------------------------------------------------------
 *                                          APIs
 * -------------------------------------------------------------------------------------------
 */

/**
 *  HttpServer_Task
 *      - An HTTP web server
 */
void HttpServer_Task(void* pvParameters);

#endif /* HTTP_SERVER_H_ */
