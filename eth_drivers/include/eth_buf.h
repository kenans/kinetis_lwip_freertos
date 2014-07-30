/*
 * eth_buf.c
 *
 *  Created on: 3 juin 2014
 *      Author: OrpheoXlite
 */

#ifndef ETH_BUF_C_
#define ETH_BUF_C_

#include "ETH1.h"

#define MAX_FRAME_SIZE        2048
#define TX_BUFFER_SIZE        512
#define TX_BUFFER_COUNT       4
#define TX_MAX_FRAG_COUNT     MAX_FRAME_SIZE / TX_BUFFER_SIZE
#define RX_BUFFER_SIZE        512
#define RX_BUFFER_COUNT       4
#define RX_MAX_FRAG_COUNT     MAX_FRAME_SIZE / RX_BUFFER_SIZE

#define MEM_POOL_SIZE         ((TX_BUFFER_SIZE * TX_BUFFER_COUNT) + (RX_BUFFER_SIZE * RX_BUFFER_COUNT))
#define MEM_POOL_ALIGN_MASK   0x0F     /* Hardware requirement */

typedef struct {
	LDD_TDeviceData *eth_device_data;
	/* Queues for empty buffers */
	LDD_QUEUE_DECL_VAR(uint8_t*, TX_BUFFER_COUNT, empty_tx_buffers, 1)
	LDD_QUEUE_DECL_VAR(uint8_t*, RX_BUFFER_COUNT, empty_rx_buffers, 1)
	/* Queues for filled buffers */
	LDD_QUEUE_DECL_VAR(uint8_t*, TX_BUFFER_COUNT, filled_tx_buffers, 1)
	LDD_QUEUE_DECL_VAR(uint8_t*, RX_BUFFER_COUNT, filled_rx_buffers, 1)
	/* Queue for numbers of used buffers for a frame */
	LDD_QUEUE_DECL_VAR(uint8_t, TX_BUFFER_COUNT, tx_frag_counts, 1)
	LDD_QUEUE_DECL_VAR(uint8_t, RX_BUFFER_COUNT, rx_frag_counts, 1)
	LDD_QUEUE_DECL_VAR(uint16_t, RX_BUFFER_COUNT, rx_lengths, 1)
} EthFrameData;

// Global APIs
void TX_InitQueues(EthFrameData *eth_data);
bool TX_SendFrame(EthFrameData *eth_data, uint8_t *data, uint16_t size);
void TX_ReleaseFrame(EthFrameData *eth_data);
void RX_InitQueues(EthFrameData *eth_data);
bool RX_ReceiveFrame(EthFrameData *eth_data, uint8_t *data, uint16_t *length);

#endif /* ETH_BUF_C_ */
