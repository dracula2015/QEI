/* 
 * File:   user.c   
 * Author: dracula
 * Comments:
 * Revision history: EDITION 0.1 
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
//#define MANUAL

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */

#define FCY 40000000
#define BAUDRATE 57600//9600  
#define BRGVAL ((FCY/BAUDRATE)/16)-1

#define FCAN  	      	40000000 
#define BITRATE 		1000000  
#define NTQ 			20		// 20 Time Quanta in a Bit Time
#define BRP_VAL			((FCAN/(2*NTQ*BITRATE))-1)

#define DELAY_105us asm volatile ("REPEAT,#4201");Nop();//105us delay 

#ifdef MANUAL
/* Assign 32x8word Message Buffers for ECAN1 in DMA RAM */
extern unsigned int ecan1MsgBuf[32][8] __attribute__((space(dma)));
#endif

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    //*************************************************************
    // Unlock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); 

    //*************************************************************
    // Configure Input Functions
    // (See Table 30-1)
    //*************************************************************
    //***************************
    // Assign U1Rx To Pin RP20
    //***************************
    //RPINR18bits.U1RXR = 0;
    //RPINR18bits.U1RXR = 19;
    RPINR18bits.U1RXR = 20;
    //***************************
    // Assign U1CTS To Pin RP1
    //***************************
    //RPINR18bits.U1CTSR = 1;
    
    //*************************************************************
    // Configure Output Functions
    // (See Table 30-2)
    //*************************************************************
    //***************************
    // Assign U1Tx To Pin RP4
    //***************************
    //RPOR1bits.RP2R = 3;
    //RPOR6bits.RP12R = 3;
    RPOR2bits.RP4R = 3;
    //***************************
    // Assign U1RTS To Pin RP3
    //***************************
    //RPOR1bits.RP3R = 4;
    
    //*************************************************************
    //Assign QEI1 Phase A To Pin RP10
    //*************************************************************
    RPINR14bits.QEA1R = 10;
    
    //*************************************************************
    //Assign QEI1 Phase B To Pin RP11
    //*************************************************************
    RPINR14bits.QEB1R = 11;
    
    //*************************************************************
    //Assign QEI1 INDEX To Pin RP25
    //*************************************************************
    RPINR15bits.INDX1R = 25;
    
    //*************************************************************
    // Configure ECAN Module
    //*************************************************************
    //***************************
    // Assign ECAN1 C1RxD To Pin RP24
    //***************************
    RPINR26bits.C1RXR = 24;
    //***************************
    // Assign ECAN1 C1TxD To Pin RP14
    //***************************
    RPOR7bits.RP14R = 14;
    //*************************************************************
    // Lock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON | (1<<6));

    /* Setup analog functionality and port direction */
    TRISAbits.TRISA7=0;
    TRISAbits.TRISA10=0;
    /*
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB5 = 1;
    */
    //TRISB = 0xFFFF;
    /* Initialize peripherals */
    DMAInit();
    ECANInit();
    UartInit();
    QEInit();
    PwmInit();
    
}

void PwmInit(void)
{
    #if 1
        /*
         * PxTPER = FCY/(FPWM*PxTMR PreScaler)-1
         * PxTMR PreScaler=1:1
         * FPWM=20KHz
         * FCY=40MHz, refer to function ConfigureOscillator
         * PxTPER=1999
         * 0x07CF
         * 0b0000 0111 1100 1111
         */
        P2TCON=0x8000;   //or P1TCONbits.PTEN = 1;
        P2TMR=0x0000;
        P2TPER=1999;     // period:50us
        //P1SECMP=0x0000;
        PWM2CON1=0x0FFF;
        PWM2CON2=0x0040;
        P2OVDCON=0xFF00;
        //P1FLTACON=0x0080;
        //P1DTCON1=0x0000;
        //P1DTCON2=0x0000;
        //P2DC1=100;
        // LSB is not used for duty cycle, the realy duty cycle count should be PIDC*/2 !!!!!!
    #endif

    #if 0
    /* Configuration register FPOR */

    /* PWM time base operates in a Free Running mode */
    P1TCONbits.PTMOD = 0b00;

    /* PWM time base input clock period is TCY (1:1 prescale) */
    /* PWM time base output post scale is 1:1 */
    P1TCONbits.PTCKPS = 0b00;
    P1TCONbits.PTOPS = 0b00;

    /* Choose PWM time period based on input clock selected */
    /* Refer to Equation: PxTPER=FCY/(FPWM × PxTMR PreScaler)-1*/
    /* PWM switching frequency is 20 kHz */
    /* FCY is 40 MHz */
    P1TPER = 1999;

    /* PWM I/O pairs 1 to 3 are in complementary mode */
    /* PWM pins are enabled for PWM output */
    PWM1CON1bits.PMOD1 = 0;
    PWM1CON1bits.PMOD2 = 0;
    PWM1CON1bits.PMOD3 = 0;
    PWM1CON1bits.PEN1H = 1;
    PWM1CON1bits.PEN2H = 1;
    PWM1CON1bits.PEN3H = 1;
    PWM1CON1bits.PEN1L = 1;
    PWM1CON1bits.PEN2L = 1;
    PWM1CON1bits.PEN3L = 1;
    /* Immediate update of PWM enabled */
    PWM1CON2bits.IUE = 1;

    /* PWM I/O pin controlled by PWM Generator */
    P1OVDCONbits.POVD3H = 1;
    P1OVDCONbits.POVD2H = 1;
    P1OVDCONbits.POVD1H = 1;
    P1OVDCONbits.POVD3L = 1;
    P1OVDCONbits.POVD2L = 1;
    P1OVDCONbits.POVD1L = 1;

    /* Initialize duty cycle values for PWM1, PWM2 and PWM3 signals */
    P1DC1 = 200;
    P1DC2 = 200;
    P1DC3 = 200;
    P1TCONbits.PTEN = 1;
    #endif
}

