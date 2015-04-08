/*
 * SPIemu.h
 *
 * Created: 2015-04-05 15:41:54
 *  Author: Paul Doose
 */ 

#include <avr/io.h>

#ifndef SPIEMU_H_
#define SPIEMU_H_

#define SPI_DDR		DDRC
#define SPI_PORT	PORTC

#define SPI_CS		PORTC2
#define SPI_SCK		PORTC1
#define SPI_DATA	PORTC0

/*
 * SPI_init()
 *
 * return null
 *
 * init port for SPI data send
 */
void SPI_init();

/*
 * SPI_bitreverse()
 *
 * in int16_t bits - bits to be reversed
 *
 * return input with bits reversed
 *
 * bit order reversal, was intended to speed up data transfer but tests
 * showed that this offered no optimization over current code
 * currently not used
 */
int16_t SPI_bitreverse(int16_t bits);

/*
 * SPI_send()
 *
 * in int16_t data - data to be sent
 *
 * return void
 *
 * Sends 16 bits of data using SPI, pins used are defined above
 */
void SPI_send(int16_t data);

#endif /* SPIEMU_H_ */
