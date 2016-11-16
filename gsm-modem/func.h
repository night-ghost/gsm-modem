
void print_SNS(const char *s1, unsigned long n, const char *s2) {
    serial.print_P(s1);
    serial.print(n);
    serial.print_P(s2);
}



void delay_1(){
    delay(1);
}

void delay_10(){
    delay(10);
}

void delay_50(){
    delay(50);
    // delay_10(); delay_10(); delay_10(); delay_10(); delay_10(); better on 2 bytes only
}

void delay_100(){
    delay_50();
    delay_50();
}

void delay_300(){
    delay_100();   delay_100();   delay_100();
}

void delay_1000(){
    delay(1000);
}

// чтение пакетов нужного протокола
bool getData(){
//LED_BLINK;
    byte sts;

    if(lflags.data_link_active || lflags.data_mode){
again:
#if defined(USE_MAVLINK)
	sts=read_mavlink();
#elif defined(USE_UAVTALK)
	sts=uavtalk_read();
#elif defined(USE_MWII)
	sts=mwii_read();
#else
#error no protocol defined
#endif

	lflags.data_mode=lflags.data_link_active; // если не пришло валидного пакета то сбросится
	return sts;
    } else {


#if defined(AUTOBAUD)
	    serial.end();
	    Red_LED_ON;
	    Green_LED_ON; // two leds together = no valid data
	    static uint8_t last_pulse = 15;
	    uint32_t pt = millis() + 100; // не более 0.1 секунды
	
	    uint8_t pulse=255;
	    
	    for(byte i=250; i!=0; i--){
	        long t=pulseIn(PD0, 0, 2500); // 2500uS * 250 = 
	        if(t>255) continue;	     // too long - not single bit
	        uint8_t tb = t;       // it less than 255 so convert to byte
	        if(tb==0) continue;   // no pulse at all
	        if(tb<pulse) pulse=tb;// find minimal possible - it will be bit time
	        
	        if(millis()>pt) break; 
	    }
	    
	    long speed;
	    
	    if(pulse == 255)    pulse = last_pulse; // no input at all - use last
	    else                last_pulse = pulse; // remember last correct time
	
	// F_CPU   / BAUD for 115200 is 138
	// 1000000 / BAUD for 115200 is 8.68uS
	//  so I has no idea about pulse times - they are simply measured
	
	    if(     pulse < 11) 	speed = 115200;
	    else if(pulse < 19) 	speed =  57600;
	    else if(pulse < 29) 	speed =  38400;
	    else if(pulse < 40) 	speed =  28800;
	    else if(pulse < 60) 	speed =  19200;
	    else if(pulse < 150)	speed =   9600;
	    else                        speed =   4800;

	    serial.begin(speed);
	    Green_LED_OFF;
	    Red_LED_OFF;
#endif    
	
	    lflags.data_mode=true; // пробуем почитать данные
	    if(lflags.hasPower) goto again; // при наличии питания - сразу же
    }
    return 0;
}

#if !defined(USE_MAVLINK) // we need  crc_init() and crc_accumulate()!
 #include "../GCS_MAVLink/include/mavlink/v1.0/checksum.h"
#endif

bool is_eeprom_valid(){// родная реализация кривая, у нас есть CRC из MAVLINKa, заюзаем его

    register uint8_t *p, *ee;
    uint16_t i;

    eeprom_read_len((byte *)&i, EEPROM_VERS, sizeof(i));
    if(i != CURRENT_VERSION) return false;		// разная версия EEPROM

    crc_init(&crc);

    for(i=sizeof(Params), p=(byte *)&p,  ee=(byte *)(EEPROM_PARAMS);  i>0; i--, ee++) 
	crc_accumulate(eeprom_read_byte((byte *) ee ), &crc);

    eeprom_read_len((byte *)&i, EEPROM_CRC, sizeof(crc));
    
    return i == crc;
}


void Read_EEPROM_Config(){ 

#if 1 // MAX_PARAMS * 4 < 255
    eeprom_read_len((byte *)&p, EEPROM_PARAMS, sizeof(p));
#else
    uint8_t *pp;
    uint16_t i, ee;

// eeprom_read_len недостаточно
    for(i=sizeof(p), pp=(byte *)&p,  ee=EEPROM_PARAMS;  i>0; i--,ee++) { // байта для адреса мало
	*pp++ = (byte)eeprom_read_byte( (byte *)ee );
    }
#endif
}

void write_Params_ram_to_EEPROM() { // записать зону параметров в EEPROM
    uint8_t *pp, *ee;
    uint16_t i;
    byte v;

    crc_init(&crc);

    for(i=sizeof(p), pp=(byte *)&p,  ee=(byte *)(EEPROM_PARAMS);  i>0; i--,ee++) { // байта для адреса мало
	v = *pp++;
	eeprom_write_byte((byte *) ee, v );
	crc_accumulate( v, &crc);
    }

    eeprom_write_len((byte *)&crc, EEPROM_CRC, sizeof(crc));

    crc=CURRENT_VERSION;
    eeprom_write_len((byte *)&crc, EEPROM_VERS, sizeof(crc));

DBG_PRINTLN("EEPROM write done!");
}
inline void printAllParams(){

    serial.print_P(PSTR("[CONFIG]\n"));


    for(byte i=0; i< PARAMS_END ;i++){
	print_SNS(PSTR("R"),i,PSTR("="));
	serial.println(((long *)&p)[i]);
    }
    for(byte i=0; i<sizeof(strParam)/sizeof(StrParam);i++){
	print_SNS(PSTR("S"),i+PARAMS_END,PSTR("="));
	serial.println((char *)pgm_read_word(&(strParam[i].ptr)));
    }

    serial.print_P(PSTR(">\n"));
}

void getSerialLine(byte *cp, void(cb)() ){	// получение строки
    byte cnt=0; // строка не длиннее 256 байт
//    unsigned long t=millis()+60000; //  макс 60 секунд ожидания - мы ж не руками в терминале а WDT выключен

//    while(t>millis()){
    while(true){
	if(!serial.available_S()){
	    if(cb) 
		cb();	// если есть callback то вызовем
	    else
		delay_1(); // ничего хорошего мы тут сделать не можем
	    continue;
	}
	
	byte c=serial.read_S();
// uses .available	if(c==0) continue; // пусто
	
	if(c==0x0d || (cnt && c==0x0a)){
//	    serial.println();
	    cp[cnt]=0;
	    return;
	}
	if(c==0x0a) continue; // skip unneeded LF
	
/*	if(c==0x08){   no manual editing
	    if(cnt) cnt--;
	    continue;
	}
*/
	cp[cnt]=c;
	if(cnt<SERIAL_BUFSIZE) cnt++;
    
    }

}

void getSerialLine(byte *cp) {
    getSerialLine(cp, NULL);
}

