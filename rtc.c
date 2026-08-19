#include "rtc_defines.h"
#include "lpc21xx.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "rtc.h"
int hh,dd,dow,mm,mo,ss,yy;

const  char week[7][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
void Init_RTC(void)
{
	   //reseting the clock tick counter using CCR
	   CCR =RTC_RESET;
	   CCR = RTC_CLKSRC | RTC_ENABLE;;
	   //setting one second tick rate as per our requirement
	   PREINT = PREINT_VAL;
	   PREFRAC = PREFRAC_VAL;
	   //enabling the clock
	   CCR=RTC_ENABLE;
}

void set_time_data_day(void)
{
    cmdLCD(CLEAR_LCD);
    SetRTCTimeInfo(12,00, 00);
    SetRTCDateInfo(26, 11, 25);
    SetRTCDay(3);
}
void SetRTCTimeInfo(unsigned int hour, unsigned int minute,unsigned int second)
{
        HOUR = hour;
        MIN = minute;
        SEC = second;
}
void SetRTCDateInfo(unsigned int date,unsigned int month,unsigned int year)
{
        DOM = date;
        MONTH = month;
        YEAR = year;
}
void SetRTCDay(unsigned int day)
{
        DOW = day;
}

void  get_time_data_day_info(void)
{
	          
            GetRTCTimeInfo(&hh, &mm, &ss);
            GetRTCDateInfo(&dd, &mo, &yy);
            GetRTCDay(&dow);
}	
void GetRTCTimeInfo(signed int *hour, signed int*minute,signed int *second)
{
        *hour = HOUR;
        *minute = MIN;
        *second = SEC;
}
void GetRTCDateInfo(signed int *date,signed int *month, signed int *year)
{
        *date = DOM;
        *month = MONTH;
        *year = YEAR;
}
void GetRTCDay(signed int *day)
{
        *day = DOW;  
}
void disp_time_data_day_info(void )
 {
	        Display_RTC_Time(hh, mm, ss);
            DisplayRTCDay(dow);
            DisplayRTCDate(dd, mo,yy);
 }
 
void Display_RTC_Time(unsigned int hour,unsigned int minute,unsigned int second)
{
	              cmdLCD(GOTO_LINE1_POS0);
                charLCD((hour/10)+48);
                charLCD((hour%10)+48);
                charLCD(':');
                charLCD((minute/10)+48);
                charLCD((minute%10)+48);
                charLCD(':');
                charLCD((second/10)+48);
                charLCD((second%10)+48);
}
void DisplayRTCDay(unsigned int dow)
{
	      if(dow<7){
        cmdLCD(GOTO_LINE1_POS0+10);
        strLCD(week[dow]);
				}
}

void DisplayRTCDate(unsigned int date,unsigned int month,unsigned int year)
{
                cmdLCD(GOTO_LINE2_POS0);
                charLCD((date/10)+48);
                charLCD((date%10)+48);
                charLCD('/');
                charLCD((month/10)+48);
                charLCD((month%10)+48);
                charLCD('/');
                U32LCD(year);
}
