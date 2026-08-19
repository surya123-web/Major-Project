#include "arduino_defines.h"
#include "arduino.h"
#include "lcd_defines.h"
#include "delay.h"
#include "i2c_defines.h"
#include "i2c__eeprom.h"


    void writeLCD(unsigned char byte)
                {
                //select write operations
                         digitalwrite(LCD_RW,0);//............
                         //write any byte onto the data pins (d0-d7)
                         write2pins(LCD_DATA,8,byte);
                         //providing high to low pulse for latching
                        digitalwrite(LCD_EN,1);
                         delay_us(1);
                        digitalwrite(LCD_EN,0);
                        //byte between byte time
                        delay_ms(2);
                }

                void cmdLCD(unsigned char cmdByte)
                {
                    //select command register
                        digitalwrite(LCD_RS,0);
                        //write to  cmd register viadata pins
                        writeLCD(cmdByte);
                }

 void Init_LCD(void)
{
        portMode(LCD_DATA,11,1);
        delay_ms(15);
        cmdLCD(MODE_8BIT_1LINE);
        delay_us(4100);
        cmdLCD(MODE_8BIT_1LINE);
        delay_us(100);
        cmdLCD(MODE_8BIT_1LINE);
        cmdLCD(MODE_8BIT_2LINE);
        cmdLCD(DSP_ON_CUR_OFF);
        cmdLCD(CLEAR_LCD);
        cmdLCD(SHIFT_CUR_RIGHT);

}

 void  charLCD(unsigned char asciival)
                        {
                                digitalwrite(LCD_RS,1);
                                writeLCD(asciival);
                        }


  void strLCD (const char *p)
        {
                while(*p)
                        charLCD(*p++);
        }


                void U32LCD(unsigned int n)
        {
                unsigned char digit[10];
                int i=0;
                if(n==0)
                {
                        charLCD('0');
                }
                else
                {
                        while(n>0)
                        {
                                digit[i++] = (n%10)+48;
                                n/=10;
                        }
                        for(--i;i>=0;i--)
                        {
                                charLCD(digit[i]);
                                }
                }
        }


        void s32LCD(signed int n)
        {
         if(n<0)
         { 
           charLCD('-');
           n= -n;
         }
         U32LCD(n);
        }
 void F32LCD(float fn,unsigned int nDP)
        {
           unsigned int n,i;
           if(fn<0.0)
           {
                   charLCD('-');
                   fn = -fn;
           }
           n=fn;
           U32LCD(n);
           charLCD('.');
           for(i=0;i<nDP;i++)
           {
           fn = (fn-n)*10;
           n=fn;
           charLCD(n+48);
           }
        }
		
				void BuildCGRAM(unsigned char *pattern, unsigned char slot)
{
    unsigned int i;

    if(slot > 7) return;          // CGRAM has only 0–7 slots

    // Point to required CGRAM slot
    cmdLCD(0x40 + (slot * 8));

    digitalwrite(LCD_RS, 1);   

    
    for(i = 0; i < 8; i++)
    {
        writeLCD(pattern[i]);
    }

    
    cmdLCD(GOTO_LINE1_POS0);
}

