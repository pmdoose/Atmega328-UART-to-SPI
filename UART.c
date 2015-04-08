/*
 * UART.c
 *
 * Created: 2015-04-05 15:40:37
 *  Author: Paul Doose
 */ 

#include <avr/interrupt.h>

#include "UART.h"
#include "buffer.h"

BUFFER uart_tran_buffer;

UART_RECVHANDLER UART_recv_handler; //function pointer used to pass control back to the user
void (*UART_recv_next)(uint8_t); //function pointer used to keep a very fast toggle
void UART_recv_hi(uint8_t); //receive high bits
void UART_recv_lo(uint8_t); //receive low bits
int16_t UART_data;

//function to calculate UBRR
inline int16_t UART_calcUBRR(int32_t freq, int32_t baud) {
	return (int16_t)((((freq<<1)/(16UL*baud))-1UL)>>1);
}

//initializes UART port
void UART_init(int16_t ubrr, UART_RECVHANDLER recv) {
	UART_recv_next = &UART_recv_hi;
	UART_recv_handler = recv;
	UART_data = 0;
	
	/*Set baud rate */
	//UBRR0 = (F_CPU/(16UL*baud))-1UL;
	UBRR0 = ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
	
	buf_init(&uart_tran_buffer);
}

//receive high bits
void UART_recv_hi(uint8_t data) {
	UART_recv_next = &UART_recv_lo; //next bit will be low so point to it
	UART_data = (int16_t)(data<<8); //apply data to UART_data
}

//receive low bits
void UART_recv_lo(uint8_t data) {
	UART_recv_next = &UART_recv_hi; //next bit will be high so point to it
	UART_data |= (int16_t)data; //apply data to UART_data
	
	sei(); //receive handler is outside of interrupt code so let it handle interrupt itself
	UART_recv_handler(UART_data);
}

int8_t UART_send(int8_t data) {
	int8_t rtemp;
	uint16_t cast_data = data;
	
	rtemp = buf_insert(&uart_tran_buffer, &cast_data);
	
	if (rtemp) //if data was added to buffer then set interrupt
		UCSR0B |= (1<<UDRIE0);
		
	return rtemp;
}

//Buffered data receive
ISR(USART_RX_vect) {
	uint8_t data = UDR0;
	UART_recv_next(data);
}

//Buffered data send
ISR(USART_UDRE_vect) {
	if ((uint8_t)uart_tran_buffer.read == (uint8_t)uart_tran_buffer.write) {
		UCSR0B &= ~(1<<UDRIE0);
		return;
	}
	
	UDR0 = (uint8_t)uart_tran_buffer.data[++uart_tran_buffer.read];
}

