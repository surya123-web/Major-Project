
#include "lcd_defines.h"
#include "lcd.h"
#include "kpm_defines.h"
#include <LPC21xx.h>
#include "delay.h"
extern unsigned  int move_next_line;
volatile  extern unsigned char officer_abort_flag;
   unsigned char KpmLUT[4][4] = { {'1','2','3','A'}, 
	                               {'4','5','6','B'},  
                               	 {'7','8','9','C'},
                              	 { '*','0','#','D'}  };


	  unsigned int LUT[4][4] = {
    {1, 2, 3, 65},   // 10 = A
    {4, 5, 6, 66},   // 11 = B
    {7, 8, 9, 67},   // 12 = C
    {15, 0, 14, 68}  // backspace , 0, #, D
};

                 
void Init_KPM(void)
{
   //rows output and grounded
   IODIR1       = IODIR1 | 15<<ROW0;
   //columns input &high ,any by default

}

unsigned int colscan(void)
{
     unsigned int status;//declaring a variable
    status = (((IOPIN1>>COL0)&15)<15)?0:1;//<15 col is down switch is pressed ,0 key pressed ,1- key not pressed
        return status;
}

unsigned int rowcheck(void)
{
  unsigned int  r;
  for(r=0;r<=3;r++)
  {
     //start with row0,upto row3
         //ground only one row,keep other rows high
     IOPIN1 = ((IOPIN1&~(15<<ROW0))      | (~(1<<r)<<ROW0));
         if(colscan()==0)
         {
           break;//if any column goes down
         }
  }
   IOCLR1 = 15<<ROW0 ;//mask all
   return r;
}

  unsigned  colcheck(void)
  {
     unsigned int c;
         for(c=0;c<=3;c++)
         {
           // checking one column at a time for low
               if(((IOPIN1>>(COL0+c))&1)==0)
               {
                break;
               }

     }
          return c;
  }
        
         unsigned int keyscan(void)
         {
               unsigned int r,c;
				 unsigned char KeyV;
                  //initialize KPM
                  Init_KPM();
                  // keep waiting till any switch press detection
                  while(colscan());
									
									//whenever switch pressed,means whenever it goes  down loop exits
                  //the row and col is checked accordimgly as per [r][c] key values is detected
									 
                  r = rowcheck();
                  c = colcheck();
                  //extracting a key value from a look up table
                  KeyV =KpmLUT[r][c];
                  return KeyV;//gonna return character

         }
				 
				 //for reaing two digits
				  unsigned int key__scan(void)
         {
               unsigned int r,c;
								 unsigned char KeyV;
                  //initialize KPM
                 Init_KPM();
                  // keep waiting till any switch press detection
                  while(colscan());//whenever switch pressed,means whenever it goes  down loop exits
                  //the row and col is checked accordimgly as per [r][c] key values is detected
                  r = rowcheck();
                  c = colcheck();
                  //extracting a key value from a look up table
                  KeyV =LUT[r][c];
                  return KeyV;//gonna return character

         }


void ReadNum(unsigned int * num,unsigned int *lastkey)
{
    unsigned char  key;
       *num=0;
       while(1)
       {
               key = keyscan();
               if(key >= '0' && key <= '9')
               {
                  *num = *num * 10 +(key - 48);
               }
               else
               { 
                       *lastkey = key;
                       while(colscan() ==0);//after that switch is released immediately breaks
                       break;
               }
       }
}

unsigned char Read_two_digits(void)
{
	   unsigned char key;
	   int num =0;
	int count=0;
	   while(1)
		 {
			 			 reenter_again:	 key = keyscan();
			         while(colscan() !=0);
			      if(key>='0' && key<='9')
						{
							 charLCD(key);
							 num = num *10+ (key - 48);
							 count++;
											 if(count == 2)
											 {
												  while(colscan() != 0); 
				enter_again:		key = keyscan();
													if(key == '#')
													{
														 return num;
													}
													else if(key == '*')
													{
														cmdLCD(0x10);
														charLCD(' ');
														num = num/10;	
														 count--;
													goto reenter_again;										
													}
													else
														goto enter_again;
											 }
						}
						else if(key == '*')
						{
							 if(count>0)
							 {
	 						 cmdLCD(0x10);
		           charLCD(' ');
               num = num/10;
						
                  count--;
							 }
                  goto reenter_again;								
						}
					
		 }
}
 //RTC change info