void Display_Party_Menu(void)
{
    const unsigned char partysymbol1[8] = {0x11,0x11,0x11,0x1F,0x01,0x01,0x1F,0x00};
    const unsigned char partysymbol2[8] = {0x1F,0x10,0x10,0x1F,0x11,0x11,0x11,0x00};
		
    const unsigned char partysymbol3[8] = {0x06,0x0F,0x1F,0x0F,0x07,0x03,0x01,0x00};
    const unsigned char partysymbol4[8] = {0x0C,0x1E,0x1F,0x1E,0x1C,0x18,0x10,0x00};
		
    const unsigned char partysymbol5[8] = {0x07,0x08,0x11,0x14,0x13,0x08,0x07,0x00};
    const unsigned char partysymbol6[8] = {0x1C,0x02,0x11,0x05,0x19,0x02,0x1C,0x00};
		
    const unsigned char partysymbol7[8] = {0x03,0x02,0x02,0x02,0x1E,0x1E,0x1E,0x00};
    const unsigned char partysymbol8[8] = {0x18,0x08,0x08,0x08,0x0F,0x0F,0x0F,0x00};

    cmdLCD(0x01);
    cmdLCD(DSP_ON_CUR_OFF);

    BuildCGRAM((unsigned char *)partysymbol1, 0);
    BuildCGRAM((unsigned char *)partysymbol2, 1);
    BuildCGRAM((unsigned char *)partysymbol3, 2);
    BuildCGRAM((unsigned char *)partysymbol4, 3);
    BuildCGRAM((unsigned char *)partysymbol5, 4);
    BuildCGRAM((unsigned char *)partysymbol6, 5);
    BuildCGRAM((unsigned char *)partysymbol7, 6);
    BuildCGRAM((unsigned char *)partysymbol8, 7);

    cmdLCD(0x80);

    cmdLCD(GOTO_LINE1_POS0);
    strLCD("PARTY A : ");
    charLCD(0);
    charLCD(1);

    cmdLCD(GOTO_LINE2_POS0);
    strLCD("PARTY B : ");
    charLCD(2);
    charLCD(3);

    cmdLCD(GOTO_LINE3_POS0);
    strLCD("PARTY C : ");
    charLCD(4);
    charLCD(5);

    cmdLCD(GOTO_LINE4_POS0);
    strLCD("PARTY D : ");
    charLCD(6);
    charLCD(7);
}
void Display_Party_Results(void)
{
    unsigned int temp;

    
    const unsigned char partysymbol1[8] = {0x11,0x11,0x11,0x1F,0x01,0x01,0x1F,0x00};
    const unsigned char partysymbol2[8] = {0x1F,0x10,0x10,0x1F,0x11,0x11,0x11,0x00};
    
    const unsigned char partysymbol3[8] = {0x06,0x0F,0x1F,0x0F,0x07,0x03,0x01,0x00};
    const unsigned char partysymbol4[8] = {0x0C,0x1E,0x1F,0x1E,0x1C,0x18,0x10,0x00};
    
    const unsigned char partysymbol5[8] = {0x07,0x08,0x11,0x14,0x13,0x08,0x07,0x00};
    const unsigned char partysymbol6[8] = {0x1C,0x02,0x11,0x05,0x19,0x02,0x1C,0x00};
    
    const unsigned char partysymbol7[8] = {0x03,0x02,0x02,0x02,0x1E,0x1E,0x1E,0x00};
    const unsigned char partysymbol8[8] = {0x18,0x08,0x08,0x08,0x0F,0x0F,0x0F,0x00};

   
    cmdLCD(0x01);
    cmdLCD(DSP_ON_CUR_OFF);

   
    BuildCGRAM((unsigned char*)partysymbol1, 0);
    BuildCGRAM((unsigned char*)partysymbol2, 1);
    BuildCGRAM((unsigned char*)partysymbol3, 2);
    BuildCGRAM((unsigned char*)partysymbol4, 3);
    BuildCGRAM((unsigned char*)partysymbol5, 4);
    BuildCGRAM((unsigned char*)partysymbol6, 5);
    BuildCGRAM((unsigned char*)partysymbol7, 6);
    BuildCGRAM((unsigned char*)partysymbol8, 7);

    
    cmdLCD(GOTO_LINE1_POS0);
    strLCD("PARTY ");
    charLCD(0); charLCD(1);
    strLCD(" : ");
    temp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x002F);
		if(temp>=10)
	  temp =0;
    U32LCD(temp);
    strLCD(" votes");

   
    cmdLCD(GOTO_LINE2_POS0);
    strLCD("PARTY ");
    charLCD(2); charLCD(3);
    strLCD(" : ");
    temp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0030);
		if(temp>=10)
	  temp =0;
    U32LCD(temp);
    strLCD(" votes");

   
    cmdLCD(GOTO_LINE3_POS0);
    strLCD("PARTY ");
    charLCD(4); charLCD(5);
    strLCD(" : ");
    temp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0031);
		if(temp>=10)
	  temp =0;
    U32LCD(temp);
    strLCD(" votes");

   
    cmdLCD(GOTO_LINE4_POS0);
    strLCD("PARTY ");
    charLCD(6); charLCD(7);
    strLCD(" : ");
    temp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0032);
	if(temp>=10)
	  temp =0;
    U32LCD(temp);
    strLCD(" votes");
}
