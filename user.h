/* 
 * File:   user.h   
 * Author: dracula
 * Comments:
 * Revision history: EDITION 0.1 
 */

// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_HEADER_USER_H
#define	XC_HEADER_USER_H

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */
///* Assign 32x8word Message Buffers for ECAN1 in DMA RAM */
//unsigned int ecan1MsgBuf[32][8] __attribute__((space(dma)));
/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */
void InitApp(void);         /* I/O and Peripheral Initialization */
void DMAInit(void);
void ECANInit(void);
void UartInit(void);
void QEInit(void);
void PwmInit(void);
void Delay(int i);

/* Message structure in RAM */
typedef struct{
    /* Keep track of the buffer status */
    unsigned char buffer_status;
    
    /* RTR message or data message */
    unsigned char message_type;
    
    /* Frame type extended or standard */
    unsigned char frame_type;
    
    /* Buffer being used to send and receive messages */
    unsigned char buffer;
    
    /* 29 bit ID max of 0x1FFF FFFF */
    /* 11 bit ID max of 0x7FF */
    unsigned long id;
    unsigned char data[8];
    unsigned char data_length;
}mID;

#endif