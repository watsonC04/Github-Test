/*
 * Lab 5.c
 *
 * Created: 12/03/2025 15:52:31
 * Author : Chamal
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "my_I2C.h"
#include "BMI270.h"
#include "BMI270_reg.h"

int main(void)
{
	//ACC variables
	unsigned char accn_x_MSB, accn_x_LSB, accn_y_MSB, accn_y_LSB, accn_z_MSB, accn_z_LSB;
	unsigned char gyr_x_MSB, gyr_x_LSB, gyr_y_MSB, gyr_y_LSB, gyr_z_MSB, gyr_z_LSB;
	unsigned short data_sample[6];
	unsigned char samples_saved = 1, samples_to_save = 100;
 	
	//Configure LEDs
    DDRD|=(1<<3);
	DDRD|=(1<<4);
	DDRD|=(1<<5);
	DDRD|=(1<<6);
	
	//Initialize I2C communication
	I2C_MasterInit();
	//Initialize BMI270
	BMI270_init();
	//Configure BMI270
	BMI270_config();
	//Configure SD card & create file with header
	init_sd_card();

	//Obtain ACC values
	while(samples_saved <= samples_to_save){
		//Read ACC values
		accn_x_LSB = I2C_MasterReceiveByte(BMI270_address,ACC_X_LSB_address);
		accn_x_MSB = I2C_MasterReceiveByte(BMI270_address,ACC_X_MSB_address);
		accn_y_LSB = I2C_MasterReceiveByte(BMI270_address,ACC_Y_LSB_address);
		accn_y_MSB = I2C_MasterReceiveByte(BMI270_address,ACC_Y_MSB_address);
		accn_z_LSB = I2C_MasterReceiveByte(BMI270_address,ACC_Z_LSB_address);
		accn_z_MSB = I2C_MasterReceiveByte(BMI270_address,ACC_Z_MSB_address);
		//Read GYR values
		gyr_x_LSB = I2C_MasterReceiveByte(BMI270_address,GYR_X_LSB_address);
		gyr_x_MSB = I2C_MasterReceiveByte(BMI270_address,GYR_X_MSB_address);
		gyr_y_LSB = I2C_MasterReceiveByte(BMI270_address,GYR_Y_LSB_address);
		gyr_y_MSB = I2C_MasterReceiveByte(BMI270_address,GYR_Y_MSB_address);
		gyr_z_LSB = I2C_MasterReceiveByte(BMI270_address,GYR_Z_LSB_address);
		gyr_z_MSB = I2C_MasterReceiveByte(BMI270_address,GYR_Z_MSB_address);
		
		//Combine 8 bit MSB & LSB values to give 16 bit sample
		data_sample[0] = (accn_x_MSB<<8)|accn_x_LSB;
		data_sample[1] = (accn_y_MSB<<8)|accn_y_LSB;
		data_sample[2] = (accn_z_MSB<<8)|accn_z_LSB;
		data_sample[3] = (gyr_x_MSB<<8)|gyr_x_LSB;
		data_sample[4] = (gyr_y_MSB<<8)|gyr_y_LSB;
		data_sample[5] = (gyr_z_MSB<<8)|gyr_z_LSB;

		//Signal tilt z direction using green LED
		if((data_sample[2]&0x8000)==0x8000){
			PORTD|=1<<PD6; //Green LED on if sample neg.
		}
		else{
			PORTD&=~(1<<PD6);
		}
		//Turn off red LED at start of SD card write operation
		PORTD&=~(1<<PD5);
		
		//Write IMU data to SD Card
		SD_sample_write(&data_sample);
		
		samples_saved++;
	}
}



