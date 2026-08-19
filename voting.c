#include "rtc.h"
#include "i2c__eeprom.h"
#include "i2c_defines.h"
#include "delay.h"
#include <string.h>
#include "lcd_defines.h"
#include "lcd.h"

extern volatile unsigned char rfid_buf[11];


int i;

void Store_Predefined_Voters(void)//storing the ids of the voters priorly
{
	 unsigned char voter1_rfid[8] ={'1','2','5','3','2','4','1','8'};
     unsigned char voter2_rfid[8] = {'1','2','5','3','4','8','6','8'};
     unsigned char voter3_rfid[8] = {'1','2','6','1','1','3','9','8'};
     unsigned char voter4_rfid[8] = {'1','2','6','2','1','5','6','7'};
		 unsigned char officer_rfid[8] = {'0','0','2','2','1','2','3','4'};
    i2c_eeprom_pagewrite(I2C_EEPROM_SA, 0x0005, voter1_rfid,8);//slave addr,addre of data,string name (base address),length
    
    i2c_eeprom_pagewrite(I2C_EEPROM_SA, 0x000D, voter2_rfid,8);
   

    i2c_eeprom_pagewrite(I2C_EEPROM_SA, 0x0015, voter3_rfid,8);
    

    i2c_eeprom_pagewrite(I2C_EEPROM_SA, 0x001D, voter4_rfid, 8);
		 
		  i2c_eeprom_pagewrite(I2C_EEPROM_SA, 0x0025,officer_rfid,8);//officer  id card storage
    
}


int Is_Voting_Time_Valid(void)
{
    int current_hour, current_min, current_sec;

    // Reading voting start/end time from EEPROM
    int voting_start_hour = i2c_eeprom_randomread(I2C_EEPROM_SA,0x0001);//i2c protocol
    int voting_start_min  = i2c_eeprom_randomread(I2C_EEPROM_SA,0x0002);
    int voting_end_hour   = i2c_eeprom_randomread(I2C_EEPROM_SA,0x0003);
    int voting_end_min    = i2c_eeprom_randomread(I2C_EEPROM_SA,0x0004);
	  GetRTCTimeInfo(&current_hour, &current_min, &current_sec);// Get current RTC time,where seconds are ignored
 


    if (current_hour < voting_start_hour) return 0; //before starting hour
    if (current_hour > voting_end_hour)   return 0; //came after ending hour
    if (current_hour == voting_start_hour && current_min < voting_start_min) return 0; //before starting minute
    if (current_hour == voting_end_hour   && current_min > voting_end_min)   return 0; //after ending minute
		
    // Within voting period
    return 1;
}


//this checks whether voting has been enabled by authority or not yet
int Is_Voting_Started(void)
{
    int voting_enabled;

    // Reading  the voting enable flag from EEPROM
    voting_enabled = i2c_eeprom_randomread(I2C_EEPROM_SA,0x0000); 

    if(voting_enabled == 1)
        return 1; // voting is enabled by authority 
    else
        return 0; // voting not enabled by authority yet
}

//checking whether voter id is already recorded or not from the database
#define RFID_DIGITS 8
#define TOTAL_IDS 4
#define RFID_START_ADDR 0x0005  // starting EEPROM address for voter IDs

unsigned char Is_RFID_Present_In_EEPROM(unsigned char *rfid_buf)
{
    unsigned char fetched_id[9];
    unsigned int addr = 0x0005;
    unsigned char i;
	  unsigned char t;
	   unsigned int j=0,k;
   
    for(i = 0; i < TOTAL_IDS; i++)
    {
        j=0;   
          
			  
		     for(k=0;k<8;k++)
			  {
					   fetched_id[j++] =i2c_eeprom_randomread(I2C_EEPROM_SA,(addr+k));
					
			  }
		  
		     
        if(memcmp(fetched_id, &rfid_buf[1],8) == 0)
        {
            return i+1;  
        }
		   
       
        addr += 8;//it  points to next voter id starting address
    }

    return 0;  // No match found
}



#define RFID_DIGITS        8      
#define OFFICER_RFID_ADDR  0x0025
unsigned  char Is_officer_RFID_Present_In_EEPROM(  unsigned char *rfid_buf)
{
	    unsigned char fetched_id_from_eeprom[9];
	     unsigned int i;
	    for(i=0;i<8;i++)
	   {
			  fetched_id_from_eeprom[i] =i2c_eeprom_randomread(I2C_EEPROM_SA,(OFFICER_RFID_ADDR +i));
	   }
	   if(memcmp(fetched_id_from_eeprom, &rfid_buf[1],8) == 0)
        {
            return 1; //if officer card is valid  
        }
				else 
				{
					 return 0;  //if officer card is invalid 
				}
	   
}


