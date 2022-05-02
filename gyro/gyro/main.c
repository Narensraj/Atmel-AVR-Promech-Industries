/*
 * gyro.c
 *
 * Created: 21-09-2020 2.59.05 PM
 * Author : Naren
 */ 

#define F_CPU 8000000UL		
#include <avr/io.h>		
#include <util/delay.h>		
#include <inttypes.h>		
#include <stdlib.h>		
#include <stdio.h>		
#include "MPU6050_res_define.h"	
#include "I2C_Master_H_file.h"	
#include "USART_RS232_H_file.h"	
uint16_t peakVal_Result;
volatile uint8_t peakVal_Timeout;
float Acc_x,Acc_y,Acc_z,Gyro_x,Gyro_y,Gyro_z,dis_x,dis_y,dis_z;

void Gyro_Init()		
{
	_delay_ms(150);		
	I2C_Start_Wait(0xD0);
	I2C_Write(SMPLRT_DIV);
	I2C_Write(0x07);	
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);
	I2C_Write(0x01);	
	I2C_Stop(); 

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);	
	I2C_Write(0x00);	
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);
	I2C_Write(0x18);	
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);	
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);	
	I2C_Write(ACCEL_XOUT_H);
	I2C_Repeated_Start(0xD1);
}
void Read_RawValue()
{
	MPU_Start_Loc();									
	Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	dis_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	dis_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	dis_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}
uint8_t peakDetector(uint32_t value)
{
	static uint32_t peakVal;
	static uint16_t counter;
	++counter;
	if(value > peakVal)
	{
		peakVal = value;
	}

	if(peakVal_Timeout == 0)
	{
		nofSamples = counter;
		peakVal_Result = peakVal;
		counter = 0;
		peakVal_Timeout = 1;
		peakVal = 0;
		return 1;
	}

	return 0;
}

if(peakVal_Timeout != COUNTER_DEFAULT_255)
{
	if(peakVal_Timeout != 0)
	{
		--peakVal_Timeout;
	}
}
int main(void)
{
		   char buffer[20], float_[10];
		   float Xa,Ya,Za;
		   float Xg=0,Yg=0,Zg=0;
		   I2C_Init();		
		   Gyro_Init();		
		   USART_Init(9600);	
		   
		   while(1)
		   {
			   Read_RawValue();
			
			 
			   Xa = Acc_x/16384.0;
			   Ya = Acc_y/16384.0;
			   Za = Acc_z/16384.0; 
			   
			   Xg = Gyro_x/16.4;
			   Yg = Gyro_y/16.4;
			   Zg = Gyro_z/16.4; 

			   dtostrf( Xa, 3, 2, float_ );
			   sprintf(buffer, " Ax = %s g\t",float_);
			   USART_SendString(buffer);
			   // _delay_ms(500);

			   dtostrf( Ya, 3, 2, float_ );
			   sprintf(buffer,"Ay = %s g\t",float_);
			   USART_SendString(buffer);
			   // _delay_ms(500);
			   
			   dtostrf( Za, 3, 2, float_ );
			   sprintf(buffer,"Az = %s g\t\r\n",float_);
			   USART_SendString(buffer);
			   // _delay_ms(500);

			   dtostrf( Xg, 3, 2, float_ );
			   sprintf(buffer,"Gx = %s%c/s\t",float_,0xF8);
			   USART_SendString(buffer);
			  // _delay_ms(500);

			   dtostrf( Yg, 3, 2, float_ );
			   sprintf(buffer,"Gy = %s%c/s\t",float_,0xF8);
			   USART_SendString(buffer);
			  // _delay_ms(500);
			   
			   dtostrf( Zg, 3, 2, float_ );
			   sprintf(buffer,"Gz = %s%c/s\r\n",float_,0xF8);
			   USART_SendString(buffer);
			   //_delay_ms(500);

	}
	
}









