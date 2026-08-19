//primary drivers,defined only for taking single digit 
void Init_KPM(void);
unsigned int colscan(void);
unsigned int rowcheck(void);
unsigned int colcheck(void);
unsigned int keyscan(void);//to decide which key is being pressed


//secondary drivers
void ReadNum(unsigned int *num,unsigned int* lastkey);//num is to store the result,lastkey is to display key which is being pressed at the last 
void Read_password(unsigned int *num,unsigned int* lastkey);
//fucntion two read only two digits
unsigned char Read_two_digits(void);//old prototype

unsigned int  Read_TwoDigits(void);
 unsigned int key__scan(void);//for reading two digits
 
 void Read___password(unsigned int *num, unsigned int *lastkey);//for officer
unsigned int Read_FourDigits(void);