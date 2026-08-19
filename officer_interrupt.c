#include <lpc21xx.h>
#include "officer_interrupt.h"
#include "kpm.h"
#include "lcd_defines.h"
#include "i2c_defines.h"
#include "i2c__eeprom.h"
#include "lcd.h"
#include "delay.h"
extern  unsigned int time_value;
 unsigned int party_A_value ;
unsigned int party_B_value;
 unsigned int party_C_value;
unsigned int party_D_value;

volatile unsigned int OFFICER_INTERRUPT_SWITCH_DETECTED=0;
volatile unsigned char officer_abort_flag = 0;

void Init_interrupt(void)
{
    PINSEL0 |= 0x0000C000;    // P0.7 as EINT2
   

    EXTMODE  |= (1<<2);      
    EXTPOLAR &= ~(1<<2);  
	
	 
    VICIntEnable = 1<<16;    
    VICIntSelect &= ~(1<<16);

    VICVectCntl1 = 0x20 | 16; 
    VICVectAddr1 = (unsigned int)EINT2_isr;
}

void EINT2_isr(void) __irq
{
    OFFICER_INTERRUPT_SWITCH_DETECTED = 1;
   

    EXTINT = (1<<2); 
    VICVectAddr = 0;
}

	void set_timings_of_voting(void)
	{
	        unsigned int key;
			unsigned int  start_hour,start_min,end_hour,end_min;
			cmdLCD(0x01);
			strLCD("DEAR SIR,");
			cmdLCD(GOTO_LINE2_POS0);
			strLCD("SELECT OPTION:");
			cmdLCD(GOTO_LINE3_POS0); 
			strLCD("1: SET TIME");
			cmdLCD(GOTO_LINE4_POS0);
			strLCD("2:DISPLAY SET TIME");
				  key = keyscan();
				   	delay_ms(200);
				  if(key == '1')
				  {
				   cmdLCD(0x01);
		re_enter_starting_hour:	strLCD("ENTER STARTING HOUR:");																
	                      	time_value =  Read_TwoDigits();
		                                  if(time_value >23)
					{
							 cmdLCD(0x01);
						 strLCD("INVALID HOUR ENTERED");
							 delay_ms(1000);
						  cmdLCD(0x01);
						  goto re_enter_starting_hour;
			   	}
		           
				   i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0001,time_value);
				  
		      time_value =0;
				  cmdLCD(0x01);
		 strLCD("VOTING STARTING ");
		 cmdLCD(GOTO_LINE2_POS0);
		 strLCD("HOUR UPDATED..");
		 delay_s(2);
																	   
		 cmdLCD(0x01);
	re_enter_starting_min:	strLCD("ENTER STARTING MIN:");	
		time_value = Read_TwoDigits(); 
				if(time_value >59)
			{
			 cmdLCD(0x01);
		 strLCD("INVALID MIN ENTERED");
				 delay_ms(1500);
				  cmdLCD(0x01);
			  goto re_enter_starting_min;
			}
		                
		  i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0002,time_value);

		  time_value =0; //for safer purpose without getting garbage
			   cmdLCD(0x01);
		strLCD("VOTING STARTING ");
			 cmdLCD(GOTO_LINE2_POS0);
			 strLCD("MINUTE UPDATED..");
				 delay_s(1);
																	
																	    cmdLCD(0x01);
							re_enter_ending_hour:strLCD("ENTER ENDING HOUR:");	
																	    time_value = Read_TwoDigits();
																			if(time_value >23)
																			{
																				 cmdLCD(0x01);
																				 strLCD("INVALID HOUR ENTERED");
																				 delay_ms(1500);
																				  cmdLCD(0x01);
																				  goto re_enter_ending_hour;
																			}
																			
																	    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0003,time_value);
																				time_value =0;
																			cmdLCD(0x01);
																    	strLCD("VOTING ENDING ");
																	    cmdLCD(GOTO_LINE2_POS0);
																	    strLCD("HOUR UPDATED..");
																	    delay_s(1);
																			
																			cmdLCD(0x01);
						re_enter_ending_min:     strLCD("ENTER ENDIN MIN:");	
																	    time_value = Read_TwoDigits();
																			
																			
																			if(time_value >59)
																			{
																				 cmdLCD(0x01);
																				 strLCD("INVALID MIN ENTERED");
																				 delay_ms(1500);
																				  cmdLCD(0x01);
																				  goto re_enter_ending_min;
																			}
																	     i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0004,time_value);
																			time_value =0;
																			cmdLCD(0x01);
																    	strLCD("VOTING ENDING ");
																	    cmdLCD(GOTO_LINE2_POS0);
																	    strLCD("MIN UPDATED..");
																	    delay_s(1);
																		}	
																		else if(key == '2')
																		{
																			 start_hour = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0001);
																			 start_min = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0002);
																			 end_hour = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0003);
																			 end_min = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0004);
																		         cmdLCD(0x01);
																				 strLCD("CURRENT VOTE TIMINGS");
																				 cmdLCD(GOTO_LINE2_POS0);
																				 strLCD("STARTING TIME:");
																				
																				 charLCD((start_hour/10) + 48);
																				 charLCD((start_hour%10) + 48);
																				 strLCD(":");
																				 charLCD((start_min/10) + 48);
																				 charLCD((start_min%10) + 48);
																				 cmdLCD(GOTO_LINE3_POS0);
																				 strLCD("ENDING TIME:");
																				  charLCD((end_hour/10) + 48);
																				 charLCD((end_hour%10) + 48);
																				 strLCD(":");
																				 charLCD((end_min/10) + 48);
																				 charLCD((end_min%10) + 48);
													enter_valid_key_again:		key = keyscan();
													                                delay_ms(200);
				                                                                   if(key == '#')
																				   	  delay_ms(200);
																					  else
																					   {
																					     cmdLCD(0x01);
																						 strLCD("INVALID KEY");
																						 delay_ms(2000);
																						 cmdLCD(0x01);
																						 strLCD("ENTER '#' KEY");
																						 cmdLCD(GOTO_LINE2_POS0);
																						 strLCD("TO EXIT");
																						 delay_ms(1000);
																						 goto enter_valid_key_again;
																					   }			                                                                  



                                                                         }
															}	
