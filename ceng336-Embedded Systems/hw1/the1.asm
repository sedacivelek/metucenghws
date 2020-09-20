LIST    P=18F8722

#INCLUDE <p18f8722.inc> 

CONFIG OSC=HSPLL, FCMEN=OFF, IESO=OFF,PWRT=OFF,BOREN=OFF, WDT=OFF, MCLRE=ON, LPT1OSC=OFF, LVP=OFF, XINST=OFF, DEBUG=OFF
    
var1 udata h'20'
var1

var2 udata h'21'
var2
 
var3 udata h'22'
var3

ports udata h'23'
ports
 
aos udata h'24'
aos

first udata h'25'
first
 
second udata h'26'
second

ORG 0x0000            
    goto    start 
    
start:    
    call init
    call mainloop
    
turn_on:
    movlw b'00001111'
    movwf LATB
    movwf LATC
    movlw b'11111111'
    movwf LATD
    return
    
turn_off:   
    clrf LATB
    clrf LATC
    clrf LATD
    return
    
led_config:
    movlw b'00010000'
    movwf TRISA
    movlw b'00011000'
    movwf TRISE
    movlw b'00000000'
    movwf TRISB
    movwf TRISC
    movwf TRISD
    return
    
delay:
    movlw 0xBE
    movwf var1
    loop1:
	movlw 0xB2
	movwf var2
	loop2:
	    movlw 0x61
	    movwf var3
	    inner:
		decfsz var3, F
		goto inner
		decfsz var2, F
		goto loop2
		decfsz var1, F
		goto loop1
    return
    
    
init:
    movlw 0FH
    movwf ADCON1
    call led_config
    call turn_on
    call delay
    call turn_off
    return
    
