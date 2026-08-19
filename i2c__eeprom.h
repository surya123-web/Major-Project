
void i2c_eeprom_bytewrite(unsigned char slaveAddr, unsigned short wBufAddr, unsigned char dat);
unsigned char i2c_eeprom_randomread(unsigned char slaveAddr, unsigned short rBufAddr);
void i2c_eeprom_pagewrite(unsigned char slaveAddr, unsigned short wBufStartAddr, unsigned char *p, unsigned char nBytes);
void i2c_eeprom_seqread(unsigned char slaveAddr, unsigned short rBufStartAddr, unsigned  char *p, unsigned char nBytes);  
void EEPROM_Write_Default_Config(void);
void Write_Password_EEPROM(unsigned short addr, unsigned short password);
void Read_prestored_password(unsigned short);
void EEPROM_Init_Votes(void);
void syncing_EEPROM_Write_Default_Config(void);



