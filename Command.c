/*
 * Command.c
 *
 * Created: 2015-04-06 02:51:58
 *  Author: Paul
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>

#include "Command.h"

uint8_t  EEMEM EEechoUART = 0;
uint8_t  EEMEM EEechoSPI = 0;
uint8_t  EEMEM EEbufFull = '>';
uint8_t  EEMEM EEbufEmpty = '<';
uint8_t  EEMEM EEbufoverflow = 'O';
uint8_t  EEMEM bufunderflow = 0;

inline void cmd_loadall() {
	cmd.echoUART = eeprom_read_byte(&EEechoUART);
	cmd.echoSPI = eeprom_read_byte(&EEechoSPI);
	cmd.bufFull = eeprom_read_byte(&EEbufFull);
	cmd.bufEmpty = eeprom_read_byte(&EEbufEmpty);
	cmd.bufoverflow = eeprom_read_byte(&EEbufoverflow);
	cmd.bufunderflow = eeprom_read_byte(&bufunderflow);
}

inline void cmd_save_echoUART(int8_t val) {
	eeprom_write_byte(&EEechoUART, val);
	cmd.echoUART = val;
}

inline void cmd_save_echoSPI(int8_t val) {
	eeprom_write_byte(&EEechoSPI, val);
	cmd.echoSPI = val;
}

inline void cmd_save_bufFull(int8_t val) {
	eeprom_write_byte(&EEbufFull, val);
	cmd.bufFull = val;
}

inline void cmd_save_bufEmpty(int8_t val) {
	eeprom_write_byte(&EEbufEmpty, val);
	cmd.bufEmpty = val;
}

inline void cmd_save_bufoverflow(int8_t val) {
	eeprom_write_byte(&EEbufoverflow, val);
	cmd.bufoverflow = val;
}

inline void cmd_save_bufunderflow(int8_t val) {
	eeprom_write_byte(&bufunderflow, val);
	cmd.bufunderflow = val;
}
