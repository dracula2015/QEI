/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file, thus actually include <p33FJ128MC804.h> */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>
//#include <p33FJ128MC804.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/* TODO DSPIC33FJ128MC804 Configuration Bit Settings*/
// 'C' source line config statements
// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

/*
_FOSCSEL(FNOSC_FRC);                                  // Select Internal FRC at POR
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);       // Enable Clock Switching and Configure Posc in XT mode
_FPOR(RST_PWMPIN & PWM1H_ACT_HI & PWM1L_ACT_HI);      // High and Low switches set to active-high state 
*/

// FOSCSEL
#pragma config FNOSC = PRI              // Primary Oscillator (XT, HS, EV))
#pragma config IESO = ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = XT              // XT Oscillator Mode
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
#pragma config FCKSM = CSECMD           // This bit is extremely important? if set to CSDCMD there will be no PWM signal!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#pragma config FCKSM = CSDCMD         // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer always enabled)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
#pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = OFF             // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD3               // Comm Channel Select (Communicate on PGC3/EMUC3 and PGD3/EMUD3)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)
/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
char ReceivedChar;
char TransmitChar;
bool go = 0;
bool stop = 0;
bool direction = 0;
/* i.e. uint16_t <variable_name>; */
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int count[2]={0,0};
int motor = 0;
int i=0;
int position = 0;
int posHigh = 0;
//int posLow = 0;
int QEIPosHigh = 0;
int QEIPosLow = 0;
int main(void)
{   
    
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
    InitApp();
    /* TODO <INSERT USER APPLICATION CODE HERE> */
 
    while(1)
    {
        if(U1STAbits.PERR==1)
        {
            continue;
        }
        if(U1STAbits.OERR==1)
        {
            //LATAbits.LATA0=1;
            U1STAbits.OERR=0;
            //receivedNumber++;
            continue;
        }
        if(U1STAbits.URXDA==1)
        {   
            //LATAbits.LATA1=1;
        }
        if(stop){
        LATAbits.LATA7=1;
        }
        
        if(go){
        LATAbits.LATA7=0;
        }
          
        if(i==0)
        {            
            motor = count[0];
            motor = motor & 0x00FF;
            motor = motor | (count[1]<<8);
        }
        
        {
            int temp=0;
            temp = motor & 0x8000;
            if(temp)
            {
                motor=motor & 0x7fff ;
                //motor[j]=~(motor[j]-1);
                direction=0;
            }else{direction=1;}
        }
        LATAbits.LATA10=direction;
        P2DC1=(5*motor/3);
        
        if(posHigh < 0)
        {
            QEIPosHigh = (~posHigh) | 0x8000; 
            QEIPosLow = (~POS1CNT +1);
        }
        else
        {
            QEIPosHigh = posHigh & 0x7fff;
            QEIPosLow = POS1CNT;
        }
    };
}

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
    ReceivedChar = U1RXREG;
    U1TXREG = ReceivedChar;
    if(ReceivedChar == 'g'){go = 1;}
    else if(ReceivedChar == 's'){stop = 1; go = 0;}
    else 
    {
        
    if(ReceivedChar == 'u'){ U1TXREG = 'u'; i = 0;}

    else
    {
        count[i] = ReceivedChar;
        i++;
        if(i>=2) i = 0;
    }
    
    }
    U1TXREG = QEIPosHigh >> 8;
    U1TXREG = QEIPosHigh;
    U1TXREG = QEIPosLow >> 8;
    U1TXREG = QEIPosLow;
    IFS0bits.U1RXIF = 0;
}

void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0; // clear TX interrupt flag
    //U1TXREG = 'b'; // Transmit one character
}

void __attribute__((interrupt, auto_psv)) _QEI1Interrupt(void)
{
    U1TXREG = QEIPosHigh >> 8;
    U1TXREG = QEIPosHigh;
    U1TXREG = QEIPosLow >> 8;
    U1TXREG = QEIPosLow;
    //U1TXREG = QEI1CONbits.UPDN;
    
    if(QEI1CONbits.UPDN == 1){posHigh += 1;}
    else {posHigh -= 1;}
    IFS3bits.QEI1IF = 0;
}