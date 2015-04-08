/*
 * bufUARTtoSPI.c
 *
 * Created: 2015-04-05 15:40:02
 *  Author: Paul Doose
 */ 

#define F_CPU 12000000UL
#define BAUD 57600

//Will emulate hardware flow control (going to PC)
#define UART_DDR	DDRD
#define UART_PORT	PORTD
#define UART_RTS	PORTD2

#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

#include "Command.h"
#include "buffer.h"
#include "SPIemu.h"
#include "UART.h"


BUFFER buf;

//displays hex values for data received
void hexsend(int8_t data) {
	const char *hexdig = "0123456789ABCDEF";
	UART_send(hexdig[(data>>4)&0xF]);
	UART_send(hexdig[data&0xF]);
}

//function handles UART data received
//This also handles any commands such as buffer reset or buffer status
void recv_handler(uint16_t data) {
	if (cmd.echoUART == 2) {
		hexsend(data>>8);
		hexsend(data);
	} else if (cmd.echoUART == 1) {
		UART_send(data>>8);
		UART_send(data);
	}
	
	switch ((data>>12)&0xF) //command switch, used to change details before buffer is processed
	{
		default:
			{ //brackets are for temp scope only
			int8_t temp;
			temp = buf_insert(&buf, &data); //push onto buffer
			if (temp <= 0) {
				UART_PORT |= (1<<UART_RTS); //Buffer is full so set RTS, this will most likely still have data on the line
				if(temp < 0) {
					if (cmd.bufFull)
						UART_send(cmd.bufFull); //if full notify
					} else {
						if (cmd.bufoverflow)
							UART_send(cmd.bufoverflow); //if over full notify, data has been lost
					}
				}
				cli(); //this compensates for posiable data on the line while the last send occurred
				if (((uint8_t)buf.read == (uint8_t)(buf.write+2))|
					((uint8_t)buf.read == (uint8_t)(buf.write+3))|
					((uint8_t)buf.read == (uint8_t)(buf.write+4)))
					UART_PORT |= (1<<UART_RTS); //near full so set RTS
				sei();
			}
			break;
		
		case 0xD: //reset buffer
			buf_init(&buf);
			break;
			
		case 0xE: //buffer status - returns buffer position
			{
				uint8_t bufpos = buf_fill(&buf);
				if (((data>>8)&0xF) == 0) { //used second digit for display of results
					hexsend(bufpos); //current position in hex
				} else {
					UART_send(bufpos); //current position raw output
				}
			}
			break;
			
		case 0xF: //change command data
			switch ((data>>8)&0xF)
			{
				case 0: //Change buf empty character
					cmd_save_bufEmpty(data&0xFF);
					break;
				case 1: //Change buf underflow character
					cmd_save_bufunderflow(data&0xFF);
					break;
				case 2: //change buffer full character
					cmd_save_bufFull(data&0xFF);
					break;
				case 3: //change buffer overflow character
					cmd_save_bufoverflow(data&0xFF);
					break;
				
				case 10: //set UART echo
					cmd_save_echoUART(data&0xFF);
					break;
				case 11: //set SPI echo
					cmd_save_echoSPI(data&0xFF);
					break;
			}
			break;
	}
}

//Wrapper to prevent SPI data from being sent out of range
//note removes first 4 bytes off of data
void SPI_send_wrap(int16_t data) {
	//set data header
	data = (data & 0x0FFF) | 0x3000; 
	
	//limit data range
	if (data > 0x3F3C)
	data = 0x3F3C;
	else if (data < 0x347C)
	data = 0x347C;

	SPI_send(data);
	
	if (cmd.echoSPI == 1) {
		UART_send(data>>8);
		UART_send(data);
	}
	else if (cmd.echoSPI == 2)
	{
		hexsend(data>>8);
		hexsend(data);
	}
}


//counter calibrated for 1ms interrupts, with 12MHz clock with a 256 precaler
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK) {
//ISR(SIG_OUTPUT_COMPARE0A, ISR_NOBLOCK) {
	uint16_t temp;
	int8_t rtemp;
		
//	static uint16_t counter = 12; //calibrated based on scope output
//	if (--counter) //counter counts down
//		return;
//	counter = 12; //counter reset
	
	
	rtemp = buf_remove(&buf, &temp);
	if (rtemp) {
		cli(); //this compensates for possiable data on the line while the last send occurred
		if (!(((uint8_t)buf.read == (uint8_t)(buf.write+2))|
			  ((uint8_t)buf.read == (uint8_t)(buf.write+3))|
			  ((uint8_t)buf.read == (uint8_t)(buf.write+4))))
			UART_PORT &= ~(1<<UART_RTS); //this is for CTS, sets CTS line low when buffer is not full
		sei();
		SPI_send_wrap(temp);
	}

	//If empty
	if (rtemp < 0) {
		if (cmd.bufEmpty)
			UART_send(cmd.bufEmpty); //if empty notify
		} else if (rtemp == 0) {
			if (cmd.bufunderflow)
				UART_send(cmd.bufunderflow); //if empty notify
	}
}

int main(void) {
	cmd_loadall();
	buf_init(&buf);
	SPI_init();
	UART_init(
		UART_calcUBRR(F_CPU,BAUD),
		&recv_handler
	);
	
	sei();
	
	UART_DDR |= (1<<UART_RTS);   //set RTS for output 
	UART_PORT &= ~(1<<UART_RTS); //start with RTS line clear
	
	//setup timer note this is for 12MHz operation only on Atmega328 series
	TIMSK1 = (1<<OCIE1A);
	OCR1A = 1499;
	
	TCCR1A = 0;
	TCCR1B = (1<<CS11)|(1<<WGM12);
	
    while(1) {
//		if (!(PIND & (1<<UART_RTS)))
//			recv_handler(buf_fill(&buf));
		;
		// _delay_ms(1);
	}
}
