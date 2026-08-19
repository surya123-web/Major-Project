//i2c_eeprom.c
#include "i2c.h"
#include "delay.h"
#include "i2c_defines.h"
#include "i2c__eeprom.h"
unsigned int stored_password;
void EEPROM_Write_Default_Config(void)
{
    // Voting configuration values
    unsigned int voting_enable_flag = 1;   // 1 = enabled, 0 = disabled

    unsigned char start_hour = 10;
    unsigned char start_min  = 00;

    unsigned char end_hour   = 18;
    unsigned char end_min    = 00;

    // Write into EEPROM
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0000, voting_enable_flag);

    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0001, start_hour);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0002, start_min);

    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0003, end_hour);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0004, end_min);
}

void syncing_EEPROM_Write_Default_Config(void)
 {
  unsigned char val;

    val = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0000);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0000, val);

    val = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0001);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0001, val);

    val = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0002);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0002, val);

    val = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0003);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0003, val);

    val = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0004);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0004, val);
 }

void i2c_eeprom_bytewrite(unsigned char slaveAddr, unsigned short wBufAddr, unsigned char dat)
{
        //issue i2c start
        i2c_start();
        //slaveAddr+w, &take ack
        i2c_write(slaveAddr<<1);
        //wBuffAddr, ack
        i2c_write(wBufAddr>>8);
        i2c_write(wBufAddr);
        //dat, ack
        i2c_write(dat);
        //issue stop
        i2c_stop();
        //write cycle delay
        delay_ms(10);
}

unsigned char i2c_eeprom_randomread(unsigned char slaveAddr, unsigned short rBufAddr)
{
        unsigned char dat;
        //issue start
        i2c_start();
        //slaveAddr+w, &take ack
        i2c_write(slaveAddr<<1);
	      //wbuff addr and take ack
        i2c_write(rBufAddr>>8);
        i2c_write(rBufAddr);
        //restart
        i2c_restart();
        //slaveAddr+r, &take ack
        i2c_write(slaveAddr<<1|1);
        //read byte & provide nack
        dat = i2c_nack();
        //stop condition
        i2c_stop();
        return dat;
}
void i2c_eeprom_pagewrite(unsigned char slaveAddr, unsigned short wBufStartAddr, unsigned  char *p, unsigned char nBytes)
{
        unsigned char i;
        //issue start condition
        i2c_start();
        //slaveAddr+w, &take ack
        i2c_write(slaveAddr<<1);
        //wBufStartAddr, &take ack^M
        i2c_write(wBufStartAddr>>8);
        i2c_write(wBufStartAddr);
        for(i=0; i<nBytes; i++)
        {
                i2c_write(p[i]);
        }
        //issue stop condition
        i2c_stop();
        //delay for internal write-cycle i eeprom
        delay_ms(10);
}

void i2c_eeprom_seqread(unsigned char slaveAddr, unsigned short rBufStartAddr, unsigned char *p, unsigned char nBytes)
{
        unsigned char i;
        //issue start condition
        i2c_start();
        //slaveAddr+w, &take ack
        i2c_write(slaveAddr<<1);
        //rBufStartAddr, &take ack
        i2c_write(rBufStartAddr>>8);
        i2c_write(rBufStartAddr);
        //issue repeated start condition
        i2c_restart();
        //slave+r, &take ack
        i2c_write(slaveAddr<<1|1);
        for(i=0; i<nBytes-1; i++)
        {
                //read(n-1) Bytes, for each provide mack
                p[i] = i2c_mack();
        }
        //read nth (last) byte, &provide nack
        p[i] = i2c_nack();
        //issue stop condition
        i2c_stop();
			}

 void Write_Password_EEPROM(unsigned short addr, unsigned short password)
{
    
   i2c_eeprom_bytewrite(I2C_EEPROM_SA,addr, (unsigned char)(password & 0xFF));// writing  LSB
   
    
    i2c_eeprom_bytewrite(I2C_EEPROM_SA,addr + 1, (unsigned char)((password >> 8) & 0xFF));//writing MSB
  
}
  
void Read_prestored_password(unsigned short addr)
{
    stored_password  = i2c_eeprom_randomread(I2C_EEPROM_SA, addr);        // read LSB
    stored_password |= i2c_eeprom_randomread(I2C_EEPROM_SA, (addr+1)) << 8;   // read MSB
}






void EEPROM_Init_Votes(void)
{
    i2c_eeprom_bytewrite(I2C_EEPROM_SA,0x002F, 0);  // initial votes of party A
    i2c_eeprom_bytewrite(I2C_EEPROM_SA,0x0030, 0);  // initial  votes of party B
    i2c_eeprom_bytewrite(I2C_EEPROM_SA,0x0031, 0);  // initial votes of party C
    i2c_eeprom_bytewrite(I2C_EEPROM_SA,0x0032, 0);  // initial  votes of party D
}