unsigned int  Read_TwoDigits(void)
{
    unsigned int k;
    unsigned char digits[2] = {0, 0}; // store two digits
    int col = 0;            // number of digits entered

    cmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        k = key__scan();

        // Wait until key released (debounce)
        while(colscan() == 0);
        delay_ms(20);
       
        if(k == 14)
        {
            if(col == 2)
                break;  
            else
                continue; 
        }

       
       
        if(k == 15)
        {
            if(col > 0)
            {
                col--;               
                digits[col] = 0;     
                cmdLCD(GOTO_LINE2_POS0 + col);
                charLCD(' ');       
                cmdLCD(GOTO_LINE2_POS0 + col);
            }
            continue;
        }

       
        if(k >= 65 && k <= 68)
            continue;

        if(k <= 9)
        {
            if(col < 2)
            {
                digits[col] = (unsigned char )k;

                // Show digit briefly
                cmdLCD(GOTO_LINE2_POS0 + col);
                charLCD(digits[col] + '0');
                delay_ms(150);

              
                cmdLCD(GOTO_LINE2_POS0 + col);
                charLCD('*');

                col++;
            }
            continue;
        }

        
    }

    return (digits[0] * 10U) + digits[1];
}


unsigned int Read_FourDigits(void)
{
    unsigned int k;
	unsigned int val = 0,i;
    unsigned char digits[4] = {0, 0, 0, 0}; // store up to 4 digits
    int col = 0;            // number of digits entered

    cmdLCD(GOTO_LINE2_POS0);

    while(1)			   
    {
        k = key__scan();


        while(colscan() == 0);
        delay_ms(200);

      
        if(k == 14)
        {
            if(col >= 2)  // accept 2 to 4 digits
                break;  
            else
                continue; 
        }

      
        if(k == 15 && col > 0)
        {
            col--;               
            digits[col] = 0;     
            cmdLCD(GOTO_LINE2_POS0 + col);
            charLCD(' ');       
            cmdLCD(GOTO_LINE2_POS0 + col);
            continue;
        }

        if(k >= 65 && k <= 68)
            continue;

      if(k <= 9)
        {
            if(col < 4)
            {
                digits[col] = (unsigned char)k;

                cmdLCD(GOTO_LINE2_POS0 + col);
                charLCD(digits[col] + '0');
                delay_ms(150);

                // Mask with '*'
                cmdLCD(GOTO_LINE2_POS0 + col);
                charLCD('*');

                col++;
            }
            continue;
        }

        // ignore other keys
    }

    // convert digits to integer
    
    for( i = 0; i < col; i++)
        val = val*10 + digits[i];

    return val; 
}
void Read_password(unsigned int *num, unsigned int *lastkey)
{
    unsigned int key;
    unsigned char i = 0;
    *num = 0;
	   if(move_next_line ==1)
	   {
	      goto enter_key;
		   move_next_line =0;
	   }
	   else
	   {
    cmdLCD(GOTO_LINE2_POS0);
	}
	  enter_key:
    while(1)
    {
        

        key = keyscan();

        if(key == '0')
        {
            if(i > 0)
            {
                i--;
                *num = *num / 10;
                cmdLCD(GOTO_LINE2_POS0 + i);
                charLCD(' ');
                cmdLCD(GOTO_LINE2_POS0 + i);
            }

            while(colscan() == 0);
            delay_ms(150);
        }
        else if(key >= '1' && key <= '9')
        {
            cmdLCD(GOTO_LINE2_POS0 + i);
            charLCD(key);
            delay_ms(100);

            cmdLCD(GOTO_LINE2_POS0 + i);
            charLCD('*');

            *num = (*num * 10) + (key - '0');
            i++;

            while(colscan() == 0);
            delay_ms(150);
        }
        else if(key == '#')
        {
            *lastkey = key;
            while(colscan() == 0);
            delay_ms(150);
            break;
        }
    }
}

void Read___password(unsigned int *num, unsigned int *lastkey)//officer purpose working
{
    unsigned char key;
    unsigned char i = 0;

    *num = 0;
    cmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        key = keyscan();   // wait for key press

        /* wait for key release */
        while(colscan() == 0);
        delay_ms(200);

        if(key == '*')   
        {
            if(i > 0)
            {
                i--;
                *num = *num / 10;

                cmdLCD(GOTO_LINE2_POS0 + i);
                charLCD(' ');
                cmdLCD(GOTO_LINE2_POS0 + i);
            }
        }
        else if(key >= '0' && key <= '9')   // DIGIT
        {
            if(i < 4)   // LIMIT DIGITS
            {
                cmdLCD(GOTO_LINE2_POS0 + i);
                charLCD('*');
                *num = (*num * 10) + (key - '0');
                i++;
            }
        }
        else if(key == '#')   // ENTER
        {
            *lastkey = key;
            break;
        }
    }
}
