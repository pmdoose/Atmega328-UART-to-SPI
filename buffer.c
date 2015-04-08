/*
 * buffer.c
 *
 * Created: 2015-04-05 15:42:49
 *  Author: Paul Doose
 */ 

#include <avr/interrupt.h>
#include "buffer.h"

inline void buf_init(LPBUFFER buf) {
	buf->read = buf->write;
}

//store value in buffer
//return of 0 indicates error buffer full
//return of 1 indicates success
//return of -1 indicates success but buffer is now full
inline int8_t buf_insert(LPBUFFER buf, uint16_t *data) {
	cli(); //disable for data update
	if ((uint8_t)buf->read == (uint8_t)(buf->write+1)) {
		sei(); //re-enable interrupts
		return 0; //buffer full
	}
	
	buf->data[++buf->write] = *data; //write to buffer
	sei(); //re-enable interrupts
	
	if ((uint8_t)buf->read == (uint8_t)(buf->write+1))
		return -1; //buffer is now full
	return 1; //buffer in middle somewhere
		
}

//get value from value in buffer
//return of 0 indicates error buffer empty
//return of 1 indicates success
//return of -1 indicates success but buffer is now empty
inline int8_t buf_remove(LPBUFFER buf, uint16_t *data) {
	cli(); //disable for data update
	if ((uint8_t)buf->read == (uint8_t)buf->write) {
		sei(); //re-enable interrupts
		return 0; //buffer empty
	}
		
	*data = buf->data[++buf->read]; //read from buffer
	sei(); //re-enable interrupts
	
	if ((uint8_t)buf->read == (uint8_t)buf->write)
		return -1; //buffer is now empty
	return 1; //buffer in middle somewhere
}

//returns current buffer fill count
inline uint8_t buf_fill(LPBUFFER buf) {
	int16_t temp;
	temp = ((buf->write < buf->read)?256:0) + buf->write;
	temp -= buf->read;
	return temp;
}
