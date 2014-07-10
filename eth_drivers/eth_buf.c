/*
 * eth_buf.c
 *
 *  Created on: 3 juin 2014
 *      Author: OrpheoXlite
 */
#include "eth_buf.h"

// Static function decelerations
static uint32_t MEM_Align(uint32_t number);
static void* MEM_AllocAligned(uint16_t size);

// ------------------------- Memory handling routines ----------------------------------
static uint32_t MEM_Align(uint32_t number)
{
	uint32_t misalign = number & MEM_POOL_ALIGN_MASK;
	uint32_t aligned = (number & ~MEM_POOL_ALIGN_MASK) + (misalign ? MEM_POOL_ALIGN_MASK + 1 : 0);

	return aligned;
}

static void* MEM_AllocAligned(uint16_t size)
{
	static uint8_t mem_pool[MEM_POOL_SIZE + MEM_POOL_ALIGN_MASK + 1];
	static uint8_t *mem_pool_top = 0;
	uint16_t mem_pool_step = (uint16_t)MEM_Align(size);

	mem_pool_top = (mem_pool_top ? mem_pool_top : (uint8_t*)MEM_Align((uint32_t)mem_pool)) + mem_pool_step;
	return (void*)(mem_pool_top - mem_pool_step);
}


// ------------------------- Frame transmission routines ----------------------------------
void TX_InitQueues(EthFrameData *eth_data)
{
	int count;
	uint8_t* ptr = 0;
	uint16_t temp;

	LDD_QUEUE_INIT(uint8_t*, eth_data->empty_tx_buffers, 1);
	LDD_QUEUE_INIT(uint8_t*, eth_data->filled_tx_buffers, 1);
	LDD_QUEUE_INIT(uint8_t, eth_data->tx_frag_counts, 1);
	for (count = 0; count < TX_BUFFER_COUNT; count++) {
		ptr = (uint8_t*)MEM_AllocAligned(TX_BUFFER_SIZE);
		LDD_QUEUE_INSERT(eth_data->empty_tx_buffers, ptr, temp);
	}
}

/* Splits the frame data into buffers and sends them. */
bool TX_SendFrame(EthFrameData *eth_data, uint8_t *data, uint16_t size)
{
	int index;
	uint8_t* ptr;
	uint16_t temp;
	uint16_t frag_count;
	LDD_ETH_TBufferDesc buffer_descs[TX_MAX_FRAG_COUNT];

	frag_count = (size / TX_BUFFER_SIZE) + (size % TX_BUFFER_SIZE ? 1 : 0);
	/* Check if there is enough empty buffer */
	if (LDD_QUEUE_GET_ITEM_COUNT(eth_data->empty_tx_buffers, temp) < frag_count) {
		return FALSE;
	}
	for (index = 0; index < size; index++) {
		/* Allocate new buffer for every fragment */
		if (index % TX_BUFFER_SIZE == 0) {
			LDD_QUEUE_REMOVE(eth_data->empty_tx_buffers, ptr, temp);
			LDD_QUEUE_INSERT(eth_data->filled_tx_buffers, ptr, temp);
			buffer_descs[index / TX_BUFFER_SIZE].DataPtr = ptr;
			buffer_descs[index / TX_BUFFER_SIZE].Size = TX_BUFFER_SIZE;
		}
		/* Copy frame data into buffers */
		ptr[index % TX_BUFFER_SIZE] = data[index];
	}
	buffer_descs[frag_count - 1].Size = size % TX_BUFFER_SIZE;
	LDD_QUEUE_INSERT(eth_data->tx_frag_counts, frag_count, temp);
	ETH1_SendFrame(eth_data->eth_device_data, buffer_descs, frag_count);
	return TRUE;
}

/* Releases the buffers of a sent frame. Should be called after each sent frame. */
void TX_ReleaseFrame(EthFrameData *eth_data)
{
	uint8_t* ptr;
	uint16_t temp;
	uint8_t frag_count;

	/* Get the number of sent fragments */
	LDD_QUEUE_REMOVE(eth_data->tx_frag_counts, frag_count, temp);
	/* Move the buffers from filled buffers queue to empty buffers queue */
	for (; frag_count > 0; frag_count--) {
		LDD_QUEUE_REMOVE(eth_data->filled_tx_buffers, ptr, temp);
		LDD_QUEUE_INSERT(eth_data->empty_tx_buffers, ptr, temp);
	}
}



// ------------------------- Frame reception routines ----------------------------------
void RX_InitQueues(EthFrameData *eth_data)
{
	int count;
	uint8_t* ptr = 0;
	uint16_t temp;
	uint8_t *buffers[RX_BUFFER_COUNT];

	LDD_QUEUE_INIT(uint8_t*, eth_data->empty_rx_buffers, 1);
	LDD_QUEUE_INIT(uint8_t*, eth_data->filled_rx_buffers, 1);
	LDD_QUEUE_INIT(uint8_t, eth_data->rx_frag_counts, 1);
	LDD_QUEUE_INIT(uint16_t, eth_data->rx_lengths, 1);
	/* Allocate buffers */
	for (count = 0; count < RX_BUFFER_COUNT; count++) {
		ptr = (uint8_t*)MEM_AllocAligned(RX_BUFFER_SIZE);
		LDD_QUEUE_INSERT(eth_data->empty_rx_buffers, ptr, temp);
		buffers[count] = ptr;
	}
	/* Pass the buffer to be filled with received data */
	ETH1_ReceiveFrame(eth_data->eth_device_data, buffers, RX_BUFFER_COUNT);
}

/* Copies a received frame to a buffer. */
bool RX_ReceiveFrame(EthFrameData *eth_data, uint8_t *data, uint16_t *length)
{
	int index;
	uint8_t *ptr;
	uint16_t temp;
	uint16_t frag_count; /* Received fragment count */
	uint8_t *buffers[RX_MAX_FRAG_COUNT];
	bool result;

	/* Check if there is any filled buffer */
	if (LDD_QUEUE_IS_EMPTY(eth_data->filled_rx_buffers)) {
		return FALSE;
	}
	LDD_QUEUE_REMOVE(eth_data->rx_frag_counts, frag_count, temp);
	LDD_QUEUE_REMOVE(eth_data->rx_lengths, *length, temp);
	if (*length != 0) {
		/* Valid frame */
		for (index = 0; index < *length; index++) {
			/* Allocate new buffer for every fragment */
			if (index % RX_BUFFER_SIZE == 0) {
				LDD_QUEUE_REMOVE(eth_data->filled_rx_buffers, ptr, temp);
				LDD_QUEUE_INSERT(eth_data->empty_rx_buffers, ptr, temp);
				buffers[index / RX_BUFFER_SIZE] = ptr;
			}
			/* Copy frame data into the buffer */
			data[index] = ptr[index % RX_BUFFER_SIZE];
		}
		result = TRUE;
	} else {
		/* Frame with error */
		for (index = 0; index < frag_count; index++) {
			LDD_QUEUE_REMOVE(eth_data->filled_rx_buffers, ptr, temp);
			LDD_QUEUE_INSERT(eth_data->empty_rx_buffers, ptr, temp);
			buffers[index] = ptr;
		}
		result = FALSE;
	}
	ETH1_ReceiveFrame(eth_data->eth_device_data, buffers, frag_count);
	return result;
}
