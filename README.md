# Atmega328-UART-to-SPI
Converts incoming UART signal to SPI signal which is output in 1ms intervals.

Currently set to 57600 Baud rate, requiring a 12MHz clock.

Everything is interrupt driven, so there are no synchronous wait times.

SPI output is handled by timer1 as I needed a 16bit counter for it, and other than minor deviations it appears to be highly accurate output.
Output to SPI is on exactly 1ms intervals (1,000Hz), this will require a recompile to adjust.
	to adjust this you need to set OCR1A to the following. F_CPU/(8x*Interval)-1 (Ex. for this project 12,000,000/(8x1,000) as I was using a 12MHz ceramic oscilator. 

UART is buffered on both input and output.
	Working flow control, on D2 pin. Can be handled by hardware or software if you read the pin value
	Note RTS was implemented only, does not currently have a CTS line.

There are a few special commands that may be helpful, these were used in debugging
These settings are saved to EEPROM, you will need to locate the EEPROM file in the complied directory (release/debug) and have to uploaded to the chip or you will get strange results.

0xFnxx - This sets the debug status commands, these will be saved in the EEPROM, so once set they will remain that way until you change them again.
	n - Selects which "command" or debug value is being set.
		n = 0x0 - xx = character for buffer empty (Set to zero to turn off)
		n = 0x1 - xx = character for buffer underflow, cant write anything more to SPI. (Set to zero to turn off)
		n = 0x2 - xx = character for buffer full
		n = 0x3 - xx = character for buffer overflow (cant store anything more data was lost)
		n = 0xA - xx = value for UART echo, 1 for raw output, 2 for hex output, all other values disable this feature.
		n = 0xB - xx = value for SPI echo through UART, 1 for raw output, 2 for hex output, all other values disable this feature.

0xEnxx - Returns buffer fill position, and buffer state if full or empty (can indicate errors)
	n = 0x0 - Output for fill position is in hex
	n = any non-Zero - Output for fill position is a raw byte

0xDxxx - Resets buffer - E.g. buffer will be empty after this, all data inside will be lost.

All other signals are interpreted as data, they will be masked using 0x0FFF, and will be sent directly to SPI output buffer.

Note 1: This version has output range filtering between 0x47C and 0x3F3C as a firend requested it for a specific project that required that the inputs fall within this range. (This is implemented in main before processing data sent through SPI, bufUARTtoSPI.c, SPI_send_wrap. (remove or coment out the 4 lines after the "limit data range" comment do disable.

Note 2: SPI receive was not implemented as the device that the data was being sent to did not send anything back, so it was not imperative to have this feature. This can be easily implemented by added a pin read and bit shift after each SCK high, in SPIemu.c, SPI_send.

Note 3: All of the debug status commands, are processed before input buffer, however any return values will be added to output buffer, so responses may be delayed, but the action will be immediate.
