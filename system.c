/* 
 * File:   system.c   
 * Author: dracula
 * Comments:
 * Revision history: EDITION 0.1 
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

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

#include "system.h"          /* variables/params used by system.c             */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c.                                          */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.*/

/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void)
{
    // Configure Oscillator to operate the device at 40Mhz
    // Fosc = Fin*M/(N1*N2), Fcy = Fosc/2
    // Fosc = 7.3728M*43/(2*2) = 80Mhz for 7.3728M input clock, Fcy=40Mhz
	PLLFBD=41;					// M=43
	CLKDIVbits.PLLPOST=0;		// N1=2
	CLKDIVbits.PLLPRE=0;		// N2=2
	OSCTUN=0x0015;              // Tune FRC oscillator 8Mhz, if FRC is used
    // Disable Watch Dog Timer
	RCONbits.SWDTEN=0;

    /*MPLAB® C Compiler for PIC24 MCUs and dsPIC DSCs provides the following built-in C language functions for unlocking and writing to the OSCCON register:*/
    //__builtin_write_OSCCONH(value)
    //__builtin_write_OSCCONL(value)
    /*See MPLAB C Compiler Help for more information.*/
    
    // Clock switch to incorporate PLL
	__builtin_write_OSCCONH(0x03);		// Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)
	__builtin_write_OSCCONL(0x01);		// Start clock switching
    //OSCCONbits.NOSC=0b011;
    //OSCCONbits.OSWEN=1;
    //OSCCON=0x0301;
	while(OSCCONbits.COSC != 0b011);    // Wait for Clock switch to occur	
	while(OSCCONbits.LOCK !=1);         // Wait for PLL to lock, only if PLL is needed
    

#if 0
        /* Disable Watch Dog Timer */
        RCONbits.SWDTEN = 0;

        /* When clock switch occurs switch to Primary Osc (HS, XT, EC) */
        __builtin_write_OSCCONH(0x02);  /* Set OSCCONH for clock switch */
        __builtin_write_OSCCONL(0x01);  /* Start clock switching */
        while(OSCCONbits.COSC != 0b010);
        /* Wait for Clock switch to occur */
        /* Wait for PLL to lock, only if PLL is needed */
        /* while(OSCCONbits.LOCK != 1); */
#endif
}