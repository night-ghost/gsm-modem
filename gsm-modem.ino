/*

http://diydrones.com/group/telemetry-over-cellular-ip/forum/topics/adding-gprs-telemetry-to-walkera-x350-pro


You'll need to interface the modem with your Arduino. 

TX from the modem's TTL port to RX1 on the Arduino, RX on the modem's TTL port to TX1 on the Arduino, 
Shared GND connecting the Modem with the Arduino and the power source and finally the VIN to your power source. 
The modem I'm using works with 5 to 20 v so I supply it from the X350 Pro's 11.1v battery. 

To handle loss of connection, you'll need to connect Pin 2 of your Arduino to the PWRKEY / PWRKEY_OUT pin on your modem 
and then connect Pin 3 of your Arduino to the Reset Pin of the Arduino

*/
///@file debug_Comm.ino
///@brief AT-command compatible cellular/GPRS/IP modem initialization & communication for SIM900D -> Arduino -> ArduCopter
///@author Robert Haddad
///@date 14-09-2014

/*
    port to Arduino Nano using AltSoftSerial & Timer2Serial


TODO: 
console 
URL in EEPROM
port in EEPROM

*/

#include <SingleSerial.h> // MUST be first
#include "Arduino.h"
#include "Timer2Serial.h"
#include <AltSoftSerial.h>
#include "compat.h" //   some missing definitions

//#include "gsm.h"

#define DEBUG_TX_PIN 4 // нога вывода отладки

SingleSerialPort(MAVlink);
Timer2Serial debug(0, DEBUG_TX_PIN); // only tx and no RSSI in DEVO


// --------- config section

#define GREEN_LED 12 // All Good LED Turns green when data is being sent over UDP Connection
#define RED_LED 11 // Error LED Turns Red when error is encountered, turns off if all is okay

#define powerKey 2 // Set PWRKEY pin
#define resetPin 3 // Set reset pin


// ---------- end config










#define Red_LED_ON   digitalWrite(RED_LED,HIGH);
#define Red_LED_OFF  digitalWrite(RED_LED,LOW);

#define Green_LED_ON digitalWrite(GREEN_LED,HIGH)
#define Green_LED_OFF digitalWrite(GREEN_LED,LOW)



#ifdef DEBUG
  #define DBG_PRINTLN(x)     { debug.print_P(PSTR(x)); debug.println(); /* debug.wait(); */ } 
  #define DBG_PRINTVARLN(x)  { debug.print(#x); debug.print(": "); debug.println(x); /* debug.wait(); */ }
  #define DBG_PRINTVAR(x)    { debug.print(#x); debug.print(": "); debug.print(x); debug.print(" ");  }
#else
  #define DBG_PRINTLN(x)     {}
  #define DBG_PRINTVAR(x)    {}
  #define DBG_PRINTVARLN(x)  {}
#endif


boolean isReady = false; // GSM modem is Ready to receive AT Commands flag
int count = 0; // Counter for instances of an error (For monitor/debug only);

String checker = ""; // String to hold debug output

