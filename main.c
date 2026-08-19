#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"
#include "rtc.h"
#include "voting.h"
#include "i2c__eeprom.h"
#include "i2c.h"
#include "i2c_defines.h"
#include "kpm.h"
#include "LED_BUZZER.h"
#include <lpc21xx.h>
#include "uart.h"
//#include "RTC_INFO_CHANGE.h"
#include "officer_interrupt.h"

#define VOTE_FLAG_ADDR   0x004E
#define VOTE_VALID       0xAA

#define VOTE_STATUS_FLAG_ADDR   0x0058
#define VOTE_STATUS_VALID       0xBB

#define VOTE_TIMING_FLAG_ADDR   0x0059
#define VOTE_TIMING_VALID       0xCC


#define VOTE_PIN_FLAG_ADDR   0x0071 //password syncing
#define VOTE_PIN_VALID       0xDD

unsigned int  voting_enable_button ;
unsigned int enter_button;
unsigned int time_value,flag1=0;
unsigned int is_it_officer_card;

volatile extern unsigned int OFFICER_INTERRUPT_SWITCH_DETECTED;
volatile  extern unsigned char officer_abort_flag;

unsigned char party_vote_buffer ;
int flag =1;
unsigned int officer_key_value,present_or_not;
unsigned int vote_casted_indication_flag =0;
unsigned int  new_password;
extern volatile int RFID_Detected;
signed char rbuff[16];
extern volatile unsigned char rfid_buf[11];
int existing_or_not =0;
unsigned char keyvalue;
unsigned int num,lastkey,key,num1,lastkey1,num2,lastkey2,num3,lastkey3;
extern unsigned int  stored_password;
unsigned int party_A_count,flag3=0 ,flag4=0,flag5=0,flag6=0;
unsigned int party_B_count;
unsigned int party_C_count;
unsigned int party_D_count;
unsigned char valid_vote = 0;
unsigned int votes_casted =0;
unsigned int move_next_line =0;
unsigned int voter_vote_flag_addr;
unsigned int voter1_flag =0,voter2_flag =0,voter3_flag =0,voter4_flag =0;
unsigned int voter4_password_flag=0,	voter3_password_flag=0,	voter2_password_flag=0,voter1_password_flag=0;
int main()
{
	int i;
	IODIR0 |= 1<<22;  //success
	IODIR0 |= 1<<23;//failure passcode mismatch
	IODIR0|= 1<<21;//to know whether interrupt triggered or not 
	Init_LCD();
	Init_RTC();
	init_i2c();
	Init_KPM();
	UART0_Init();
	UART0_VIC_Init();
	Init_interrupt();

	cmdLCD(GOTO_LINE1_POS0);
	strLCD("AN  RFID - BASED ");
	cmdLCD(GOTO_LINE2_POS0);
	strLCD("SMART  ELECTRONIC ");
	cmdLCD(GOTO_LINE3_POS0);  
	strLCD("VOTING SYSTEM");
	delay_s(2);
	cmdLCD(CLEAR_LCD);
	set_time_data_day();
	cmdLCD(0x01);
	strLCD("system Initializing.");
	delay_ms(1000);
	flag5 = i2c_eeprom_randomread(I2C_EEPROM_SA, VOTE_TIMING_FLAG_ADDR);
	if(flag5 ==  VOTE_TIMING_VALID)
	 {
		 cmdLCD(GOTO_LINE2_POS0);
		 strLCD("syncng time and flag..");
		 
		 delay_ms(2000);
	 }
	else
	 {
		 cmdLCD(GOTO_LINE2_POS0);		
		strLCD("time & flag storage");
		EEPROM_Write_Default_Config();
		delay_ms(2000);
		i2c_eeprom_bytewrite(I2C_EEPROM_SA,
                          VOTE_TIMING_FLAG_ADDR,
                          VOTE_TIMING_VALID);
		 
  	} 

	flag4 = i2c_eeprom_randomread(I2C_EEPROM_SA, VOTE_STATUS_FLAG_ADDR);
	if(flag4 == VOTE_STATUS_VALID)
	{
	     
		   cmdLCD(GOTO_LINE2_POS0);
		   strLCD("                    ");
		   cmdLCD(GOTO_LINE2_POS0);
		   strLCD("syncing status");
		   syncing_voters_status();
		   delay_ms(2000);
	}
	else
	{			    cmdLCD(GOTO_LINE2_POS0);
		   strLCD("                    ");
		     cmdLCD(GOTO_LINE2_POS0);
		strLCD("initial status");
		init_votes_status();
		i2c_eeprom_bytewrite(I2C_EEPROM_SA,
        VOTE_STATUS_FLAG_ADDR,
        VOTE_STATUS_VALID);
		delay_ms(2000); 
	} 
	   cmdLCD(GOTO_LINE2_POS0);
       strLCD("                    ");
	   cmdLCD(GOTO_LINE2_POS0);
	   strLCD("storing voters data");
	   Store_Predefined_Voters();
	   delay_ms(2000);

       cmdLCD(GOTO_LINE2_POS0);
	   strLCD("                    ");
	   cmdLCD(GOTO_LINE2_POS0);
	   strLCD("writing passcode");
	
	   delay_ms(2000);

	  flag3 = i2c_eeprom_randomread(I2C_EEPROM_SA, VOTE_FLAG_ADDR);
	  if(flag3 == VOTE_VALID)
	{
		 cmdLCD(GOTO_LINE2_POS0);
		 strLCD("                    ");
		 cmdLCD(GOTO_LINE2_POS0);
		 strLCD("syncing results");
	     syncing_votes_counts();
		 delay_ms(2000);
		 
	}
	else
	{
		  cmdLCD(GOTO_LINE2_POS0);
		  strLCD("                    ");
		  cmdLCD(GOTO_LINE2_POS0);
		  strLCD("initial results");
		  EEPROM_Init_Votes();
		  delay_ms(2000);
		  i2c_eeprom_bytewrite(I2C_EEPROM_SA, VOTE_STATUS_FLAG_ADDR, VOTE_STATUS_VALID);//added

	}
	 
	flag6 = i2c_eeprom_randomread(I2C_EEPROM_SA, VOTE_PIN_FLAG_ADDR);
	if(flag6 == VOTE_PIN_VALID)
	{
		   cmdLCD(GOTO_LINE2_POS0);
		   strLCD("                    ");
		   cmdLCD(GOTO_LINE2_POS0);
		   strLCD("syncing PINS");
		   delay_ms(2000);
		 
	}
	else
	{
		  cmdLCD(GOTO_LINE2_POS0);
		  strLCD("                    ");
		  cmdLCD(GOTO_LINE2_POS0);
		  strLCD("initial pins");
	      Write_Password_EEPROM(0x002D, 1111);//voter1

	Write_Password_EEPROM(0x006A, 2222);//voter2
	Write_Password_EEPROM(0x006C, 3333);  //3
	Write_Password_EEPROM(0x006E, 4444); //4
	Write_Password_EEPROM(0x0070, 5555);//for officer

		delay_ms(2000);
		i2c_eeprom_bytewrite(I2C_EEPROM_SA, VOTE_PIN_FLAG_ADDR, VOTE_PIN_VALID);//added

	}
	cmdLCD(0x01);

	for(i=5;i>0;i--)
	{
		get_time_data_day_info();
		disp_time_data_day_info();
		delay_s(1);
	}

while(1) 
{
waiting_for_card:
  	if(flag == 1)
	{
		cmdLCD(CLEAR_LCD);
		cmdLCD(GOTO_LINE1_POS0);
		strLCD("WAITING FOR CARD");
		delay_s(2);
		flag = 0;
	}
	
	if (OFFICER_INTERRUPT_SWITCH_DETECTED) 
	{
		OFFICER_INTERRUPT_SWITCH_DETECTED = 0;
		flag = 1;
		flag1 = 1;

		while (1) 
		{
			if (flag1 == 1)
			{
				cmdLCD(CLEAR_LCD);
				cmdLCD(GOTO_LINE1_POS0);
				strLCD("WAITING FOR OFFICER");
				cmdLCD(GOTO_LINE2_POS0);
				strLCD("CARD..");
				flag1 = 0;
			}

			if (RFID_Detected == 0)
			{
				delay_ms(10); 
				continue;
			}

			cmdLCD(0x01);
			strLCD("CARD DETECTED..");
			delay_ms(1000);
			RFID_Detected = 0;
       
			if (Is_officer_RFID_Present_In_EEPROM(rfid_buf)) 
			{
				cmdLCD(CLEAR_LCD);
				strLCD("ACCESS GRANTED");
				delay_ms(1500);

				while (1) 
				{
					officer_menu();
					officer_key_value = keyscan();
                    switch (officer_key_value)
					{
						case '1': delay_ms(150);
           						set_timings_of_voting();
             						break;
						case '2': 
							while (1)
							{
								delay_ms(200);
								cmdLCD(0x01);
								strLCD("ENTER THE PIN:");
								Read___password(&num1, &lastkey1);
								Read_prestored_password(0x0070);

								if (num1 == stored_password)
								{
								select_proper_key: cmdLCD(0x01);
									strLCD("PRESS 7 TO ENABLE");
									cmdLCD(GOTO_LINE2_POS0);
									strLCD("VOTING PROCESS..");

									voting_enable_button = keyscan();
									if (voting_enable_button == '7')
									{
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0000, 1);
										cmdLCD(0x01);
										strLCD("VOTING PROCESS");
										cmdLCD(GOTO_LINE2_POS0);
										strLCD("HAS BEEN ENABLED");
										delay_ms(1500);
										break;
									}
									else
									{
										cmdLCD(0x01);
										strLCD("INVALID KEY..");
										delay_ms(1500);
										goto select_proper_key;
									}
								}
								else
								{
									cmdLCD(0x01);
									strLCD("INVALID PIN");
									delay_ms(1500);
								}
							}
							break;
						case '3': 
							while (1)
							{
								delay_ms(200);
								cmdLCD(0x01);
								strLCD("ENTER THE PIN:");
								Read___password(&num2, &lastkey2);
								Read_prestored_password(0x0070);

								if (num2 == stored_password)
								{
								select_disable_key:cmdLCD(0x01);
									strLCD("PRESS 8 TO DISABLE");
									cmdLCD(GOTO_LINE2_POS0);
									strLCD("VOTING PROCESS..");
									voting_enable_button = keyscan();
									if (voting_enable_button == '8')
									{
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0000, 0);
										
										cmdLCD(0x01);
										strLCD("VOTING PROCESS");
										cmdLCD(GOTO_LINE2_POS0);
										strLCD("HAS BEEN DISABLED");
										delay_ms(1500);
										break;
									}
									else
									{
										cmdLCD(0x01);
										strLCD("INVALID KEY..");
										delay_ms(1500);
										goto select_disable_key;
									}
								}
								else
								{
									cmdLCD(0x01);
									strLCD("INVALID PIN");
									delay_ms(1500);
								}
							}
							break;
						case '4': 
							while (1)
							{
								delay_ms(200);
								cmdLCD(0x01);
								strLCD("ENTER THE PIN:");
								Read___password(&num3, &lastkey3);
								Read_prestored_password(0x0070);

								if (num3 == stored_password)
								{
									Display_Party_Results();
								re_enter_key:
									if (keyscan() == '#') break;
									else
									{
										cmdLCD(0x01);
										strLCD("PRESS '#' TO EXIT");
										delay_ms(1500);
										goto re_enter_key;
									}
								}
								else
								{
									cmdLCD(0x01);
									strLCD("INVALID PIN");
									delay_ms(1500);
								}
							}
							break;
						case '5': 
							while (1)
							{
								delay_ms(150);
								cmdLCD(0x01);
								strLCD("ENTER THE PIN:");
								Read___password(&num, &lastkey);
								Read_prestored_password(0x0070);

								if (num == stored_password) 
								{
									clear_vote_records();
									cmdLCD(0x01);
									strLCD("VOTES CLEARED");
									cmdLCD(GOTO_LINE2_POS0);
									strLCD("SUCCESSFULLY");
									delay_ms(1500);
									break;
								}
								else
								{
									cmdLCD(0x01);
									strLCD("INVALID PIN");
									delay_ms(1500);
								}
							}
							break;
						case '6': RTC_Info_CHANGE(); break;
						case '7':
						           
							cmdLCD(0x01);
							strLCD("Exiting...");
							delay_s(2);
						  	goto exit_officer_menu; 
						case '8':while(1)
							   {
enter_old_password_again:    
								cmdLCD(CLEAR_LCD);
								strLCD("Enter your old");
								cmdLCD(GOTO_LINE2_POS0);
								strLCD("Pin:");
								cmdLCD(GOTO_LINE3_POS0);
								move_next_line =1;
                                Read_password(&num,&lastkey);
								if(num == stored_password)
								{
re_enter_passcode: 
									cmdLCD(CLEAR_LCD);
									strLCD("Enter new Pin");
									cmdLCD(GOTO_LINE2_POS0);

									Read_password(&num, &lastkey);
									new_password = num;
									cmdLCD(CLEAR_LCD);
									strLCD("Confirm pin:");
									Read_password(&num, &lastkey);

									if(new_password == num)
									{
										cmdLCD(CLEAR_LCD);
										strLCD("Updating pin..");
										Write_Password_EEPROM(0x0070, num);
										delay_ms(3000);
										cmdLCD(CLEAR_LCD);
										strLCD("Passcode updated");
										delay_ms(1500);
										break;
									}
									else
									{
										cmdLCD(CLEAR_LCD);
										strLCD("PIN MISMATCH");
										delay_s(2);
										goto re_enter_passcode;
									}
								}
								else
								{
									cmdLCD(CLEAR_LCD);
									strLCD("INVALID OLD PIN");
									delay_s(2);
									goto enter_old_password_again;
								}
								break;
							}
							
							
								
						default: 
							cmdLCD(0x01);
							strLCD("INVALID KEY PRESSED");
							delay_s(1);
							break;
					}
				}
			exit_officer_menu:
				break; 
			}
			else 
			{
				cmdLCD(0x01);
				strLCD("NOT AN");
				cmdLCD(GOTO_LINE2_POS0);
				strLCD("AUTHORIZED PERSON");
				cmdLCD(GOTO_LINE3_POS0);
				strLCD("ACCESS DENIED..");
				delay_s(2);
				break;
			}
		} 
	}

	if(RFID_Detected)
	{
		flag = 1;
		cmdLCD(CLEAR_LCD);
		cmdLCD(GOTO_LINE1_POS0);
		strLCD("CARD DETECTED");
		delay_ms(1000);
		cmdLCD(CLEAR_LCD);
		RFID_Detected = 0;

		if( Is_Voting_Time_Valid())
		{ 
			if(	Is_Voting_Started())
			{
				existing_or_not = Is_RFID_Present_In_EEPROM(rfid_buf);
				if(existing_or_not != 0) 
				{
					switch(existing_or_not)
					{
						case 1: strLCD("VOTER-1");
						         voter1_flag =1	 ;
								   voter1_password_flag=1; break;
						case 2: strLCD("VOTER-2");
						         voter2_flag =1;
								 voter2_password_flag=1; break;
						case 3: strLCD("VOTER-3");
						         voter3_flag =1; 
								 voter3_password_flag=1;break;
						case 4: strLCD("VOTER-4");
						        voter4_flag =1;
								voter4_password_flag=1; break;
					}
					delay_ms(1500);
					cmdLCD(CLEAR_LCD);

     				voter_vote_flag_addr = 0x0050 + (existing_or_not - 1);

					while(1)
					{
						cmdLCD(CLEAR_LCD);
						strLCD("1.VOTING    2.EDIT");
						cmdLCD(GOTO_LINE2_POS0);
						strLCD("3.EXIT");

						key = keyscan();
						while(colscan() == 0);

						if(key == '1')
						{
						     if(i2c_eeprom_randomread(I2C_EEPROM_SA, voter_vote_flag_addr) == 1)
								{
									cmdLCD(0x01);
									strLCD("VOTER ALREADY");
									cmdLCD(GOTO_LINE2_POS0);
									strLCD("CASTED VOTE");
									delay_ms(2000);
									break;
								}
							cmdLCD(0x01);
							cmdLCD(GOTO_LINE1_POS0);
							strLCD("ENTER PIN:");
							 Read_password(&num, &lastkey);;
							
						 if (voter1_flag == 1)
						 {
						    Read_prestored_password(0x002D); 
							voter1_flag =0;
						 }
						 else if (voter2_flag == 1)
						 {					   
						    Read_prestored_password(0x006A); 
							voter2_flag =0;
						 }
						 else if (voter3_flag == 1)
						 {					  
						    Read_prestored_password(0x006C); 
							voter3_flag =0;
						 }
						  else if (voter4_flag == 1)
						 {					  
						    Read_prestored_password(0x006E); 
							voter4_flag =0;
						 }  
						  	

							if(num == stored_password)
							{
								

								cmdLCD(0x01);
								strLCD("valid pin");
								delay_ms(1500);
								cmdLCD(CLEAR_LCD);
								Display_Party_Menu();
								party_vote_buffer = keyscan();

								switch(party_vote_buffer)
								{
									case 'A':
										party_A_count = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x002F);
										party_A_count++;
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0X002F, party_A_count);
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0X004A, party_A_count);//for bacup results purpose
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, voter_vote_flag_addr, 1);
										valid_vote = 1;
										break;
									case 'B':
										party_B_count = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0030);
										party_B_count++;
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0030, party_B_count);
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x004B, party_B_count);
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, voter_vote_flag_addr, 1);
										valid_vote = 1;
										break;
									case 'C':
										party_C_count = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0031);
										party_C_count++;
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0031, party_C_count);
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x004C, party_C_count);
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, voter_vote_flag_addr, 1);
										valid_vote = 1;
										break;
									case 'D':
										party_D_count = i2c_eeprom_randomread(I2C_EEPROM_SA, 0x0032);
										party_D_count++;
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x0032, party_D_count);
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, 0x004D, party_D_count);
										i2c_eeprom_bytewrite(I2C_EEPROM_SA, voter_vote_flag_addr, 1);
										valid_vote = 1;
										break;
									default:
										valid_vote = 0;
										break;
								}

								if(valid_vote)
								{
									cmdLCD(CLEAR_LCD);
									strLCD("Vote Casted");
									cmdLCD(GOTO_LINE2_POS0);
									i2c_eeprom_bytewrite(I2C_EEPROM_SA, VOTE_FLAG_ADDR, 0xAA);
									strLCD("Successfully");
									IOSET0 = 1 << 22;
									delay_ms(2000);
									IOCLR0 = 1 << 22;
									valid_vote = 0;
								}
								else
								{
									cmdLCD(CLEAR_LCD);
									strLCD("INVALID");
									cmdLCD(GOTO_LINE2_POS0);
									strLCD("SELECTION");
									delay_ms(2000);
								}
							}
							else
							{
								IOSET0 = 1<<23;
								cmdLCD(CLEAR_LCD);
								strLCD("WRONG PIN");
								delay_ms(2000);
								IOCLR0 = 1<<23;
								cmdLCD(CLEAR_LCD);
							}
							break;
						}
						else if(key == '2') 
						{
							while(1)
							{
enter_old_password_again1:    
								cmdLCD(CLEAR_LCD);
								strLCD("Enter your old");
								cmdLCD(GOTO_LINE2_POS0);
								strLCD("pin :");
								cmdLCD(GOTO_LINE3_POS0);
								 move_next_line =1;
								Read_password(&num,&lastkey);
							   if(voter1_password_flag ==1)
							   {
								Read_prestored_password(0x002D);
								
								}
								else if(voter2_password_flag ==1)
							   {
								Read_prestored_password(0x006A);
								
								}
								else if(voter3_password_flag ==1)
							   {
								Read_prestored_password(0x006C);
								
								}
								else if(voter4_password_flag ==1)
							   {
								Read_prestored_password(0x006E);
								
								}
                               								




								if(num == stored_password)
								{
re_enter_passcode1: 
									cmdLCD(CLEAR_LCD);
									strLCD("Enter new pin");
									cmdLCD(GOTO_LINE2_POS0);

									Read_password(&num, &lastkey);
									new_password = num;
									cmdLCD(CLEAR_LCD);
									strLCD("Confirm pin:");
									Read_password(&num, &lastkey);

									if(new_password == num)
									{
										cmdLCD(CLEAR_LCD);
										strLCD("Updating pin..");
										delay_ms(3000);
										 if(voter1_password_flag ==1)
										 {
										Write_Password_EEPROM(0x002D, num);
										voter1_password_flag=0;
										}
										else if(voter2_password_flag ==1)
										 {
										Write_Password_EEPROM(0x006A, num);
										voter2_password_flag=0;
										}
										else if(voter3_password_flag ==1)
										 {
										Write_Password_EEPROM(0x006C, num);
										voter3_password_flag=0;
										}
										else if(voter4_password_flag ==1)
										 {
										Write_Password_EEPROM(0x006E, num);
										voter4_password_flag=0;
										}
										cmdLCD(CLEAR_LCD);
										strLCD("Pin updated");
										delay_ms(1500);
										break;
									}
									else
									{
										cmdLCD(CLEAR_LCD);
										strLCD("PIN MISMATCH");
										delay_s(2);
										goto re_enter_passcode1;
									}
								}
								else
								{
									cmdLCD(CLEAR_LCD);
									strLCD("INVALID OLD PIN");
									delay_s(2);
									goto enter_old_password_again;
								}
							}
							break;
						}
						else if(key == '3') 
						{
							cmdLCD(0x01);
							strLCD("exiting..");
							delay_ms(1500);
							break;
						}
					} // end voter menu
				} // end if valid voter ID
				else
				{
					cmdLCD(CLEAR_LCD);
					strLCD("VOTER ID DOES");
					cmdLCD(GOTO_LINE2_POS0);
					strLCD("NOT EXIST IN RECORDS");
					delay_ms(2000); 
				}
			}
			else
			{
				cmdLCD(CLEAR_LCD);
				strLCD("VOTING AUTHORITY HAS");
				cmdLCD(GOTO_LINE2_POS0);
				strLCD("NOT ENABLED PROCESS");
				delay_ms(2000); 
			}
		} 
		else
{
    cmdLCD(CLEAR_LCD);
    strLCD("VOTING NOT STARTED");
    cmdLCD(GOTO_LINE2_POS0);
    strLCD("PLEASE VISIT LATER");
    delay_ms(2000); 
} // end else for voting not started
} // end if RFID_Detected
} // end outer while(1)
} // end main
