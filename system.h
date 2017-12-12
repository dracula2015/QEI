/* 
 * File:   system.h
 * Author: dracula
 * Comments:
 * Revision history: EDITION 0.1 
 */

// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_HEADER_SYSTEM_H
#define	XC_HEADER_SYSTEM_H

/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        7370000L
#define FCY             SYS_FREQ/2

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

#endif