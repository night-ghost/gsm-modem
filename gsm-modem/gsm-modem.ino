/*


transparent GSM modem on Arduino Nano (3.3v 8MHz) and SIM800L tiny GSM module

uses AltSoftSerial & Timer2Serial


Powering: by 1 LiPo cell or 4.2 DC-DC module. DON'T CONNECT IT TO 5V !!!




You'll need to interface the modem with your Arduino. 

TX from the modem's TTL port to RX1 on the Arduino, RX on the modem's TTL port to TX1 on the Arduino, 
Shared GND connecting the Modem with the Arduino and the power source and finally the VIN to your power source. 
The modem I'm using works with 5 to 20 v so I supply it from the X350 Pro's 11.1v battery. 


based on:

http://diydrones.com/group/telemetry-over-cellular-ip/forum/topics/adding-gprs-telemetry-to-walkera-x350-pro

*/


#include <SingleSerial.h> // MUST be first
#include "Arduino.h"

#include "Timer2Serial.h"
#include <AltSoftSerial.h>
#include "compat.h" //   some missing definitions

#include "bufstream.h"

BS bs; // BuffStream


#include "config.h" 
#include "version.h" 

#include "vars.h"
#include "eeprom.h"

#include "gsm.h"


SingleSerialPort(trueSerial);        // hardware
Timer2Serial debug(0, DEBUG_TX_PIN); // only tx
GSM gsm;                             // AltSoftSerial


#include "gsmSerial.h"
gsmSerial serial; // fake one which act as gsm-serial bridge


#include "gsm_core.h"

#include "protocols.h"

#include "func.h" 



boolean isReady = false; // GSM modem is Ready to receive AT Commands flag

int count = 0; // Counter for instances of an error (For monitor/debug only);


#define STR_BUF_SIZE 128
char strBuf[STR_BUF_SIZE];
char *strPtr = &strBuf[0];


void init_GSM(){
    // Initializing GSM Module
debug.println_P(PSTR("Initializing..."));

again:

//      try to Start GSM Module Communication
    if(!gsm.begin() ) {
	for(int i=0; i<15; i++){ // 9 seconds of flashing
            Red_LED_OFF;
            delay_300();
            Red_LED_ON;
            Green_LED_OFF;
            delay_300();
            Green_LED_ON;
	}
                        // and then restart
        __asm__ __volatile__ (    // Jump to RST vector
            "clr r30\n"
            "clr r31\n"
            "ijmp\n"
        );

    }

debug.println_P(PSTR("SIM800 Ready & Loading"));

    isReady = true;
debug.println_P(PSTR("Registered: Starting Configuration"));


    if(!gsm.initGPRS()) goto again;

//    delay(2000);

    if(!gsm.setAPN(p.apn)) goto again;

debug.println_P(PSTR("APN Set"));


}

void consoleCommands(){     
        struct Params loc_p;
                    
        loc_p=p;
                                    
        Green_LED_ON;
        Red_LED_ON;

        static const char PROGMEM patt[] = "gsm-modem";
        delay(1000); 

        serial.printf_P(PSTR("\n%S" " "  TO_STRING(RELEASE_NUM) "\n"),patt );

DBG_PRINTLN("console");
      
        byte try_count=0;

                  
        while(1) {
            byte cnt=0;
            for(unsigned long t=millis()+3000; millis() < t;){
                if(serial.available_S()) {
                    byte c=serial.read_S();
DBG_PRINTF("c=%x\n", c);
//              if(strncasecmp_P( buf, pat, sizeof(pat) )==0){

                    if(cnt>=(sizeof(patt)-1)) break;

                    if(c != pgm_read_byte(&patt[cnt]) ) {
                        cnt=0;
                        continue;
                    }
                    cnt++;
                }
            }
            if(cnt == sizeof(patt)-1){
debug.println_P(PSTR("console OK"));
                if(!is_eeprom_valid()) {
                   DBG_PRINTLN("CRC!\n");
                    write_Params_ram_to_EEPROM();
                }


                Read_EEPROM_Config();

		

                while(true){
                    serial.print_P(PSTR("[CONFIG]\n"));
                    printAllParams();

                    getSerialLine(buf);

//DBG_PRINTF("buf=%s\n", buf);
                    
                    if(buf[0] && !buf[1]) { // one char command
                        byte type=*buf;
                        *buf=0; // clear

//DBG_PRINTF("cmd=%v\n", type);
                        switch(type){
                        case 'd':
                            p=loc_p; // восстановить параметры
                        // no break!

                        case 'w':
                            write_Params_ram_to_EEPROM();
DBG_PRINTF("eeprom write\n");
                            break;

                        case 'q':
                            goto console_done;
                            
                        case 'm': // get balance
                            if(!isReady)
                                init_GSM();
                            gsm.sendUSSD(100);
                            // in buffer
                            if(*buf)
                                serial.println((char *)buf);
                            else
                                serial.println_P(PSTR("unknown"));
                            break;

                        case 'n': // get balance 101
                            if(!isReady)
                                init_GSM();
                            gsm.sendUSSD(101);
                            // in buffer
                            if(*buf)
                                serial.println((char *)buf);
                            else
                                serial.println_P(PSTR("unknown"));
                            break;
                            
                        case 'c':
                            if(!isReady)
                                init_GSM();

                            lflags.data_link_active=true;
                            loop();
                            
                            gsm.println_P(PSTR("GSM Modem connected OK!"));
                            serial.println_P(PSTR("Modem connected OK!"));
                            break;

                        case 'g': // прямая связь с модемом
                            if(!isReady)
                                init_GSM();

DBG_PRINTF("connect start\n");
                            do {
                                getSerialLine(buf, readGSM); // считать строку, во время ожидания выводим все с GSM в линию
                                if(buf[0]=='.' && !buf[1]) // one dot  - exit
                                    break;
                                
                                for(byte *cp=buf;*cp;){
                                    gsm.write(*cp++); // 822b
                                }
                                gsm.println();
                            } while(*buf); // выход по пустой строке
                            //gsm.end();  // всяко это потребуется только дома у компа, а значит потом передернут питание
DBG_PRINTF("connect done\n");
                            break;

                        }
                    } else { // new param value
                	
                        byte n=atol(((char *)buf)+1); // зачем еще и atoi тaщить
                        byte type=*buf;
                        
                        char *bp=(char *)buf;
                        while(*bp) {
                            if(*bp++ == '=') break;
                        }
                        
                        
			switch(type){
			case 'L': // long
			case 'R':
			    if(n > PARAMS_END) 
                        	break;

                            if(*bp) {
                                ((long *)&p )[n] = atol((char *)bp);  // если не пустая строка то преобразовать и занести в численный параметр
DBG_PRINTF("new r%d=%s\n",n,(char *)bp);

                            }
			    break;
			    
			case 'S': // string
			    if(n > SPARAMS_END)
				break;

DBG_PRINTF("new s%d=%s\n",n,(char *)bp);

                            n-=PARAMS_END;
                            if(n<sizeof(strParam)/sizeof(StrParam)) {
                               const StrParam *sp = &strParam[n];
                               strncpy((char *)pgm_read_word(&(sp->ptr)), (char *)bp, pgm_read_byte(&(sp->length))-1); // занести как есть в строковый параметр
                            }
                            break;
			}                    
                    }
                }
            } else {    // no pattern
                if(is_eeprom_valid()){
                    Read_EEPROM_Config();

//                  DBG_PRINTLN("load EEPROM OK");
                    break;
                }
                if(try_count>=20) {
                    write_Params_ram_to_EEPROM();
                    break;
                }
//              DBG_PRINTLN("Wait command EEPROM bad");
                for(byte i=3; i>0; i--){
                    Red_LED_OFF;
                    delay_300();
                    Red_LED_ON;
                    Green_LED_OFF;
                    delay_300();
                    Green_LED_ON;
                }

            }
            try_count++;
        }

console_done:
DBG_PRINTF("exit console\n");
        Green_LED_OFF;
        Red_LED_OFF;

}            




