/*
 * UART.h
 *
 * Created: 2015-04-05 15:42:17
 *  Author: Paul Doose
 */ 

#ifndef UART_H_
#define UART_H_

typedef void (*UART_RECVHANDLER)(uint16_t data);

/*
 * UART_init()
 *
 * in int16_t ubrr - UBRR value for uart calcuated as (F_CPU/(16UL*baud))-1UL;
 * in fptr UART_recv_handler() - a function pointer to the asynch recieve handler
 *		* in data - this is the 16 bit data received by the UART communication
 *		* return void
 * return void
 *
 * initializes UART
 */
void UART_init(int16_t ubrr, UART_RECVHANDLER UART_recv);

/*
 * UART_calcUBRR()
 *
 * in int32_t freq - CPU frequency
 * in int32_t baud - Desired baud rate
 * return void
 *
 * Initializes UART
 * Note this is done on the processor live, not in the compiler
 *		Using this is very processor intensive.
 * Uses rounded version of the formula (F_CPU/(16UL*BAUD))-1UL
 *		It is very is as accurate as any table I found
 */
int16_t UART_calcUBRR(int32_t freq, int32_t baud);

/*
 * UART_send()
 *
 * in int8_t data - data to be added to send buffer
 * return void
 *	 0 - buffer full no update
 *  -1 - buffer full, but item was added
 *   1 - item was added
 *
 * Sends data out over UART
 */
int8_t UART_send(int8_t data);

#endif /* UART_H_ */
