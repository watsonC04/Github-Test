/*
 * SDcard.c
 *
 * Created: 07/11/2024 14:16:46
 *  Author: Simon Pope
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "diskio.h"
#include "ff.h"

FATFS fs;		/* Filesystem object for each logical drive */
FIL fp;

void init_sd_card(void)
{
	DSTATUS status;
	BYTE init_complete = 0;
	UINT bw;
	FRESULT rc;
	BYTE buff[64];

	/* Mount volume */
	rc = f_mount(&fs,"",0);
	if (rc == FR_OK){
		// Yellow LED on if mounted
			
		PORTD|=1<<PD4;
		_delay_ms(1000);
		PORTD&=~(1<<PD4);
		_delay_ms(1000);
	}


	/* Open file */
	rc = f_open(&fp,"rocketdata.txt", FA_WRITE | FA_CREATE_ALWAYS);
	if (rc == FR_OK){
		// Yellow LED on if opened
	
		PORTD|=1<<PD4;
		_delay_ms(1000);
		PORTD&=~(1<<PD4);
		_delay_ms(1000);
	}
	
	// Write file header
	rc = f_write(&fp, "Columns are accn_x accn_y accn_z gyro_x gyro_y gyro_z\n\n", 55, &bw);
	
	if (rc == FR_OK) {
		// Yellow LED on if data written
		PORTD|=1<<PD4;
	}	
	
	// Close file to ensure data is saved
	rc = f_close(&fp);
}

void SD_sample_write(unsigned short *samples){

	UINT bw;
	FRESULT rc;
	BYTE buff[64];
		
	// Open file
	rc = f_open(&fp,"rocketdata.txt", FA_WRITE | FA_OPEN_ALWAYS);
	
	// Format data to send to file on SD card
	rc = sprintf(buff, "%04x %04x %04x %04x %04x %04x\n", samples[0], samples[1], samples[2], samples[3], samples[4], samples[5]);
	
	// Find last line of file
	rc = f_lseek(&fp, f_size(&fp));
	
	// Write data to file
	rc = f_write(&fp, &buff, 30, &bw);
	
	// Red LED on if data written successfully
	if(rc == FR_OK){
		PORTD|=(1<<PD5);
	}
	
	// Close file to ensure data is saved	
	rc = f_close(&fp);
		
}

DWORD get_fattime (void)
{
	
	/* Pack date and time into a DWORD variable */
	return	 0;
}