// Setup procedure
void setup()
{
    pinMode(GREEN_LED, OUTPUT); // All Good LED
    pinMode(RED_LED, OUTPUT); // Warning LED
    Green_LED_OFF;		 // Set All Good LED to Off
    Red_LED_ON; 
#ifdef resetPin
    digitalWrite(resetPin, HIGH); // Set Reset Pin
    pinMode(resetPin, OUTPUT);
#endif

    gsm.initGSM();  // настроить ноги к GSM модулю
    
    debug.begin(DEBUG_SPEED); // Start Debug Port (For Monitor/Debug only)

    serial.begin(TELEMETRY_SPEED); // Start Autopilot/Console Communication 

    sei();
        
    consoleCommands();     // Wait 1 Seconds for commands 


/* speed test
   debug._tx_delay=10;
again:
   debug._tx_delay ++; // 132
   debug.printf_P(PSTR("Timer2Serial speed test v1234567890 i=%d\n"), debug._tx_delay);

  if(debug._tx_delay < 255) goto again;
//*/

    if(!isReady)
        init_GSM();
    

    Red_LED_OFF;    // Turn off Warning LED - init done

    
}

void raw_check(){
    const static PROGMEM char patt[]="DEACT";

    if(strncasecmp_P( strBuf, patt, sizeof(patt)-1 )==0){
    // we lost connection - do a software reset
        gsm.doOnDisconnect();

    }
}


void check_disconnect(char b){

    // Check For Disconnection
    if(b=='\n' || b=='\r') {
        raw_check();
        strPtr = &strBuf[0];
    } else if(strPtr >= &strBuf[STR_BUF_SIZE-1]) {
        raw_check();
        memcpy(strPtr-10, strBuf, 10); // move last 10 bytes to begin
        strPtr = &strBuf[10];
    } else 
	*strPtr++=b;

}


// Main Loop
void loop(){
    getData(); // all GSM magick done by gsmSerial class
    
    if(lflags.data_link_active && !serial.is_gsmMode() ){ // we got valid MAVlink packet and still not connected
	Red_LED_ON;    // Turn off Warning LED
	Green_LED_ON;

	gsm.initUDP(p.port);
debug.println_P(PSTR("Connecting to UDP Server"));

	gsm.connectUDP(p.url, p.port);
	
    // now we in transparent data mode, exit by DTR, to return back say "ATO"
        serial.gsmMode(1);
debug.println_P(PSTR("Connected!"));

	Green_LED_ON; // Turn on All Good LED
	Red_LED_OFF;    // Turn off Warning LED

	delay_1000();
	Green_LED_OFF;
    }

// now we has a place for misc housekeeping - communication protocol parsed and variables filled by data

    // at the end of HEARTBEAT packet we can add own state packet    


}


// fake functions for library compatibility with CoolBeacon
void chute_got_climb() {}
void chute_got_atti() {}
void chute_got_imu() {}
void chute_got_alterr() {}