void officer_menu(void)
{
	                              cmdLCD(0x01);
														    cmdLCD(GOTO_LINE1_POS0);
														    strLCD("1.SET VTG TIME 7.EXT");
													    	cmdLCD(GOTO_LINE2_POS0);
														    strLCD("2.START VTG 3.STOP");
														    cmdLCD(GOTO_LINE3_POS0);
														    strLCD("V0TING 4.VIEW RESULT");
														    cmdLCD(GOTO_LINE4_POS0);
														    strLCD("5.RST VTG 6.RTC EDT");
															delay_ms(4000);
															cmdLCD(0x01);
															strLCD("8:EDIT PIN");

}
void clear_vote_records(void)
{
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x002F, 0);
	  
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0030, 0);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0031, 0);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0032, 0);
	 
	    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x004A, 0);
	    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x004B, 0);
	    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x004C, 0);
	    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x004D, 0);
}

void syncing_votes_counts(void) 
{
    unsigned char partyVotesTmp;

    partyVotesTmp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x004A);
    if(partyVotesTmp == 0xFF) partyVotesTmp = 0;
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x002F, partyVotesTmp);

    partyVotesTmp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x004B);
    if(partyVotesTmp == 0xFF) partyVotesTmp = 0;
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0030, partyVotesTmp);

    partyVotesTmp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x004C);
    if(partyVotesTmp == 0xFF) partyVotesTmp = 0;
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0031, partyVotesTmp);

    partyVotesTmp = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x004D);
    if(partyVotesTmp == 0xFF) partyVotesTmp = 0;
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0032, partyVotesTmp);
}

  void init_votes_status(void)
  {
     i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0050,0);
	 i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0051, 0);
	 i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0052, 0);
	 i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0053, 0); 
  }

	void syncing_voters_status(void)
{
   
    unsigned char vote_status;

    vote_status = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0050);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0050, vote_status); 

    vote_status = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0051);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0051, vote_status); 

    vote_status = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0052);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0052, vote_status); 

    vote_status = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0053);
    i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0053, vote_status); 
}

																		