//i2c.h
void init_i2c(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_stop(void);
void i2c_write(unsigned char dat);
unsigned char  i2c_nack(void);
unsigned char i2c_mack(void);     
