#include "uart_defines.h"
#include <lpc21xx.h>
#include "delay.h"


volatile int RFID_Detected = 0;
volatile unsigned char rfid_buf[11];
void UART0_ISR(void) __irq;

void UART0_Init(void)
{
        PINSEL0 |=0x00000001;
        PINSEL0 |=0x00000004;
 
        U0LCR =WORD_LEN |(1<<DLAB_BIT);//dlab set to access baud rate registers
        U0DLL = DIVISOR;
        U0DLM = DIVISOR>>8;
	    // U0IER |=0x01;//whenever a new byte is arrived an interrupt is triggered
        //disable dlab bit (to access THR and RBR SFrs)
        U0LCR  =U0LCR &~(1<<DLAB_BIT);//dlab disabl allows us to transmit data via THR and RDR
	U0IER |=0x01;
}

void UART0_VIC_Init(void)
{
   
    
  
    VICIntEnable = (1 << 6);                 // Enable UART0 in VIC
	 VICIntSelect &= ~(1 << 6);          	
	  VICVectCntl0 = 0x20 | 6;                 
	  VICVectAddr0 = (unsigned long)UART0_ISR; 
}

void UART0_ISR(void) __irq
{
    unsigned static int i = 0;  
    char num;
     
    if (((U0IIR >> 1) & 0x07) == 2)
    {
        num = U0RBR; 
       
        if (i < 10)  
        {
            rfid_buf[i++] = num ;
        }

        if (i >= 10)
        {
            rfid_buf[i] = '\0'; 
            RFID_Detected = 1;    
            i = 0;               
        }
				
    }

    VICVectAddr = 0;  // Signal end of interrupt
}