mainloop:
    
    clrf aos
    clrf first
    clrf second
    clrf ports
    
    
    RA4press:
	btfss PORTA,4 
	goto RA4press    
    RA4release:
	btfsc PORTA,4 
	goto RA4release  
	INCF aos
	
	
	RE3press: 
	    btfsc PORTA,4 
	    goto RA4release 
	    btfss PORTE,3 
	    goto RE3press
	RE3release:
	    btfsc PORTE,3
	    goto RE3release
	    INCF ports
	    
	    movf ports,W
	    xorlw 1
	    bz caseB
	    movf ports,W
	    xorlw 2
	    bz caseC
	    movf ports,W
	    xorlw 3
	    bz caseD
	    
	    
	    caseB:
		RE4press0:
		    clrf first
		    clrf LATB
		RE4release0:
		    btfsc PORTE,4
		    goto RE4release0
		RE4press1:
		    
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press1
		RE4release1:
		    btfsc PORTE,4
		    goto RE4release1
		movlw b'00000001'
		movwf LATB
		INCF first
		RE4press2:
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press2
		RE4release2:
		    btfsc PORTE,4
		    goto RE4release2
		movlw b'00000011'
		movwf LATB
		INCF first
	    
		RE4press3:
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press3
		RE4release3:
		    btfsc PORTE,4
		    goto RE4release3
		movlw b'00000111'
		movwf LATB
		INCF first
		RE4press4:
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press4
		RE4release4:
		    btfsc PORTE,4
		    goto RE4release4
		movlw b'00001111'
		movwf LATB
		INCF first
		RE4press5:
		    btfsc PORTE,4
		    goto RE4press0
		    btfss PORTE,3
		    goto RE4press5

	    caseC:
		RE4press0C:
		    clrf second
		    clrf LATC
		RE4release0C:
		    btfsc PORTE,4
		    goto RE4release0C
		RE4press1C: 
		    clrf second
		    clrf LATC
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press1C
		RE4release1C:
		    btfsc PORTE,4
		    goto RE4release1C
		movlw b'00000001'
		movwf LATC
		INCF second
		RE4press2C:
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press2C
		RE4release2C:
		    btfsc PORTE,4
		    goto RE4release2C
		movlw b'00000011'
		movwf LATC
		INCF second
	    
		RE4press3C:
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press3C
		RE4release3C:
		    btfsc PORTE,4
		    goto RE4release3C
		movlw b'00000111'
		movwf LATC
		INCF second
		RE4press4C:
		    btfsc PORTE,3
		    goto RE3release
		    btfss PORTE,4
		    goto RE4press4C
		RE4release4C:
		    btfsc PORTE,4
		    goto RE4release4C
		movlw b'00001111'
		movwf LATC
		INCF second
		RE4press5C:
		    btfsc PORTE,4
		    goto RE4press0C
		    btfss PORTE,3
		    goto RE4press5C
		
	    caseD:
		btfss aos,0
		goto substraction
		goto addition
		
		substraction:
		movf second,W
		cpfslt first
		goto firstbigger
		goto firstless
		
		firstbigger:
		    movf second,W
		    subwf first
		    movf first,W
		    xorlw 0
		    bz sw0
		    movf first,W
		    xorlw 1
		    bz sw1
		    movf first,W
		    xorlw 2
		    bz sw2
		    movf first,W
		    xorlw 3
		    bz sw3
		    movf first,W
		    xorlw 4
		    bz sw4
		    movf first,W
		    xorlw 5
		    bz sw5
		    movf first,W
		    xorlw 6
		    bz sw6
		    movf first,W
		    xorlw 7
		    bz sw7
		    movf first,W
		    xorlw 8
		    bz sw8
		firstless:
		    movf first,W
		    subwf second
		    movf second,W
		    xorlw 0
		    bz sw0
		    movf second,W
		    xorlw 1
		    bz sw1
		    movf second,W
		    xorlw 2
		    bz sw2
		    movf second,W
		    xorlw 3
		    bz sw3
		    movf second,W
		    xorlw 4
		    bz sw4
		    movf second,W
		    xorlw 5
		    bz sw5
		    movf second,W
		    xorlw 6
		    bz sw6
		    movf second,W
		    xorlw 7
		    bz sw7
		    movf second,W
		    xorlw 8
		    bz sw8
		    
		
    
		addition:
		
		movf first,W
		addwf second, 1
		movf second,W
		xorlw 0
		bz sw0
		movf second,W
		xorlw 1
		bz sw1
		movf second,W
		xorlw 2
		bz sw2
		movf second,W
		xorlw 3
		bz sw3
		movf second,W
		xorlw 4
		bz sw4
		movf second,W
		xorlw 5
		bz sw5
		movf second,W
		xorlw 6
		bz sw6
		movf second,W
		xorlw 7
		bz sw7
		movf second,W
		xorlw 8
		bz sw8
		
		sw0:
		   movlw b'00000000'
		   movwf LATD
		   goto delay1
		sw1:
		   movlw b'00000001'
		   movwf LATD
		   goto delay1
		sw2:
		   movlw b'00000011'
		   movwf LATD
		   goto delay1
		sw3:
		   movlw b'00000111'
		   movwf LATD
		   goto delay1
		sw4:
		   movlw b'00001111'
		   movwf LATD
		   goto delay1
		sw5:
		   movlw b'00011111'
		   movwf LATD
		   goto delay1
		sw6:
		   movlw b'00111111'
		   movwf LATD
		   goto delay1
		sw7:
		   movlw b'01111111'
		   movwf LATD
		   goto delay1
		sw8:
		   movlw b'11111111'
		   movwf LATD
		   goto delay1
		   
		
    delay1:
	movlw 0xBE
	movwf var1
	loop11:
	    movlw 0xB2
	    movwf var2
	    loop21:
		movlw 0x61
		movwf var3
		inner1:
		    decfsz var3, F
		    goto inner1
		    decfsz var2, F
		    goto loop21
		    decfsz var1, F
		    goto loop11
	clrf LATB
	clrf LATC
	clrf LATD
	goto mainloop
	
end  
    