void UartInit(void)
{
    
    U1MODEbits.STSEL = 0; // 1-stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U1MODEbits.BRGH = 0; // Low Speed mode
    U1MODEbits.LPBACK = 0;
    U1BRG = BRGVAL; // BAUD Rate Setting for 57600
    U1STAbits.UTXISEL0 = 0; // Interrupt after one Tx character is transmitted
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.URXISEL = 0;
    IEC0bits.U1TXIE = 1; // Enable UART Tx interrupt
    IEC0bits.U1RXIE = 1; // Enable UART Rx interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART Tx
    /* wait at least 104 usec (1/9600) before sending first char */
    DELAY_105us
//    int i;
//    for(i = 0; i < 4160; i++)
//    {
//    Nop();
//    }

}

void QEInit(void)
{

    MAX1CNT = 36351; //512*71-1=36351
    IEC3bits.QEI1IE = 1;
    DFLT1CONbits.QEOUT = 1;
    DFLT1CONbits.QECK = 2;//1:4????
    QEI1CONbits.QEIM = 7;
    /*
    QEI1CONbits.QEIM = 6;
    QEI1CONbits.POSRES = 1;
    */
}

#ifdef MANUAL

void ECANInit(void)
{   
    /* Set the ECAN module for Configuration Mode before writing into the Baud
    Rate Control Registers*/
    C1CTRL1bits.REQOP = 4;
    /* Wait for the ECAN module to enter into Configuration Mode */
    while(C1CTRL1bits.OPMODE != 4);
    /* Phase Segment 1 time is 8 TQ */
    C1CFG2bits.SEG1PH = 0x7;
    /* Phase Segment 2 time is set to be programmable */
    C1CFG2bits.SEG2PHTS = 0x1;
    /* Phase Segment 2 time is 6 TQ */
    C1CFG2bits.SEG2PH = 0x5;
    /* Propagation Segment time is 5 TQ */
    C1CFG2bits.PRSEG = 0x4;
    /* Bus line is sampled three times at the sample point */
    C1CFG2bits.SAM = 0x1;
    /* Synchronization Jump Width set to 4 TQ */
    C1CFG1bits.SJW = 0x3;
    /* Baud Rate Prescaler bits set to 1:1, i.e., TQ = (2*1*1)/ FCAN */
    C1CFG1bits.BRP = BRP_VAL;
    //C1CFG1bits.BRP = 0x0;
    /* 4 CAN Messages to be buffered in DMA RAM */
    C1FCTRLbits.DMABS = 0b000;
    
    /* Filter configuration */
    /* Enable window to access acceptance filter registers */
    C1CTRL1bits.WIN = 0b1;
    
//    /* Select Acceptance Filter Mask 0 for Acceptance Filter 0 */
//    C1FMSKSEL1bits.F0MSK=0x0;
//    /* Configure Acceptance Filter Mask 0 register to mask EID<5:0>
//    Mask Bits (29-bits) : 0b0 0000 0000 0000 0000 0000 0011 1111
//    SID<10:0> : 0b00000000000 ..SID<10:0> or EID<28:18>
//    EID<17:16> : 0b00 ..EID<17:16>
//    EID<15:0> : 0b0000000000111111 ..EID<15:0> */
//    C1RXM0SIDbits.SID = 0x0;
//    C1RXM0SIDbits.EID = 0x0;
//    C1RXM0EIDbits.EID = 0x3F;
//    /* Configure Acceptance Filter 0 to match extended identifier
//    Filter Bits (29-bits) : 0b1 1110 0000 0011 1111 0101 10xx xxxx
//    SID<10:0> : 0b11110000000 ..SID<10:0> or EID<28:18>
//    EID<17:16> : 0b11 ..EID<17:16>
//    EID<15:0> : 0b1111010110xxxxxx ..EID<15:0> */
//    C1RXF0SIDbits.SID = 0x780;
//    C1RXF0SIDbits.EID = 0x3;
//    C1RXM0EIDbits.EID = 0xF580;
//    /* Acceptance Filter 0 to check for Extended Identifier */
//    C1RXM0SIDbits.MIDE = 0x1;
//    C1RXF0SIDbits.EXIDE= 0x1;
////    /* Acceptance Filter 0 to use FIFO Message Buffer to store message */
////    C1BUFPNT1bits.F0BP = 0xF;
//    /* Acceptance Filter 0 to use Message Buffer 1 to store message */
//    C1BUFPNT1bits.F0BP = 0x1;
//    /* Enable Acceptance Filter 0 */
//    C1FEN1bits.FLTEN0=0x1;
    
    /* Select Acceptance Filter Mask 0 for Acceptance Filter 0 */
    C1FMSKSEL1bits.F0MSK=0x0;
    /* Configure Acceptance Filter Mask 0 register to mask EID<28:0>
    Mask Bits (29-bits) : 0b1 1111 1111 1111 1111 1111 1111 1111
    SID<10:0> : 0b11111111111 ..SID<10:0> or EID<28:18>
    EID<17:16> : 0b11 ..EID<17:16>
    EID<15:0> : 0b1111111111111111 ..EID<15:0> */
    C1RXM0SIDbits.SID = 0x7FF;
    C1RXM0SIDbits.EID = 0x3;
    C1RXM0EIDbits.EID = 0xFFFF;
    /* Configure Acceptance Filter 0 to match extended identifier
    Filter Bits (29-bits) : 0b1 1110 0000 0011 1111 0101 1000 0000
    SID<10:0> : 0b11110000000 ..SID<10:0> or EID<28:18>
    EID<17:16> : 0b11 ..EID<17:16>
    EID<15:0> : 0b1111010110000000 ..EID<15:0> */
    C1RXF0SIDbits.SID = 0x780;
    C1RXF0SIDbits.EID = 0x3;
    C1RXF0EIDbits.EID = 0xF580;
    /* Acceptance Filter 0 to check for Extended Identifier */
    C1RXM0SIDbits.MIDE = 0x1;
    C1RXF0SIDbits.EXIDE= 0x1;
    /* Acceptance Filter 0 to use Message Buffer 1 to store message */
    C1BUFPNT1bits.F0BP = 0x1;
    /* Enable Acceptance Filter 0 */
    C1FEN1bits.FLTEN0=0x1;
    
    /* Select Acceptance Filter Mask 1 for Acceptance Filter 1 */
    C1FMSKSEL1bits.F1MSK=0x1;
    /* Configure Acceptance Filter Mask 0 register to mask EID<28:0>
    Mask Bits (29-bits) : 0b1 1111 1111 1111 1111 1111 1111 1111
    SID<10:0> : 0b11111111111 ..SID<10:0> or EID<28:18>
    EID<17:16> : 0b11 ..EID<17:16>
    EID<15:0> : 0b1111111111111111 ..EID<15:0> */
    C1RXM1SIDbits.SID = 0x7FF;
    C1RXM1SIDbits.EID = 0x3;
    C1RXM1EIDbits.EID = 0xFFFF;
    /* Configure Acceptance Filter 0 to match extended identifier
    Filter Bits (29-bits) : 0b1 1110 0000 0011 1111 0101 1000 0001
    SID<10:0> : 0b11110000000 ..SID<10:0> or EID<28:18>
    EID<17:16> : 0b11 ..EID<17:16>
    EID<15:0> : 0b1111010110000001 ..EID<15:0> */
    C1RXF1SIDbits.SID = 0x780;
    C1RXF1SIDbits.EID = 0x3;
    C1RXF1EIDbits.EID = 0xF581;
    /* Acceptance Filter 0 to check for Extended Identifier */
    C1RXM1SIDbits.MIDE = 0x1;
    C1RXF1SIDbits.EXIDE= 0x1;
    /* Acceptance Filter 0 to use Message Buffer 2 to store message */
    C1BUFPNT1bits.F1BP = 0x2;
    /* Enable Acceptance Filter 0 */
    C1FEN1bits.FLTEN1=0x1;
    
    /* Select Acceptance Filter Mask 2 for Acceptance Filter 2 */
    C1FMSKSEL1bits.F2MSK=0x2;
    /* Configure Acceptance Filter Mask 0 register to mask EID<28:0>
    Mask Bits (29-bits) : 0b1 1111 1111 1111 1111 1111 1111 1111
    SID<10:0> : 0b11111111111 ..SID<10:0> or EID<28:18>
    EID<17:16> : 0b11 ..EID<17:16>
    EID<15:0> : 0b1111111111111111 ..EID<15:0> */
    C1RXM2SIDbits.SID = 0x7FF;
    C1RXM2SIDbits.EID = 0x3;
    C1RXM2EIDbits.EID = 0xFFFF;
    /* Configure Acceptance Filter 0 to match extended identifier
    Filter Bits (29-bits) : 0b1 1110 0000 0011 1111 0101 1000 0010
    SID<10:0> : 0b11110000000 ..SID<10:0> or EID<28:18>
    EID<17:16> : 0b11 ..EID<17:16>
    EID<15:0> : 0b1111010110000010 ..EID<15:0> */
    C1RXF2SIDbits.SID = 0x780;
    C1RXF2SIDbits.EID = 0x3;
    C1RXF2EIDbits.EID = 0xF582;
    /* Acceptance Filter 0 to check for Extended Identifier */
    C1RXM2SIDbits.MIDE = 0x1;
    C1RXF2SIDbits.EXIDE= 0x1;
    /* Acceptance Filter 0 to use Message Buffer 2 to store message */
    C1BUFPNT1bits.F2BP = 0x3;
    /* Enable Acceptance Filter 0 */
    C1FEN1bits.FLTEN2=0x1; 
    
    /* Clear Window Bit to Access ECAN Control Registers */
    C1CTRL1bits.WIN=0b0;

    C1FCTRLbits.FSA = 0x8;
    /* Put the ECAN Module into Normal Mode Operating Mode*/
    C1CTRL1bits.REQOP = 0;
    /* Wait for the ECAN module to enter into Normal Operating Mode */
    while(C1CTRL1bits.OPMODE != 0);

    /* Configure Message Buffer 0-7 for Transmission and assign priority*/
    C1TR01CONbits.TXEN0 = 0x1;
    C1TR01CONbits.TXEN1 = 0x0;
    C1TR23CONbits.TXEN2 = 0x0;
    C1TR23CONbits.TXEN3 = 0x0;
    C1TR01CONbits.TX0PRI = 0x3;
    C1RXFUL1 = 0x0;
    C1INTFbits.RBIF = 0;
        
}

