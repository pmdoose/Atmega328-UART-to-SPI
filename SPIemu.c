/*
 * SPIemu.c
 *
 * Created: 2015-04-05 15:41:12
 *  Author: Paul Doose
 */ 

#include "SPIemu.h"

inline void SPI_init() {
	SPI_DDR |= (1<<SPI_CS) | (1<<SPI_SCK) | (1<<SPI_DATA);
	SPI_PORT |= (1<<SPI_CS);
	SPI_PORT &= ~((1<<SPI_SCK) | (1<<SPI_DATA));
}

inline int16_t SPI_bitreverse(int16_t org) {
	register int16_t rev = org;
	rev = ((rev & 0xFF00)>>8)|((rev & 0x00FF)<<8);
	rev = ((rev & 0xF0F0)>>4)|((rev & 0x0F0F)<<4);
	rev = ((rev & 0xCCCC)>>2)|((rev & 0x3333)<<2);
	rev = ((rev & 0xAAAA)>>1)|((rev & 0x5555)<<1);
	return rev;
}

void SPI_send(int16_t data) {
	SPI_PORT &= ~(1<<SPI_CS); //CS low

	//bit 15
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>15)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 14
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>14)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 13
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>13)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 12
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>12)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 11
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>11)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 10
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>10)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 9
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>9)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 8
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>8)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 7
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>7)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 6
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>6)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 5
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>5)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 4
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>4)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 3
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>3)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 2
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>2)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 1
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data>>1)&1)<<SPI_DATA);	//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	//bit 0
	SPI_PORT = (SPI_PORT&(~((1<<SPI_DATA)|(1<<SPI_SCK))))|(((data)&1)<<SPI_DATA);		//SCK low and set data
	SPI_PORT |= (1<<SPI_SCK); //SCK high
	
	SPI_PORT |= (1<<SPI_CS); //CS high
}
