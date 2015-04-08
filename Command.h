/*
 * Command.h
 *
 * Created: 2015-04-03 18:23:44
 *  Author: Paul
 */

#include <avr/eeprom.h>

#ifndef COMMAND_H_
#define COMMAND_H_

typedef struct DBGCOMMAND
{
	int8_t echoUART; //echo UART Input (0 - disabled, 1 - raw, 2 -hex)
	int8_t echoSPI; //echo SPI Output (0 - disabled, 1 - raw, 2 -hex)
	
	//Error report chars (0 == disabled)
	char bufFull;  //buffer full report character
	char bufEmpty; //buffer empty report character
	char bufoverflow; //buffer has overflowed report character
	char bufunderflow; //buffer has underflowed report character
} DBGCOMMAND;

DBGCOMMAND cmd;

/*
 * cmd_loadall()
 *
 * no inputs
 * returns void
 *
 * loads all debug command variables from EEPROM
 */
void cmd_loadall();


/*
 * cmd_save_XXX()
 *
 * in int8_t val - replaces current value
 *
 * All command save functions behave the same
 * Saves val to EEPROM and updates current cmd struct
 */
void cmd_save_echoUART(int8_t val);
void cmd_save_echoSPI(int8_t val);
void cmd_save_bufFull(int8_t val);
void cmd_save_bufEmpty(int8_t val);
void cmd_save_bufoverflow(int8_t val);
void cmd_save_bufunderflow(int8_t val);



#endif /* COMMAND_H_ */