void DMAInit(void)
{
    
    /* Data Transfer Size: Word Transfer Mode */
    DMA0CONbits.SIZE = 0x0;
    /* Data Transfer Direction: DMA RAM to Peripheral */
    DMA0CONbits.DIR = 0x1;
    /* DMA Addressing Mode: Peripheral Indirect Addressing mode */
    DMA0CONbits.AMODE = 0x2;
    /* Operating Mode: Continuous, Ping-Pong modes disabled */
    DMA0CONbits.MODE = 0x0;
    /* Assign ECAN1 Transmit event for DMA Channel 0 */
    DMA0REQ = 70;
    /* Set Number of DMA Transfer per ECAN message to 8 words */
    DMA0CNT = 7;
    /* Peripheral Address: ECAN1 Transmit Register */
    DMA0PAD = &C1TXD;
    /* Start Address Offset for ECAN1 Message Buffer 0x0000 */
    DMA0STA = __builtin_dmaoffset(ecan1MsgBuf);
    /* Channel Enable: Enable DMA Channel 0 */
    DMA0CONbits.CHEN = 0x1;
    /* Channel Interrupt Enable: Enable DMA Channel 0 Interrupt */
    IEC0bits.DMA0IE = 1;
    
    /* Data Transfer Size: Word Transfer Mode */
    DMA2CONbits.SIZE = 0x0;
    /* Data Transfer Direction: Peripheral to DMA RAM */
    DMA2CONbits.DIR = 0x0;
    /* DMA Addressing Mode: Peripheral Indirect Addressing mode */
    DMA2CONbits.AMODE = 0x2;
    /* Operating Mode: Continuous, Ping-Pong modes disabled */
    DMA2CONbits.MODE = 0x0;
    /* Assign ECAN1 Receive event for DMA Channel 2 */
    DMA2REQ = 34;
    /* Set Number of DMA Transfer per ECAN message to 8 words */
    DMA2CNT = 7;
    /* Peripheral Address: ECAN1 Receive Register */
    DMA2PAD = &C1RXD;
    /* Start Address Offset for ECAN1 Message Buffer 0x0000 */
    //DMA1STA = 0x0000;
    DMA2STA = __builtin_dmaoffset(ecan1MsgBuf);
    /* Channel Enable: Enable DMA Channel 1 */
    DMA2CONbits.CHEN = 0x1;
    /* Channel Interrupt Enable: Enable DMA Channel 1 Interrupt */
    IEC1bits.DMA2IE = 1;
    
}
#endif