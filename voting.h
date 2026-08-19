int Is_Voting_Time_Valid(void);
int Is_Voting_Started(void);
int Is_RFID_Present_In_EEPROM(volatile unsigned char *rfid_buf);
void Store_Predefined_Voters(void);
unsigned  char Is_officer_RFID_Present_In_EEPROM( volatile unsigned char *rfid_buf);
 void syncing_voters_status(void);
