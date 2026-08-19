
void Init_LCD(void);//to initialize the LCD display
void cmdLCD(unsigned char );
void charLCD(unsigned char );//to display the character on LCD
void writeLCD(unsigned char byte);//to transfer the data to LCD
void strLCD(const char *);
void U32LCD(unsigned int);
void S32LCD(int);
void F32LCD(float);     //to display floating number8
void BuildCGRAM(unsigned char *,unsigned char);
void Display_Party_Menu(void);
void Display_Party_Results(void);
