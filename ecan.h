/* 
 * File:   ecan.h
 * Author: dracula
 * Revision history: EDITION 0.1 
 * Created on December 13, 2017, 2:06 PM
 */

// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_HEADER_ECAN_H
#define	XC_HEADER_ECAN_H

/* CAN Baud Rate Configuration 		*/
#define FCAN  	      	40000000 
#define BITRATE 		1000000  
#define NTQ 			20		// 20 Time Quanta in a Bit Time
#define BRP_VAL			((FCAN/(2*NTQ*BITRATE))-1)

/* Defines used in ecan.c */
/* ECAN message buffer length */
#define ECAN1_MSG_BUF_LENGTH 	32

/* ECAN message type identifiers */
#define CAN_MSG_DATA	0x01
#define CAN_MSG_RTR		0x02
#define CAN_FRAME_EXT	0x03
#define CAN_FRAME_STD	0x04
#define CAN_BUF_FULL	0x05
#define CAN_BUF_EMPTY	0x06

/* Filter and mask defines */
/* Macro used to write filter/mask ID to Register CiRXMxSID and 
CiRXFxSID. For example to setup the filter to accept a value of 
0x123, the macro when called as CAN_FILTERMASK2REG_SID(0x123) will 
write the register space to accept message with ID 0x123 
USE FOR STANDARD MESSAGES ONLY */
#define CAN_FILTERMASK2REG_SID(x) ((x & 0x07FF)<< 5)
/* the Macro will set the "MIDE" bit in CiRXMxSID */
#define CAN_SETMIDE(sid) (sid | 0x0008)
/* the macro will set the EXIDE bit in the CiRXFxSID to 
accept extended messages only */
#define CAN_FILTERXTD(sid) (sid | 0x0008)
/* the macro will clear the EXIDE bit in the CiRXFxSID to 
accept standard messages only */
#define CAN_FILTERSTD(sid) (sid & 0xFFF7)
/* Macro used to write filter/mask ID to Register CiRXMxSID, CiRXMxEID and 
CiRXFxSID, CiRXFxEID. For example to setup the filter to accept a value of 
0x123, the macro when called as CAN_FILTERMASK2REG_EID(0x123) will 
write the register space to accept message with ID 0x123 
USE FOR EXTENDED MESSAGES ONLY */
#define CAN_FILTERMASK2REG_EID0(x) (x & 0xFFFF)
#define CAN_FILTERMASK2REG_EID1(x) (((x & 0x1FFC)<< 3)|(x & 0x3))

/* DMA CAN Message Buffer Configuration */
typedef unsigned int ECAN1MSGBUF [ECAN1_MSG_BUF_LENGTH][8];
extern ECAN1MSGBUF  ecan1MsgBuf __attribute__((space(dma)));

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

/* function prototypes as defined in can.c */
void ECANInit (void);
void DMAInit(void);
void sendECAN(mID *message);
void rxECAN(mID *message);
void clearRxFlags(unsigned char buffer_number);

#endif	/* XC_HEADER_ECAN_H */
