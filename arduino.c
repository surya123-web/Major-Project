#include <LPC21xx.h>
#include "arduino_defines.h"
#include "arduino.h"
void pinMode( unsigned int pinno,unsigned int pindir)
{
     if(pinno <32)
         {
           if(pindir == OUTPUT)
           {
              IODIR0 = IODIR0 | 1<<pinno;
           }
       else
          {
               IODIR0 = IODIR0 &~(1<<pinno);
           }
           }
           else if(pinno>=32 && pinno<=47)
           {
              if(pindir == OUTPUT)
                  {
              IODIR1= IODIR1 | (1<<(pinno-16));
                   }
                   else
                   {
                     IODIR1 = IODIR1 &~(1<<(pinno -16));
                   }
                }
}

        void digitalwrite(unsigned int pinno,unsigned int bit)
        {
                if(pinno<32)
                {
                        bit?(IOSET0= 1<<pinno):(IOCLR0 = 1<<pinno);
                }
                else if((pinno>=32 )   && ( pinno<=47))
                {
                        bit ? (IOSET1 = (1 << (pinno-16))) : (IOCLR1 = (1 << (pinno-16)));

                }
        }

          unsigned int digitalRead(unsigned int  pinno)
          {
               unsigned int bit;
                   if(pinno<32)
                   {
                    bit = ((IOPIN0>>pinno)&1);
                        }
                        else if((pinno>=32) && (pinno<=47))
                        {
                        bit = ((IOPIN1>>(pinno-16))&1)  ;
                        }
                        return bit;
           }
					
					 void portMode(unsigned int pinstartno,unsigned int npins,unsigned int pinsdir)//prototype declared
{
	if(pinstartno <32)
	{
		if(pinsdir ==1)//1 or OUTPUT
		{
			IODIR0 |= ((1<<npins)-1) <<pinstartno;
		}
		else
		{
			IODIR0 &= ~((1<<npins)-1)<<pinstartno;
		}
	}
	else if(pinstartno>=32 && pinstartno <=47)
	{
		if(pinsdir == 1)
		{
			IODIR1 |= ((1<<npins)-1)<<(pinstartno-16);
		}
		else
		{
			IODIR1 &= ~ (((1<<npins)-1)<<(pinstartno-16));
		}
	}
}
void write2pins(unsigned int pinstartno,unsigned int npins,unsigned int data)
{
		if(pinstartno<32)
		{
			IOPIN0 = ((IOPIN0 & ~(((1<<npins)-1)<<pinstartno)) | (data<< pinstartno));
		}
		else if (pinstartno >=32  && pinstartno <=47)
		{
			 IOPIN1 = (  (IOPIN1 & ~(((1<<npins)-1)<<  (pinstartno-16))) | (data<< (pinstartno-16)));
		}
}	