void consoleCommands(){     
        struct Params loc_p;
                    
        loc_p=p;
                                    
        Green_LED_ON;
        Red_LED_ON;

#if !defined(DEBUG)
        MAVlink.begin(57600);
#endif

        static const char PROGMEM patt[] = "gsm-modem\n";

        MAVlink.print_P(patt);
        MAVlink.println();
      
        byte try_count=0;
 
//    DBG_PRINTLN("console");
                  
        while(1) {
            byte cnt=0;
            for(unsigned long t=millis()+3000; millis() < t;){
                if(MAVlink.available_S()) {
                    byte c=MAVlink.read_S();
                            
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
//                  MAVlink.print_P(PSTR("CRC!\n"));
                    write_Params_ram_to_EEPROM();
                }


                Read_EEPROM_Config();

		

                while(true){
                    MAVlink.print_P(PSTR("[CONFIG]\n"));
                    printAllParams();

                    getSerialLine(buf);

                    if(buf[0] && !buf[1]) { // one char command
                        switch(buf[0]){
                        case 'd':
                            p=loc_p; // восстановить параметры

//                          for(int i=4; i>=0; i--)    // испортить CRC
//                          for(unsigned int i=E2END; i>=0; i--)// стереть весь EEPROM
//                               eeprom_write(i,0xFF); 
                        // no break!

                        case 'w':
                            write_Params_ram_to_EEPROM();
                            break;

                        case 'q':
                            //return;
                            goto console_done;
                            
#if defined(USE_GSM)
                        case 'm': // get balance
                            gsm.sendUSSD(100);
                            // in buffer
                            MAVlink.print((char *)buf);
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
#endif
                        }
                    } else { // new param value
                	
                        byte n=atol(((char *)buf)+1); // зачем еще и atoi тaщить
                        byte type=*buf;
			switch(type){
			case 'L': // long
			    if(n > PARAMS_END) 
                        	break;
                    	    println_SNS(PSTR("R"),n, PSTR("="));

                	    getSerialLine(buf); // new data

                            if(*buf)
                                ((long *)&p )[n] = atol((char *)buf); // если не пустая строка то преобразовать и занести в численный параметр

			    break;
			    
			case 'S': // string
			    if(n > SPARAMS_END)
				break;
				
			    println_SNS(PSTR("R"),n, PSTR("="));
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
#if !defined(DEBUG) 
        MAVlink.end();
#endif
}            

// Setup procedure
void setup()
{
    pinMode(GREEN_LED, OUTPUT); // All Good LED
    pinMode(RED_LED, OUTPUT); // Warning LED
    digitalWrite(GREEN_LED,LOW); // Set All Good LED to Off
    digitalWrite(RED_LED,LOW); // Set Warning LED to Off
    digitalWrite(resetPin, HIGH); // Set Reset Pin

    pinMode(powerKey, OUTPUT);

    // Set Data Rate for debug Ports
    debug.begin(57600); // Start USB Port debug @ 57,600 Baud Rate (For Monitor/Debug only)
    gsm.begin(57600); // Start GSM Module debug Communication @ 57,600 Baud Rate
    
    MAVlink.begin(57600); // Start Autopilot Communication @ 57,600 Baud Rate
    
    consoleCommands();     // Wait 3 Seconds for power on (Add Auto on feature through GSM module power button pin


//* speed test

   debug._tx_delay=10;
again:
   debug._tx_delay ++; // 132
   debug.printf_P(PSTR("Timer2Serial speed test v1234567890 i=%d\n"), debug._tx_delay);

  if(debug._tx_delay < 255) goto again;

//*/
    
    pinMode(resetPin, OUTPUT);

    // Initializing GSM Module
    debug.println(F("Initializing..."));
    gsm.println(F("AT"));
    while(!isReady){
	if(debug.available()){
	    String a=debug.readString();
	    gsm.print(a);
	}

	if(gsm.available()){
	    String b=gsm.readString();
	
	    if(b.indexOf(F("+CREG: 1")) > 0){// || b.indexOf("OK") > 0){
		debug.println(F("SIM800 Ready & Loading"));
		isReady = true;
		String ok=F("OK");
		while(!sendATCommand(F("AT V1 E1 X1 S0=0"),ok,100)); // Set error response and do not pickup on ring
		debug.println(F("Registered: Starting Configuration"));

		while(!sendATCommand(F("AT+CREG=2"),ok,100)); // Enable network registration messages in extended format
		while(!sendATCommand(F("AT+CMEE=2"),ok,100));	// enable CMEE error reporting in extended format
		while(!sendATCommand(F("AT+CR=1"),ok,100));	// enable intermediate result code
		while(!sendATCommand(F("AT+CRC=1"),ok,100));	// enable extended format of incoming call indicator
		while(!sendATCommand(F("AT+CSNS=4"),ok,100));	// Single Numbering Scheme mode=data
		while(!sendATCommand(F("AT+CSMINS=1"),ok,100)); // enable SIM status report
		while(!sendATCommand(F("AT+CSCLK=0"),ok,100));	// disable slow clock
		while(!sendATCommand(F("AT+CIURC=1"),ok,100));	// enable URC presentation
		while(!sendATCommand(F("AT+CGEREP=2"),ok,100)); // GPRS error reporting: 0 disable, 1 enable
		while(!sendATCommand(F("AT+CIPMUX=0"),ok,100)); // Single channel communication (ie only one socket can be opened)
		while(!sendATCommand(F("AT+CIPMODE=1"),ok,100)); // Transparent bridge mode
		//                          NmRetry, WaitTm, SendSz, Esc. 
		while(!sendATCommand(F("AT+CIPCCFG=8,10,400,0,0,460,50"),ok,100)); // GPRS params
		//                 mode,subset,portspeed(4->57600),frame size, ack time,
		gsm.println(F("AT+CMUX=0,0,4,32768,10,3,30,10,2)"); // GPRS/IP params
		//0 4 127/32768 3 2
		delay(2000);
		while(!sendATCommand(F("AT+CGATT?"),ok,1000)); // Make sure GPRS is Attached
		while(!sendATCommand(F("AT+CSTT= \"internet\",\"\",\"\""),ok,1000)); // AT+CSTT="APN","username","password" - login to service provider/carrier
		debug.println(F("APN Set"));
		while(!sendATCommand(F("AT+CIICR"),ok,1000)); // Connect!
		while(!sendATCommand(F("AT+CIFSR"),".",100)); // Get IP address (for info only);
		while(!sendATCommand(F("AT+CLPORT=\"UDP\",8888"),ok,100)); // Prep UDP Port 8888
		debug.println(F("Connecting to UDP Server"));

		// this says CONNECT
		while(!sendATCommand(F("AT+CIPSTART=\"UDP\",\"drone.dyndns.org\",8888"),ok,1000)); // AT+CIPSTART="protocol","ip address or domain","port #"
/* server case
		//while(!sendATCommand("AT+CIPCTL=1","+CREG: 1",100)); // Set port to listen. Not needed
		//while(!sendATCommand("AT+CIPSERVER=1,8888",ok,1000)); // TCP/UDP Server
*/
	    // now we in transparent data mode, exit by DTR, return back to ATO

		digitalWrite(GREEN_LED,HIGH); // Turn on All Good LED
		digitalWrite(RED_LED,LOW);    // Turn off Warning LED

//		debug.end(); // Stop USB debug communication - let it translates MAVlink
// at startup	MAVlink.begin(57600); // Start Autopilot Communication @ 57,600 Baud Rate

		delay(1000);
		digitalWrite(GREEN_LED,LOW);

/*
//* First test if everything is okay 

=> AT

<= AT //* This should come back. SIM900 default is to echo back commands you enter 

<= OK //* This string should tell you all is well

=>AT+CPIN? //*This is to check if SIM is unlocked. This sample assumes unlocked SIMs

<= +CPIN: READY //* If your response contains this, then it means SIM is unlocked and ready

=>AT+CREG? //*This checks if SIM is registered or not

<=+CREG: 0,1 //*This string in the response indicates SIM is registered

=>AT+CGATT? //*Check if GPRS is attached or not

<=+CGATT: 1 //*A response containing this string indicates GPRS is attached

=>AT+CIPSHUT //*Reset the IP session if any

<=SHUT OK //*This string in the response represents all IP sessions shutdown.

=>AT+CIPSTATUS //*Check if the IP stack is initialized

<=STATE: IP INITIAL //*This string in the response indicates IP stack is initialized

=>AT+CIPMUX=0 //*To keep things simple, I’m setting up a single connection mode

<=OK //*This string indicates single connection mode set successfully at SIM 900

=>AT+CSTT= “APN”, “UNAME”, “PWD” //*Start the task, based on the SIM card you are using, you need to know the APN, username and password for your service provider

<= OK //*This response indicates task started successfully

=> AT+CIICR //*Now bring up the wireless. Please note, the response to this might take some time

<=OK //*This text in response string indicates wireless is up

=>AT+CIFSR //*Get the local IP address. Some people say that this step is not required, but if I do not issue this, it was not working for my case. So I made this mandatory, no harm.

<= xxx.xxx.xxx.xxx //*If previous command is successful, you should see an IP address in the response

=>AT+CIPSTART= “TCP” , “www.vishnusharma.com”, “80” //*Start the connection, TCP, domain name, port

<= CONNECT OK //*This string in the response indicates TCP connection established

=>AT+CIPSEND //*Request initiation of data sending (the request)

<= > //*The response should be the string “>” to indicate, type your data to send

=> xxxxxx //*Just type anything for now

=>#026 //*Now type the sequence #026. This tells the terminal.exe to send the hex code 0x1a (which is Ctrl+Z) to indicate end of data sending

<= xxxxxxxxxx //*You should get some response back from the server…it would generally be a complain that the request string was not valid…but that is a different subject…you have established the connection

//*To close the connection

=>AT+CIPSHUT //*Request shutting down of the current connections

<=SHUT OK //*Indicates shutdown successful


*/


/*
String dyndns = "members.dyndns.org";
String hostname = "YOUR DYNAMIC HOST NAME"; // IE drone.dyndns.org
String userpwdb64 = "BASE64 Username & Password"; // username:password

String currentIP = "YOUR CURRENT IP ADDRESS";
debug.print("AT+CIPSEND\r");
debug.print("GET /nic/update?hostname=" + hostname + "&myip=" + currentIP + "&wildcard=NOCHG&mx=NOCHG&backmx=NOCHG HTTP/1.0\r");
debug.print("Host: " + dyndns + "\r");
debug.print("Authorization: Basic " + userpwdb64 + "\r");
debug.print("User-Agent: arduinodydns - 1.1\r");
debug.print("\x1A\r");

*/
	    } else {
		digitalWrite(RED_LED,HIGH); // Turn on Warning LED
		digitalWrite(GREEN_LED,LOW); // Turn off All Good LED
		debug.print(b);
	    }
	}
    }
}


// TODO: ограничить время ожидания ?
void getSerialLine(byte *cp, void(cb)() ){       // получение строки
    byte cnt=0; // строка не длиннее 256 байт
//    unsigned long t=millis()+60000; //  макс 60 секунд ожидания - мы ж не руками в терминале а WDT выключен

//    while(t>millis()){
    while(true){
        if(!MAVlink.available_S()){
            if(cb)
                cb();   // если есть callback то вызовем
            else            
                delay_1(); // ничего хорошего мы тут сделать не можем
            continue;
        }

        byte c=MAVlink.read_S();
// uses .available       if(c==0) continue; // пусто

        if(c==0x0d || (cnt && c==0x0a)){
//           MAVlink.println();
            cp[cnt]=0;
            return;
        }
        if(c==0x0a) continue; // skip unneeded LF

/*       if(c==0x08){   no manual editing
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

// AT Command Sender
boolean sendATCommand(String input, String output, int wait){ // AT Command, Expected Result, Delay before issue command
    boolean resp = false;
    
    debug.write('>'); debug.println(input);
    gsm.println(input);
    
    uint32_t to=millis() + wait;
    while(!resp){
	if(gsm.available()){
	    String b=gsm.readString();
	    debug.write('<'); debug.println(b);
	    gsm.read();
	    debug.read();
	    if(b.indexOf(output) > 0){ // True if expected result is returned
	        resp = true;
	        count = 0;
	        digitalWrite(RED_LED,LOW); // Warning LED OFF
	        break;
	    } else if(b.indexOf("DEACT") > 0){
		if(powerUpOrDown()){
		    digitalWrite(resetPin,LOW);
		}
	    } else {
		digitalWrite(RED_LED,HIGH); // Warning LED ON
		debug.print(b);
		count++;
		return resp;
	    }
	}
	
	if(millis()>to) break;
    }
    return resp;
}

//Start Up Wait Period with LEDs
void startUPWait(){
    digitalWrite(LED,HIGH);
    digitalWrite(RED_LED,LOW);
    for(byte i=0; i<30;i++){
        delay(100);
        digitalWrite(GREEN_LED,!digitalRead(GREEN_LED));
	digitalWrite(RED_LED,!digitalRead(RED_LED));
    }
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(RED_LED,LOW);
}

// Main Loop
void loop(){
    // Relay All GSM Module communication to Autopilot and USB (USB for monitor/debug only)
    if(gsm.available()){
	digitalWrite(RED_LED,HIGH);
	char b=gsm.read();
	MAVlink.write(b);
	    // Check For Disconnection
	    checker += b;
	    if(checker.indexOf("\n") > 0 || checker.indexOf("\r") > 0){
	        if(checker.indexOf("DEACT") > 0){
		    if(powerUpOrDown()){
		        digitalWrite(3,LOW);
		    }
	        }
	        checker = "";
	    }
	    debug.print(b);
	digitalWrite(RED_LED,LOW);
    }

    // Relay all Autopilot communication to GSM module and USB (USB for monitor/debug only)
    if(MAVlink.available()){
	digitalWrite(GREEN_LED,HIGH);
	    char c=MAVlink.read();
	    gsm.write(c);
	    debug.print(c);
	digitalWrite(GREEN_LED,LOW);
    }
}

boolean powerUpOrDown(){
    boolean powered = false;
    digitalWrite(powerKey,LOW);
    delay(1000);
    digitalWrite(powerKey,HIGH);
    delay(2000);
    digitalWrite(powerKey,LOW);
    delay(3000);
    powered = true;
    return powered;
}

