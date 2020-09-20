/*
 * GROUP-24
 * Group Member 1: Emil Shikhaliyev
 *           
 * Group Member 2: Seda Civelek
 *                 
 */

#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include <xc.h>
#include "breakpoints.h"
/* The variables below are the binary values for 7-segment display*/
int dyok = 0b00000000;
int d0 = 0b00111111;
int d1 = 0b00000110;
int d2 = 0b01011011;
int d3 = 0b01001111;
int d4 = 0b01100110;
int d5 = 0b01101101;
int d6 = 0b01111101;
int d7 = 0b00000111;
int d8 = 0b01111111;
int d9 = 0b01101111;
int adcVal; //value readed from adresh and adresl
int guess; //guess value converted from adcVal
int timer1_5s = 0; //variable to control whether 5 second is passed or not 
int timer_for_blink = 0; //variable to control 500ms blinks  
int correct_guess_flag = 0; //flag that is set when guess is correct
int rb_flag = 0; //flag that is set when pushed the rb4 button more than 10ms 
int game_over_flag = 0; //flag that is set when 5 second is passed 
int timer_for_rb4 = 0; //variable to control rb4 is pushed more than 10 ms 

/* display function assigns the LATJ value to display correct value of
 7-segment display.*/
void display(int val){
    switch(val){
        case 0:
            LATJ = d0;
            break;
        case 1:
            LATJ = d1;
            break;
        case 2:
            LATJ = d2;
            break;
        case 3:
            LATJ = d3;
            break;
        case 4:
            LATJ = d4;
            break;
        case 5:
            LATJ = d5;
            break;
        case 6:
            LATJ = d6;
            break;
        case 7:
            LATJ = d7;
            break;
        case 8:
            LATJ = d8;
            break;
        case 9:
            LATJ = d9;
            break;
    }
}

/*This function is used for mapping the ADC values to 
 corresponding digits.*/
void adcToGuess(int val){
    if(0 <= val && val <= 102){
        guess= 0;
    }
    else if(102 < val && val <= 204){
        guess=1;
    }
    else if(204 < val && val <= 306){
        guess=2;
    }
    else if(306 < val && val <= 408){
        guess=3;
    }
    else if(408 < val && val <= 510){
        guess=4;
    }
    else if(510 < val && val <= 612){
        guess=5;
    }
    else if(612 < val && val <= 714){
        guess=6;
    }
    else if(714 < val && val <= 816){
        guess=7;
    }
    else if(816 < val && val <= 918){
        guess=8;
    }
    else if(918 < val && val <= 1023){
        guess=9;
    }
}
/*Hint function is used for comparing the assigned value by player and special number. If assigned value is less than correct
 number LATC,LATD and LATE leds configured as up arrow. If assigned value is more than correct number LATC,LATD and LATE  leds
 configured as down arrow. If the assigned value is correct leds are cleared.*/
void hint(void){
    // down arrow
    if(guess > special_number()){
        LATC = 0b00000100;
        LATD = 0b00001111;
        LATE = 0b00000100;
    }
    // up arrow
    else if(guess < special_number()){
        LATC = 0b00000010;
        LATD = 0b00001111;
        LATE = 0b00000010;
    }
    else {
        LATC = 0b00000000;
        LATD = 0b00000000;
        LATE = 0b00000000;
        correct_guess_flag = 1;
        correct_guess();
        latjh_update_complete();
    }
    latcde_update_complete();
}

