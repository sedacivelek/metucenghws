; Seda	Civelek 
; Emil	Shikhaliyev 
; Our choice is THE_v2 on simulation environment
; We tested it on board. It works same as simulation environment
    
    LIST P=18F8722
    #INCLUDE<p18f8722.inc>
    config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF
    
    hp udata 0x20
    hp
    
    level udata 0x21
    level
    
    factor udata 0x22
    factor
    
    timer1vall udata 0x23
    timer1vall
    
    modul udata 0x24
    modul
        
    timer1valh udata 0x25
    timer1valh
    
    left udata 0x26
    left
    
    right udata 0x27
    right
    
    loop2 udata 0x28
    loop2
    
    loop3 udata 0x29
    loop3 
    
    ballcount udata 0x30
    ballcount
    
    place_of_board udata 0x31
    place_of_board
    
    board_line udata 0x32
    board_line
    
    util udata 0x33
    util       
    
    temp udata 0x34
    temp
    
    temp2 udata 0x35
    temp2
    
    seg1 udata 0x36
    seg1
    
    seg2 udata 0x37
    seg2
    
    org 0x00
    goto init
    
    org 0x08
    goto isr 
    
    init: 
	clrf INTCON
	clrf INTCON2
	clrf RCON
	clrf PIE1
	clrf TMR1L
	clrf TMR1H
	clrf LATA
	clrf LATB
	clrf LATC
	clrf LATD
	clrf LATG
	clrf LATH
	clrf LATJ
	
	clrf TRISA
	clrf TRISB
	clrf TRISC
	clrf TRISD
	clrf TRISG
	clrf TRISH
	clrf TRISJ
	
	clrf PORTA
	clrf PORTB
	clrf PORTC
	clrf PORTD
	clrf PORTG
	clrf PORTH
	clrf PORTJ
	clrf ballcount
	clrf util
	clrf loop3
	clrf loop2
	clrf temp
	clrf temp2
	clrf right
	clrf left
	movlw h'0f'
	movwf ADCON1
	movlw b'00001101'
	movwf TRISG
	
	movlw b'01000111' ;disable timer0 8-bit prescaler 256
	movwf T0CON
	
	movlw b'11000000'
	movwf T1CON
	
	movlw	b'11000000'
	movwf	board_line
	movwf	place_of_board
	bsf	PORTA,	5 ;initial bar location
	bsf	LATA,	5
	bsf	PORTB,	5
	bsf	LATB,	5
	
	movlw h'05'
	movwf hp 
	
	movlw h'64'
	movwf factor
	movlw h'01'
	movwf level
	movlw b'10100000' ;global and timer0 overflow interrupts enabled
	movwf INTCON
	bsf T1CON,0 ;timer1 started
	
    main:
	btfss	PORTG, 0 
	goto	main
	bsf	T0CON, 7
	movlw d'61'
	movwf TMR0
	
	movff TMR1L,timer1vall
	movff TMR1H,timer1valh
	movlw h'03'
	movwf loop2
	movlw h'05'
	movwf loop3
	
    mainloop:
	btfsc PORTG,2
	call moveright 
	btfsc PORTG,3
	call moveleft
	btfsc util, 0
	goto updateIsr
	btfss util, 7
	goto gameUpdate
	call show
	goto mainloop
	
    isr:
	btfss INTCON,2
	retfie FAST
	bcf INTCON,2
	decf factor
	movlw d'61'
	movwf TMR0
	bsf util,0
	retfie FAST
    
    updateIsr:
	bcf util,0
	movlw h'0'
	cpfseq factor
	goto mainloop
	
    gameUpdate:
    	btfss	util,6
	incf	ballcount
	movlw	h'04' ;h'06' da olabilir bu 
	cpfslt	ballcount
	goto	levelball1;level2 ye ge?mi? olcaz
	movlw	h'64'
	movwf	factor
	goto	updateAll
	levelball1:
	    movlw   h'02'
	    movwf   level
	    movlw   h'0e' ;h'10' da olabilir bu
	    cpfslt  ballcount
	    goto    levelball2
	    movlw   h'50'
	    movwf   factor
	    goto    updateAll
	    ;yukar?daki gotonun ayn?s?
	    levelball2:
		movlw	h'03'
		movwf	level
		movlw	h'1e'
		cpfslt	ballcount
		bsf	util,6 ;dontcreateball anymore 
		movlw	h'46'
		movwf	factor
		goto	updateAll
	
    updateAll:
	adakileri_hallet:
	    movff	PORTA, temp
	    btfsc	place_of_board, 7
	    goto	handle_board_and_a
	    rlncf	temp, 1
	    btfsc	temp, 6
	    call	decr
	    movff	temp, PORTA
	    goto	bdekileri_hallet
	handle_board_and_a:
	    rlncf   temp,   1
	    bcf	    temp,   6
	    bsf	    temp,   5
	    movff   temp,  PORTA
	
	bdekileri_hallet:
	    movff	PORTB, temp
	    btfsc	place_of_board, 6
	    goto	handle_board_and_b
	    rlncf	temp, 1
	    btfsc	temp, 6
	    call	decr
	    movff	temp, PORTB
	    goto	cdekileri_hallet
	handle_board_and_b:
	    rlncf   temp,   1
	    bcf	    temp,   6
	    bsf	    temp,   5
	    movff   temp,  PORTB

	cdekileri_hallet:
	    movff	PORTC, temp
	    btfsc	place_of_board, 5
	    goto	handle_board_and_c
	    rlncf	temp, 1
	    btfsc	temp, 6
	    call	decr
	    movff	temp, PORTC
	    goto	ddekileri_hallet
	handle_board_and_c:
	    rlncf   temp,   1
	    bcf	    temp,   6
	    bsf	    temp,   5
	    movff   temp,  PORTC
	    
	ddekileri_hallet:
	    movff	PORTD, temp
	    btfsc	place_of_board, 4
	    goto	handle_board_and_d
	    rlncf	temp, 1
	    btfsc	temp, 6
	    call	decr
	    movff	temp, PORTD
	    goto	ball_creation
	handle_board_and_d:
	    rlncf   temp,   1
	    bcf	    temp,   6
	    bsf	    temp,   5
	    movff   temp,  PORTD
	    
	ball_creation:
	    movlw   d'30'
	    cpfseq  ballcount
	    call    createball
	    bsf	    util,7
	    
	ballfinish:
	    movlw   d'30'
	    cpfsgt  ballcount
	    goto    ballfinishes
	    goto    mainloop
	    
	ballfinishes:
	    btfsc PORTA,0
	    goto  mainloop
	    btfsc PORTA,1
	    goto mainloop
	    btfsc PORTA,2
	    goto mainloop
	    btfsc PORTA,3
	    goto mainloop
	    btfsc PORTA,4
	    goto mainloop
	    
	    btfsc PORTB,0
	    goto  mainloop
	    btfsc PORTB,1
	    goto mainloop
	    btfsc PORTB,2
	    goto mainloop
	    btfsc PORTB,3
	    goto mainloop
	    btfsc PORTB,4
	    goto mainloop 
	    
	    btfsc PORTC,0
	    goto  mainloop
	    btfsc PORTC,1
	    goto mainloop
	    btfsc PORTC,2
	    goto mainloop
	    btfsc PORTC,3
	    goto mainloop
	    btfsc PORTC,4
	    goto mainloop
	    
	    btfsc PORTD,0
	    goto  mainloop
	    btfsc PORTD,1
	    goto mainloop
	    btfsc PORTD,2
	    goto mainloop
	    btfsc PORTD,3
	    goto mainloop
	    btfsc PORTD,4
	    goto mainloop
	    
	    btfsc PORTA,5
	    goto testbara
	testb:
	    btfsc PORTB,5
	    goto testbarb
	testc:
	    btfsc PORTC,5
	    goto testbarc
	testd:
	    btfsc PORTD,5
	    goto testbard
	    
	    goto game_over
	    
	    
	    
	testbara:
	    btfss place_of_board,7
	    goto mainloop
	    goto testb
	    
	testbarb:
	    btfss place_of_board,6
	    goto mainloop
	    goto testc
	    
	testbarc:
	    btfss place_of_board,5
	    goto mainloop
	    goto testd
	    
	testbard:
	    btfss place_of_board,4
	    goto mainloop
	    goto game_over
	    
	decr:
	    bcf	    temp, 6
	    DCFSNZ  hp
	    goto    game_over
	    return
	
    createball:
	btfss timer1vall,0
	goto xx_0
	goto xx_1
	
	xx_0:
	    btfss timer1vall,1
	    goto xx_00
	    goto xx_10
	xx_1:
	    btfss timer1vall,1
	    goto xx_01
	    goto xx_11
	    
	    xx_00:
	    btfss timer1vall,2
	    goto xx_000
	    goto xx_100
	    xx_01:
	    btfss timer1vall,2
	    goto xx_001
	    goto xx_101
	    xx_10:
	    btfss timer1vall,2
	    goto xx_010
	    goto xx_110
	    xx_11:
	    btfss timer1vall,2
	    goto xx_011
	    goto xx_111
	    
		xx_000:
		    bsf PORTA,0
		    bsf LATA,0
		    movlw h'01'
		    cpfseq level
		    goto notlevel01
		    goto level1shift
		    
		    notlevel01:
			movlw h'02'
			cpfseq level
			goto level3shift
			goto level2shift
		    
		xx_001:
		    bsf PORTB,0
		    bsf LATB,0
		    movlw h'01'
		    cpfseq level
		    goto notlevel11
		    goto level1shift
		    
		    notlevel11:
			movlw h'02'
			cpfseq level
			goto level3shift
			goto level2shift
		    
		xx_010:
		    bsf PORTC,0
		    bsf LATC,0
		    movlw h'01'
		    cpfseq level
		    goto notlevel21
		    goto level1shift
		    
		    notlevel21:
			movlw h'02'
			cpfseq level
			goto level3shift
			goto level2shift
		    
		xx_011:
		    bsf PORTD,0
		    bsf LATD,0
		    movlw h'01'
		    cpfseq level
		    goto notlevel31
		    goto level1shift
		    
		    notlevel31:
			movlw h'02'
			cpfseq level
			goto level3shift
			goto level2shift
		    
		xx_100:
		    goto xx_000
		    
		xx_101:
		    goto xx_001
		    
		xx_110:
		    goto xx_010
		    
		xx_111:
		    goto xx_011
		
		level1shift:
			btfss timer1valh,0
			goto sifirla
			goto birle
			
			sifirla:
			movlw h'00'
			movwf left
			goto rightbit
			birle:
			movlw h'01'
			movwf left
			goto rightbit
			
			rightbit:
			btfss timer1vall,0
			goto sifirlar
			goto birler
			
			sifirlar:
			movlw h'00'
			movwf right
			goto bitmanipule
			birler:
			movlw h'01'
			movwf right
			
			bitmanipule:
			rrncf timer1valh
			rrncf timer1vall
			btfss left,0
			goto lclear
			goto lset
			lclear:
			bcf timer1vall,7
			goto rmanipule
			lset:
			bsf timer1vall,7
			rmanipule:
			btfss right,0
			goto rclear
			goto rset
			rclear:
			bcf timer1valh,7
			goto son
			rset:
			bsf timer1valh,7
			son:
			return
		    level2shift:
			btfss timer1valh,0
			goto sifirla2
			goto birle2
			
			sifirla2:
			movlw h'00'
			movwf left
			goto rightbit2
			birle2:
			movlw h'01'
			movwf left
			
			rightbit2:
			btfss timer1vall,0
			goto sifirlar2
			goto birler2
			
			sifirlar2:
			movlw h'00'
			movwf right
			goto bitmanipule2
			birler2:
			movlw h'01'
			movwf right
			
			bitmanipule2:
			rrncf timer1valh
			rrncf timer1vall
			btfss left,0
			goto lclear2
			goto lset2
			lclear2:
			bcf timer1vall,7
			goto rmanipule2
			lset2:
			bsf timer1vall,7
			rmanipule2:
			btfss right,0
			goto rclear2
			goto rset2
			rclear2:
			bcf timer1valh,7
			goto son2
			rset2:
			bsf timer1valh,7
			son2:
			decfsz loop2
			goto level2shift
			movlw	h'03'
			movwf	loop2
			return
		    level3shift:
			btfss timer1valh,0
			goto sifirla3
			goto birle3
			
			sifirla3:
			movlw h'00'
			movwf left
			goto rightbit3
			birle3:
			movlw h'01'
			movwf left
			goto rightbit3
			
			rightbit3:
			btfss timer1vall,0
			goto sifirlar3
			goto birler3
			
			sifirlar3:
			movlw h'00'
			movwf right
			goto bitmanipule3
			birler3:
			movlw h'01'
			movwf right
			
			bitmanipule3:
			rrncf timer1valh
			rrncf timer1vall
			btfss left,0
			goto lclear3
			goto lset3
			lclear3:
			bcf timer1vall,7
			goto rmanipule3
			lset3:
			bsf timer1vall,7
			rmanipule3:
			btfss right,0
			goto rclear3
			goto rset3
			rclear3:
			bcf timer1valh,7
			goto son3
			rset3:
			bsf timer1valh,7
			son3:
			
			decfsz loop3
			goto level3shift
			movlw	h'05'
			movwf	loop3
			return
    moveright:
	btfsc	PORTG, 2
	goto	moveright
	movlw	b'00110000'
	cpfseq	place_of_board
	goto	moverright
	return
	moverright:
	    btfsc   PORTA, 5
	    bsf	    board_line, 7
	    btfsc   PORTB, 5
	    bsf	    board_line, 6
	    btfsc   PORTC, 5
	    bsf	    board_line, 5
	    btfsc   PORTD, 5
	    bsf	    board_line, 4
	    movf    place_of_board, W
	    xorwf   board_line, 1
	    
	    movf    place_of_board, W
		xorlw    b'11000000'
		bz	 ab_bc
	    movf    place_of_board, W
		xorlw	b'01100000'
		bz	bc_cd
	
	    ab_bc:
		movf	board_line, W
		iorlw	b'01100000'
		movwf	board_line
		rrncf	place_of_board, 1
		bcf	LATA, 5
		bsf	LATC, 5
		clrf	board_line
		return
	    bc_cd:
		movf	board_line, W
		iorlw	b'00110000'
		movwf	board_line
		rrncf	place_of_board, 1
		bcf	LATB, 5
		bsf	LATD, 5
		clrf	board_line
		return
		
    moveleft:
	btfsc	PORTG, 3
	goto	moveleft
	movlw	b'11000000'
	cpfseq	place_of_board
	goto	movelleft
	return
	movelleft:
	    btfsc   PORTA, 5
	    bsf	    board_line, 7
	    btfsc   PORTB, 5
	    bsf	    board_line, 6
	    btfsc   PORTC, 5
	    bsf	    board_line, 5
	    btfsc   PORTD, 5
	    bsf	    board_line, 4
	    movf    place_of_board, W
	    xorwf   board_line, 1

	    movf    place_of_board, W
		xorlw   b'00110000'
		bz	    dc_cb
	    movf    place_of_board, W
		xorlw   b'01100000'
		bz	    cb_ba

	     dc_cb:
		movf	board_line, W
		iorlw	b'01100000'
		movwf	board_line
		rlncf	place_of_board, 1
		bcf	LATD, 5
		bsf	LATB, 5
		clrf	board_line
		return

	    cb_ba:
		movf	board_line, W
		iorlw	b'11000000'
		movwf	board_line
		rlncf	place_of_board, 1
		bcf	LATC, 5
		bsf	LATA, 5
		clrf	board_line
		
		return
	 
	LevelSegment:
	    btfss level,0
	    goto seg_0
	    goto seg_1
	    
	    seg_0:
		btfss level,1
		goto seg_00
		goto seg_10
	    seg_1:
		btfss level,1
		goto seg_01
		goto seg_11
		
	seg_00:
	    movlw b'00111111'
	    movwf seg1
	    return
	seg_01:
	    movlw b'00000110'
	    movwf seg1
	    return
	seg_10:
	    movlw b'01011011'
	    movwf seg1
	    return
	seg_11:
	    movlw  b'01001111' 
	    movwf seg1
	    return
