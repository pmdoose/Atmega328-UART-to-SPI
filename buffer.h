/*
 * buffer.h
 *
 * Created: 2015-04-05 15:42:37
 *  Author: Paul Doose
 */

#include <avr/io.h>

#ifndef BUFFER_H_
#define BUFFER_H_

/*
 * Using one spot in buffer to indicate state of buffer
 * so there will always be one spot empty
 * buffer max size will be 255 not 256
 */
typedef struct BUFFER {
	uint8_t		read; //buffer read position
	uint8_t		write; //buffer write position
	uint16_t	data[256]; //data array
} BUFFER, *LPBUFFER;

/*
 * buf_init()
 *
 * out LPBUFFER buf - data buffer
 * return null
 *
 * initializes buffer
 */
void buf_init(LPBUFFER buf);

/*
 * buf_insert()
 *
 * in out LPBUFFER buf - data buffer
 * in int16_t* data - data to be inserted into buffer
 * return
 *	 0 - buffer full no update
 *  -1 - buffer full, but item was added
 *   1 - item was added
 *
 * Adds item into buffer
 */
int8_t buf_insert(LPBUFFER buf, uint16_t* data);

/*
 * buf_remove()
 *
 * in out LPBUFFER buf - data buffer
 * out int16_t* data - data to be removed from buffer
 * return
 *	 0 - buffer empty no update
 *  -1 - buffer empty, but item was returned
 *   1 - item was returned
 *
 * removed and returns item from buffer
 */
int8_t buf_remove(LPBUFFER buf, uint16_t* data);

/*
 * buf_fill()
 *
 * in LPBUFFER buf - data buffer
 * returns current fill count of buffer
 */
uint8_t buf_fill(LPBUFFER buf);

#endif /* BUFFER_H_ */
