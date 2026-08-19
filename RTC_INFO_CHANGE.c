#include <LPC21xx.h>
#include "officer_interrupt.h"
#include "kpm.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"
unsigned  int  read__key__blocking(void)
{
    unsigned int key;

    while(1)
    {
        key = key__scan();     

        if(key >= 1 && key <= 8)
            return key;      
    }
}

void  SetRTCHour( unsigned int hh)
{
	HOUR = hh;
}
 void SetRTCMin(unsigned int mm)
{
	  MIN = mm;
}
void setRTC_sec(unsigned int ss)
{
	SEC = ss;
}
 void SetRTCDate(unsigned int dd)
 {
	 DOM= dd;
 }
 void  SetRTCMonth(unsigned int mo)
 {
	 MONTH = mo;
 }
 void SetRTCYear(unsigned int yy)
 {
	 YEAR = yy;
 }
   void SetRTC_Day(unsigned int  dow)
	 {
		  DOW = dow;
	 }


void RTC_Info_CHANGE(void)
{
    unsigned int  hh, mm, ss, dd, mo, yy, dow;
    unsigned int choice, temp;
    
    while(1)
    {
        cmdLCD(CLEAR_LCD);
        strLCD("1H 2M 3S 4Dt");
        cmdLCD(GOTO_LINE2_POS0);
        strLCD("5Mo 6Y 7Dy 8Ex");
        delay_ms(200);

        choice = read__key__blocking();   

        switch(choice)
        {
            
            case 1:
                while(1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("SET HOUR(0-23):");
									   delay_ms(200);
                    cmdLCD(GOTO_LINE2_POS0);
                    hh = Read_TwoDigits();

                    if(hh <= 23)
                    {
                        SetRTCHour(hh);
											  strLCD("..HOUR UPDATED");
											   delay_ms(1000);
                        break;
                    }
                    else
                    {
                        cmdLCD(CLEAR_LCD);
                        strLCD("INVALID HOUR");
                        delay_s(2);
                    }
                }
                break;

         
            case 2:
                while(1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("SET MIN(0-59):");
									   delay_ms(200);//to avoid fetching garbage key value 
                    cmdLCD(GOTO_LINE2_POS0);
                    mm =Read_TwoDigits( );

                    if(mm <= 59)
                    {
                        SetRTCMin(mm);
											 strLCD("..MIN UPDATED");
											   delay_ms(1500);
                        break;
                    }
                    else
                    {
                        cmdLCD(CLEAR_LCD);
                        strLCD("INVALID MIN");
                        delay_s(1);
                    }
                }
                break;

       
            case 3:
                while(1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("SET SEC(0-59):");
									   delay_ms(200);
                    cmdLCD(GOTO_LINE2_POS0);
                    ss = Read_TwoDigits();

                    if(ss <= 59)
                    {
                        setRTC_sec(ss);
											 strLCD("..SEC'S UPDATED");
											   delay_ms(1000);
                        break;
                    }
                    else
                    {
                        cmdLCD(CLEAR_LCD);
                        strLCD("INVALID SEC");
                        delay_s(1);
                    }
                }
                break;

        
            case 4:
                while(1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("SET DATE(1-31):");
									  delay_ms(200);
                    cmdLCD(GOTO_LINE2_POS0);
                    dd = Read_TwoDigits();
									   if(dd ==00)
										 {
											 goto invalid_date;
										 }

                    if(dd >= 1 && dd <= 31)
                    {
                        SetRTCDate(dd);
											  cmdLCD(0x01);
											   strLCD("DATE UPDATED...");
											    delay_ms(1000);
                        break;
                    }
                    else
                    {
invalid_date:          cmdLCD(CLEAR_LCD);
                        strLCD("INVALID DATE");
                        delay_s(1);
                    }
                }
                break;

        
            case 5:
                while(1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("SET MONTH(1-12):");
									  delay_ms(200);
                    cmdLCD(GOTO_LINE2_POS0);
                    mo =Read_TwoDigits();

                    if(mo >= 1 && mo <= 12)
                    {
                        SetRTCMonth(mo);
                        break;
                    }
                    else
                    {
                        cmdLCD(CLEAR_LCD);
                        strLCD("INVALID MONTH");
                        delay_s(1);
                    }
                }
                break;

        
            case 6:
                while(1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("YEAR(YYYY):");
									  delay_ms(150);
                    cmdLCD(GOTO_LINE2_POS0);
                   yy = Read_FourDigits();

                    if(yy >= 2000 && yy <= 2099)
                    {
                        SetRTCYear(yy);
											  cmdLCD(0x01);
											  strLCD("UPDATED YEAR");
											  delay_ms(1000);
                        break;
                    }
                    else
                    {
                        cmdLCD(CLEAR_LCD);
                        strLCD("INVALID YEAR");
                        delay_s(1);
                    }
                }
                break;

            
            case 7:
                while(1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("SET DOW(0-6):");
									  delay_ms(200);
                    cmdLCD(GOTO_LINE2_POS0);
                    dow =Read_TwoDigits();

                    if(dow <= 6)
                    {
                        SetRTC_Day(dow);
                        break;
                    }
                    else
                    {
                        cmdLCD(CLEAR_LCD);
                        strLCD("INVALID DOW");
                        delay_s(1);
                    }
                }
                break;

        
            case 8:
                cmdLCD(CLEAR_LCD);
                strLCD("EXITING...");
                delay_s(1);
						cmdLCD(CLEAR_LCD);
                return;

        
            default:
                cmdLCD(CLEAR_LCD);
                strLCD("INVALID OPTION");
                delay_s(1);
                break;
        }
    }
}