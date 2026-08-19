//i2c_defines.h
//define pin functions
#define SCL_0_2 0x00000010
#define SDA_0_3 0x00000040

//define for I2C_speed configuration
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4) //HZ
#define I2C_SPEED 100000 //HZ
#define I2C_DIVIDER ((PCLK/I2C_SPEED)/2)

//bit defines for I2CONSET sfr
#define AA_BIT 2
#define SI_BIT 3
#define STO_BIT 4
#define STA_BIT 5
#define I2EN_BIT 6

//bit defines for I2CONCLR sfr
#define AAC_BIT 2
#define SIC_BIT 3
#define STAC_BIT 5
#define I2ENC_BIT 6

#define I2C_EEPROM_SA 0x50  //7Bit slave Addr  
