/* 
 * File:   delay.h
 * Author: dracula
 * Comments:
 * Revision history: EDITION 0.1
 */

// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_HEADER_DELAY_H
#define	XC_HEADER_DELAY_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define Fcy  40000000

void Delay( unsigned int delay_count );
void Delay_Us( unsigned int delayUs_count );

#define Delay200uS_count  (Fcy * 0.0002) / 1080
#define Delay_1mS_Cnt	  (Fcy * 0.001) / 2950
#define Delay_2mS_Cnt	  (Fcy * 0.002) / 2950
#define Delay_5mS_Cnt	  (Fcy * 0.005) / 2950
#define Delay_15mS_Cnt 	  (Fcy * 0.015) / 2950
#define Delay_1S_Cnt	  (Fcy * 1) / 2950 
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

