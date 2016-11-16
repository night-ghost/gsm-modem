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

void consoleCommands(){     
        struct Params loc_p;
                    
        loc_p=p;
                                    
        Green_LED_ON;
        Red_LED_ON;

#if !defined(DEBUG)
        serial.begin(57600);
#endif

        static const char PROGMEM patt[] = "gsm-modem\n";

        serial.print_P(patt);
        serial.println();
      
        byte try_count=0;
 
//    DBG_PRINTLN("console");
                  
        while(1) {
            byte cnt=0;
            for(unsigned long t=millis()+3000; millis() < t;){
                if(serial.available_S()) {
                    byte c=serial.read_S();
                            
//              if(strncasecmp_P( buf, pat, sizeof(pat) )==0){

                    if(cnt>=(sizeof(patt)-1)) break;

                    if(c != pgm_read_byte(&patt[cnt]) ) {
                        cnt=0;
                        continue;
                    }
                    cnt++;
                }
            }
            if(cnt == sizeof(patt)-2){
DBG_PRINTLN("console OK");
                if(!is_eeprom_valid()) {
//                  serial.print_P(PSTR("CRC!\n"));
                    write_Params_ram_to_EEPROM();
                }


                Read_EEPROM_Config();

		

                while(true){
                    serial.print_P(PSTR("[CONFIG]\n"));
                    printAllParams();

                    getSerialLine(buf);

                    if(buf[0] && !buf[1]) { // one char command
                        switch(buf[0]){
                        case 'd':
                            p=loc_p; // восстановить параметры

                        // no break!

                        case 'w':
                            write_Params_ram_to_EEPROM();
                            break;

                        case 'q':
                            //return;
                            goto console_done;
                            
                        case 'm': // get balance
                            gsm.sendUSSD(100);
                            // in buffer
                            serial.print((char *)buf);
                            break;

                        case 'g': // прямая связь с модемом
                            gsm.begin();
                            do {
                                getSerialLine(buf, readGSM); // считать строку, во время ожидания выводим все с GSM в линию
                                for(byte *cp=buf;*cp;){
                                    //gsm.print(*cp++); 828b
                                    gsm.write(*cp++); // 822b
                                }
                                gsm.println();
                            } while(*buf); // выход по пустой строке
                            gsm.end();  // всяко это потребуется только дома у компа, а значит потом передернут питание
                            break;

                        }
                    } else { // new param value
                	
                        byte n=atol(((char *)buf)+1); // зачем еще и atoi тaщить
                        byte type=*buf;
			switch(type){
			case 'L': // long
			    if(n > PARAMS_END) 
                        	break;
                    	    print_SNS(PSTR("R"),n, PSTR("="));

                	    getSerialLine(buf); // new data

                            if(*buf)
                                ((long *)&p )[n] = atol((char *)buf); // если не пустая строка то преобразовать и занести в численный параметр

			    break;
			    
			case 'S': // string
			    if(n > SPARAMS_END)
				break;
				
			    print_SNS(PSTR("R"),n, PSTR("="));
                	    getSerialLine(buf); // new data
                            if(n<sizeof(strParam)/sizeof(StrParam)) {
                                StrParam s=strParam[n];
                                strncpy(s.ptr, (char *)buf, s.length-1);        // занести как есть в строковый параметр
                            }
			}                    
                    }
                }
            } else {            
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
    digitalWrite(resetPin, HIGH); // Set Reset Pin

    gsm.initGSM();  // настроить ноги к GSM модулю
    
    // Set Data Rate for debug Ports
    debug.begin(38400); // Start Debug Port @ 38400 Baud Rate (For Monitor/Debug only)

    serial.begin(57600); // Start Autopilot/Console Communication @ 57,600 Baud Rate
    
    consoleCommands();     // Wait 1 Seconds for commands 

    Red_LED_OFF; // Set Warning LED to Off - we are ready

//* speed test

   debug._tx_delay=10;
again:
   debug._tx_delay ++; // 132
   debug.printf_P(PSTR("Timer2Serial speed test v1234567890 i=%d\n"), debug._tx_delay);

  if(debug._tx_delay < 255) goto again;

//*/
    
    pinMode(resetPin, OUTPUT);

    // Initializing GSM Module
debug.println_P(PSTR("Initializing..."));

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


	gsm.initGPRS();
	//0 4 127/32768 3 2
	delay(2000);

	gsm.setAPN(p.apn);

debug.println_P(PSTR("APN Set"));


	gsm.initUDP(p.port);
debug.println_P(PSTR("Connecting to UDP Server"));

	// this says CONNECT
	gsm.connectUDP(p.url, p.port);
	
    // now we in transparent data mode, exit by DTR, to return back say "ATO"
        serial.gsmMode(1);
debug.println_P(PSTR("Connected!"));

	Green_LED_ON; // Turn on All Good LED
	Red_LED_OFF;    // Turn off Warning LED

//	debug.end(); // Stop debug communication - let it translates serial
// at startup	serial.begin(57600); // Start Autopilot Communication @ 57,600 Baud Rate, autoBaud is on

	delay_1000();
	Green_LED_OFF;


    
}


//Start Up Wait Period with LEDs

void startUPWait(){
    Green_LED_ON;
    Red_LED_OFF;
    for(byte i=0; i<30;i++){
        delay(100);
        digitalWrite(GREEN_LED,!digitalRead(GREEN_LED));
	digitalWrite(RED_LED,  !digitalRead(RED_LED));
    }
    Green_LED_OFF;
    Red_LED_OFF;
}



void check_disconnect(char b){

    const static PROGMEM char patt[]="DEACT";

    // Check For Disconnection
    if(b=='\n' || b=='\r' || strPtr >= &strBuf[STR_BUF_SIZE-1]) {
	if(strncasecmp_P( strBuf, patt, sizeof(patt)-1 )==0){
	    // we lost connection - do a software reset

                __asm__ __volatile__ (    // Jump to RST vector
                    "clr r30\n"
                    "clr r31\n"
                    "ijmp\n"
                );

        }
        strPtr = &strBuf[0];
    } else 
	*strPtr++=b;

}


// Main Loop
void loop(){
    getData(); // all GSM magick done by gsmSerial class
    
// now we has a place for misc housekeeping - communication protocol parsed and variables filled by data

    // at the end of HEARTBEAT packet we can add own state packet    

/*
    // Relay All GSM Module communication to Autopilot and USB (USB for monitor/debug only)
    if(gsm.available()){
        Red_LED_ON;

	char b=gsm.read();
	serial.write(b);

	check_disconnect(b);
	
	Red_LED_OFF;
    }

    // Relay all Autopilot communication to GSM module and USB (USB for monitor/debug only)
    if(serial.available()){
	
	Green_LED_ON;
	    char c=serial.read();
	    gsm.write(c);
	    debug.print(c);
	Green_LED_OFF;
    }
*/

}


// fake functions for library compatibility with CoolBeacon
void chute_got_climb() {}
void chute_got_atti() {}
void chute_got_imu() {}
void chute_got_alterr() {}

