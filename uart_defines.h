#define  FOSC 12000000
#define  CCLK (FOSC*5)
#define  PCLK  (CCLK/4)
#define   BAUD 9600 // rate at which  bits transeferring happens per second
#define  DIVISOR (PCLK/(16*BAUD))//Value used to generate required baud rate

//UOLCR register bit definition(Line control register)
#define _8BIT 3 //data lenght selection
#define WORD_LEN _8BIT
#define DLAB_BIT 7

//U0LSR register bit definitions(line status register)
#define RDR_BIT  0 // Receiver Data Ready
#define THRE_BIT 5//
#define TEMT_BIT 6//indicates completion status
