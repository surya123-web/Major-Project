//i2c_periphearl.c

#include "i2c_defines.h"
#include <LPC21xx.h>

void init_i2c(void)
{
        //cfg p0.2, p0.3 for SCL, SDA functions using PINSEL0
        PINSEL0 |= SCL_0_2 | SDA_0_3;

        //cfg speed for I2C serial communication
        I2SCLH = I2C_DIVIDER;
        I2SCLL = I2C_DIVIDER;

        //I2C peripheral Enable for communication

        I2CONSET = 1<<I2EN_BIT;
}

void i2c_start(void)
{
        //start condition
        I2CONSET = 1<<STA_BIT;
        while(((I2CONSET>>SI_BIT)&1)==0);
        //clear start condition
        I2CONCLR = 1<<STAC_BIT;
}

void i2c_restart(void)
{
        //start condition
        I2CONSET = 1<<STA_BIT;
        //clear SI bit to initiate repeated start
        I2CONCLR = 1<<SIC_BIT;
        //wait for restart bit status

        while(((I2CONSET>>SI_BIT)&1)==0);

        //clear start condition
        I2CONCLR = 1<<STAC_BIT;
}

void i2c_stop(void)
{
        //stop condition
        I2CONSET = 1<<STO_BIT;
        //clear SIC bit to initiate stop
        I2CONCLR = 1<<SIC_BIT;
}

void i2c_write(unsigned char dat)
{
    
        I2DAT = dat;
        I2CONCLR = 1<<SIC_BIT; //wait for SI bit status, end of serlization
        while(((I2CONSET>>SI_BIT)&1)==0);
}

unsigned char i2c_nack(void)//concentrate on unsigned char
{
        //clr SIC_BIT initiates serlization
        I2CONCLR = 1<<SIC_BIT;
        //wait for SI BIT status, end of serlization
        while(((I2CONSET>>SI_BIT)&1)==0);
        return I2DAT;
}

unsigned char i2c_mack(void)//here too u8
{
        //cfg AA_BIT to issue mack
        I2CONSET = AA_BIT;
        //clr SIC_BIT intlizes serlization in
        I2CONCLR = 1<<SIC_BIT;
        //wait for SI bit status end of serlization
        while(((I2CONSET>>SI_BIT)&1)==0);
        //clear AA_BIT
        I2CONCLR = 1<<AA_BIT;
        return I2DAT;
}