/*We have used one interrupt service routine for 2 timer and 1 port change interrupt. 
 * 
 We configured timer0 in order to set GO/DONE and ADIE bits so that it
 triggers the ad conversion in every 50 ms. We used adc interrupt in order to start ad conversion.When the conversion is done and ADIF is set, we read the converted value and compare
 with correct number.If number is true correct_guess_flag is set and we started blinks. When blinks are over, game restarts.
 * 
 * 
 We used timer1 in order to restart game in every 5 second. We also used timer1 to do blink operation in 500ms intervals.
 Also we used timer1 to control whether rb4 is pushed more than 10 ms.
 * 
 *  
 We used rb port change interrupt. Rb4 is used to assign the values guessed by player. 
*/
void __interrupt() isr(void){
    if(TMR0IF){
        /*enter every 50 ms start ADC conversion
         ADIE=0, ADIF=1,G0=1
         update seven segment display*/
        //Start ADC
        ADIF = 0;
        ADIE = 1;
        ADCON0bits.GODONE = 1;
        TMR0H = 0xB; TMR0L = 0xDC;
        TMR0IF=0;
    }

    if(ADIF){ //Update seven-segment display in every 50ms 
        ADIF = 0;
        adcVal =  (ADRESH << 8) + ADRESL;
        adc_value = adcVal;
        adc_complete();
        if(!correct_guess_flag && !game_over_flag){
            adcToGuess(adcVal);
            display(guess);
            latjh_update_complete();
        }
    }
    if(TMR1IF){ //500ms for blink //5s for gameover
        if(rb_flag == 1) {
            ++timer_for_rb4;
        }
        ++timer1_5s;
        if(!correct_guess_flag && !game_over_flag && timer1_5s % 50 == 0 && timer1_5s < 500) {
                hs_passed();
        }
        if(correct_guess_flag || timer1_5s >= 500){
            if(!correct_guess_flag && !game_over_flag) {
                game_over();
                LATC = 0b00000000;
                LATD = 0b00000000;
                LATE = 0b00000000;
                game_over_flag = 1;
                latcde_update_complete();
                latjh_update_complete();
            }
            ++timer_for_blink;
            switch(timer_for_blink){
                case 1: 
                    display(special_number());
                    latjh_update_complete();
                    break;
                case 50:
                    LATJ = dyok;
                    hs_passed();
                    latjh_update_complete();
                    break;
                case 100:
                    display(special_number());
                    hs_passed();
                    latjh_update_complete();
                    break;
                case 150:
                    LATJ = dyok;
                    hs_passed();
                    latjh_update_complete();
                    break;
                case 200:
                    hs_passed();
                    latjh_update_complete();
                    restart();
                    timer_for_blink = 0;
                    timer1_5s = 0;
                    rb_flag = 0;
                    game_over_flag = 0;
                    correct_guess_flag = 0;
                    break;
            }
        }
        TMR1IF = 0;
        TMR1H = 0xCF; TMR1L = 0x2C;
        }
        if(rb_flag == 1 && PORTBbits.RB4 == 1 && timer_for_rb4 == 2 && !game_over_flag && !correct_guess_flag){
            rb4_handled();
            hint();
            latcde_update_complete();
            rb_flag = 0;
            timer_for_rb4 = 0;
        }
        else if(rb_flag == 0 ||  PORTBbits.RB4 == 0){
            rb_flag = 0;
            timer_for_rb4 = 0;
        }
        if(RBIF == 1){
            if(PORTBbits.RB4 == 1){
                rb_flag = 1;
            }
            else if(PORTBbits.RB4 == 0){
                rb_flag = 0;
            }
            RBIF = 0;
        }
}

//ADC configuration
void setupADC(){
    ADCON0=0b00110000; //AN12 is selected, GODONE and ADON is 0
    ADCON1=0b00000000; 
    ADCON2=0b10101110; //ADCS=fosc/64, ACQT=12Tad ADFM=1(Right justified)
    ADIE=1;
    ADON=1;
}

// Timer0 configuration
void setupTimer0(){ //50ms interrupt
    T0CON = 0b00000010;         //Prescaler 1:8 16-bit
    TMR0H = 0xB; TMR0L = 0xDC;  //Preload value 3036 in decimal
    TMR0IE = 1;                 //Enable timer0 interrupt
    TMR0ON = 1;                 //Enable for starting adc
}

// Timer1 configuration
void setupTimer1(){ //10ms interrupt
    T1CON = 0b11110000;         //Prescaler 1:8 16-bit
    TMR1H = 0xCF; TMR1L = 0x2C;  //Preload value 53036 in decimal
    TMR1IE = 1;                 //enable timer1 interrupt
    TMR1ON = 1;                 //enable timer1

}
//RB4 change interrupt configuration
void setupRB(){
    RBIE=1; //enable portb change interrupt
    RBIF=0; //clear portb change interrupt flag 
}

void ports_init(void){
    TRISH = 0b11110000;
    LATC = 0b00000000;
    LATD = 0b00000000;
    LATE = 0b00000000;
    LATHbits.LH0 = 0; //Porth is configured to display seven segment at D0.
    LATHbits.LH1 = 0;
    LATHbits.LH2 = 0;
    LATHbits.LH3 = 1;
    TRISJ = 0b00000000;
    TRISB = 0b00010000; //RB4 is configured as input. 
    TRISC = 0b00000000; //All PORTC,PORTD,PORTE pins configured as output.
    TRISD = 0b00000000;
    TRISE = 0b00000000;
}

void main(void) {
    //Round robin with interrupt approach
    
    ports_init();
    init_complete();
    setupADC();
    setupTimer1();
    setupTimer0();
    setupRB();
    GIE=1;
    PEIE=1;
    while(1){
        
    }
    return;
}
