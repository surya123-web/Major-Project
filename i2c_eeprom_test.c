//imported one 
//i2c_eeprom_test.c

#include "i2c__eeprom.h"
#include "i2c_eeprom_defines.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"

signed char rbuff[16];
int main()
{
        char temp,temp1;
         unsigned int i;

        signed char wbuff[] = "ABCDEFIJKLMNOPQ";
        init_i2c();
        Init_LCD();

        strLCD("I2C_TEST");
        delay_s(1);
        cmdLCD(0xc0);
        i2c_eeprom_bytewrite(I2C_EEPROM_SA1, 0x0000, 'a');
        temp = i2c_eeprom_randomread(I2C_EEPROM_SA1, 0x0000);
        charLCD(temp);
	
	
        delay_ms(2000);
	
	
        i2c_eeprom_pagewrite(I2C_EEPROM_SA1, 0x0000, wbuff, 17);
        i2c_eeprom_seqread(I2C_EEPROM_SA1, 0x0000, rbuff, 17);
                cmdLCD(0x01);
                 delay_ms(2000);
                for(i=0;i<15;i++)
                {
                  temp1=i2c_eeprom_randomread(I2C_EEPROM_SA1, i);  //reading character by character
                  charLCD(temp1);
                   delay_ms(500);  
                }
        while(1);
}