HpSegment:
	    btfss hp,0
	    goto segh_0
	    goto segh_1
	    
	    segh_0:
		btfss hp,1
		goto segh_00
		goto segh_10
	    segh_1:
		btfss hp,1
		goto segh_01
		goto segh_11
		
	segh_00:
		btfss hp,2
		goto segh_000
		goto segh_100
	    
	segh_01:
		btfss hp,2
		goto segh_001
		goto segh_101
	    
	segh_10:
		btfss hp,2
		goto segh_010
		goto segh_110
	    
	segh_11:
		btfss hp,2
		goto segh_011
		goto segh_111
	segh_000:
	    movlw b'00111111'
	    movwf seg2
	    return
	segh_001:
	    movlw b'00000110'
	    movwf seg2
	    return
	segh_010:
	    movlw b'01011011'
	    movwf seg2
	    return
	segh_011:
	    movlw  b'01001111' 
	    movwf seg2
	    return
	segh_100:
	    movlw  b'01100110'
	    movwf seg2
	    return
	segh_101:
	    movlw b'01101101'
	    movwf seg2
	    return
	segh_110:
	    movlw b'01111101'
	    movwf seg2
	    return
	segh_111:
	    movlw b'00000111'
	    movwf seg2
	    return
    show:
	call LevelSegment
	bcf LATH ,1
	bcf LATH ,2
	bcf LATH, 3
	bsf LATH ,0
	movff seg1, LATJ
	call HpSegment
	bcf LATH,2
	bcf LATH,1
	bcf LATH,0
	bsf LATH,3
	movff seg2, LATJ
	return

    game_over:
	goto init
end
