/* 
 * File:   delay.c   
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

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in delay.h and insert code for user algorithms.> */

#include "delay.h"

unsigned int temp_count;

#if defined(__dsPIC33F__)

void Delay( unsigned int delay_count ) 
{
	temp_count = delay_count +1;
	asm volatile("outer: dec _temp_count");	
	asm volatile("cp0 _temp_count");
	asm volatile("bra z, done");
	asm volatile("do #3200, inner" );	
	asm volatile("nop");
	asm volatile("inner: nop");
	asm volatile("bra outer");
	asm volatile("done:");
}
	

void Delay_Us( unsigned int delayUs_count )
{
	temp_count = delayUs_count +1;
	asm volatile("outer1: dec _temp_count");	
	asm volatile("cp0 _temp_count");
	asm volatile("bra z, done1");
	asm volatile("do #1500, inner1" );	
	asm volatile("nop");
	asm volatile("inner1: nop");
	asm volatile("bra outer1");
	asm volatile("done1:");
}		
#elif defined(__PIC24H__)
void Delay( unsigned int delay_count ) 
{
	temp_count = delay_count +1;
	asm volatile("outer: dec _temp_count");	
	asm volatile("cp0 _temp_count");
	asm volatile("bra z, done");
	asm volatile("repeat #3200" );	
	asm volatile("nop");
	asm volatile("repeat #3200" );	
	asm volatile("nop");
	asm volatile("bra outer");
	asm volatile("done:");
}
	

void Delay_Us( unsigned int delayUs_count )
{
	temp_count = delayUs_count +1;
	asm volatile("outer1: dec _temp_count");	
	asm volatile("cp0 _temp_count");
	asm volatile("bra z, done1");
	asm volatile("repeat #1500" );	
	asm volatile("nop");
	asm volatile("repeat #1500" );	
	asm volatile("nop");
	asm volatile("bra outer1");
	asm volatile("done1:");
}

#endif