/*

    Нативная библиотека GSM чудовищна и огромна, а  нам много не надо

    к тому же нога прерывания, нужная для работы SoftwareSerial, занята


Отправка данных на сервер (GSM/GPRS shield)
Подключаемся к Arduino к контактам 7 и 8. 


http://cxem.net/arduino/arduino170.php
http://arduino.ua/ru/guide/ArduinoGSMShield

*/

// printf without float
#define SKIP_FLOAT

//#define RX_BUFFER_SIZE 64 // AltSoftSerial buffer
//#define SERIAL_TX_BUFFER_SIZE 4 //SingleSerial buffer

#include "compat.h"
#include <SingleSerial.h>
#include <AltSoftSerial.h>
#include <avr/power.h>

//extern gsmSerial serial;


#include "Arduino.h"// Get the common arduino functions

#include "config.h"
#include "gsm.h"
#include "protocols.h"


#define GSM_DEBUG 1
//#define USSD_DEBUG 1

extern GSM gsm;
extern void delay_300();
extern void delay_100();
extern void delay_50();
extern void delay_10();
extern void delay_1();

char GSM::GSM_response[RESPONCE_LENGTH]; 
//#define GSM_response (msg.response)
byte GSM::lastError=0;
byte GSM::isTransparent=0;
byte GSM::isActive=0;
byte GSM::isGPRS=0;

static const char PROGMEM s_cpin_q[]="+CPIN?";
static const char PROGMEM s_creg_q[]="+CREG?";
static const char PROGMEM s_cpin_a[]="CPIN: READY";
static const char PROGMEM s_creg_a[]="CREG: 0,1";

char * result_ptr, * result2_ptr;

const char PROGMEM GSM::s_ok[]="OK";
const char PROGMEM      s_at[]="AT";


GSM::GSM(){

}


extern void delay_1000();

bool GSM::cregWait(){
    static const PROGMEM char patt_creg[]="CREG: 0,";

    byte nTry=60; // seconds

    while(nTry-- >0){
         byte f= command_P(s_creg_q, s_ok, patt_creg);   // NET registered?
	// handle CREG: 0,2

#ifdef GSM_DEBUG
//serial.printf_P(PSTR("#+CREG? result2=%c\n"),*(result2_ptr+sizeof(patt_creg)-1));
#endif	
	if(!f) return false; // command_P failed

        switch(*(result2_ptr+sizeof(patt_creg)-1)){
    	case '1':	// registered or roaming
    	case '5':
    	     return true; 
        
        case '3':	//denied
            return false; 
            
        case '0':	//  not registered but searching
        case '2':
        default:
    	    delay_1000();// will wait
    	    continue;
        }
        break;// all loop done by continue;
    }
    return false;
}


bool GSM::syncSpeed() {
    for(byte i=55;i!=0; i--)			// speed negotiation
        if(GSM::command_P(PSTR(""), 2) ) return true;

    return false;
}


void GSM::pulseDTR(){
    digitalWrite(GSM_DTR,LOW); // wake up
    delay_300();
    digitalWrite(GSM_DTR,HIGH);
    delay_100();
}

void GSM::pulseReset(){
    digitalWrite(GSM_EN,LOW);	// this is RESET
    delay_1000();
    digitalWrite(GSM_EN,HIGH);
    delay_1000();

}

bool  GSM::begin(){
    return begin(GSM_SPEED);
}

bool  GSM::begin(long speed){
    if(isActive) return true;

    power_timer1_enable();
    AltSoftSerial::begin(speed);

    byte try_count=10;
    byte f=0;

    pulseReset();

    do {

	if(syncSpeed()){

            command_P(PSTR("E0 V1 X1 S0=0")); // ECHO off, set error response and do not pickup on ring
            command_P(PSTR("+CFUN=1")); // work  - +CFUN=1,1 to reset
//	                GSM::command_P(PSTR("+CNETLIGHT=0")); // LED off
    
            command_P(PSTR("+CSCB=1")); // запрет широковещательных уведомлений
            command_P(PSTR("+CIPSHUT=1"),2); // закрытие текущего подключения --TODO
                
            f=      command_P(s_cpin_q,s_cpin_a); // SIM ok?

    	// http://we.easyelectronics.ru/blog/part/369.html
	// AT+CMGF
	// AT+CSCB=1
	// AT+GSMBUSY=1
	    command_P(PSTR("+CLIP=1")); // turn on Caller Identification

	    if(f) f=cregWait();
            if(f) f=command_P(PSTR("+CCALR?"),PSTR("CCALR: 1"));   // CALL enabled?
        
            if(f) break; // all OK
	} else {
	    pulseDTR();	    	
	    pulseReset();
	}
        command_P(PSTR("+CFUN=1,1")); // reset
        for(byte i=7;i>0;i--)
            delay_1000();	// delay 7 seconds
        
    } while(try_count-- >0); /// максимальное время ожидания при отсутствии сети - 10 раз по (10 секунд плюс время ожидания команды), около 3 минут

    if(f) {
	isActive=true;
    } else {    	//	так и не удалось разбудить
	AltSoftSerial::end();
        power_timer1_disable();
    }

    return f;
}

void GSM::turnOff(){
    power_timer1_enable();
    AltSoftSerial::begin(GSM_SPEED);
    syncSpeed();
    command_P(PSTR("E0")); // ECHO off
    end();
}


// закончили работу с GSM - выключим
void GSM::end() {
    command_P(PSTR("+CNETLIGHT=0"),2); // LED off
    command_P(PSTR("+CPOWD=1"),PSTR("DOWN"),2); //power down

    AltSoftSerial::end();
    power_timer1_disable();
    isActive=false;

}



void GSM::initGSM(void){
    digitalWrite(GSM_EN,HIGH);
    pinMode(GSM_EN,OUTPUT);

    digitalWrite(GSM_TX,HIGH);
    pinMode(GSM_TX, OUTPUT);

    digitalWrite(GSM_DTR,HIGH);
    pinMode(GSM_DTR, OUTPUT);
    
    pinMode(GSM_RX, INPUT);
//    pinMode(GSM_INT, INPUT_PULLUP);
    
// GSM_RING not used
}



bool GSM::set_sleep(byte mode){
    if(mode) {
               command_P(PSTR("+CNETLIGHT=0")); // LED off
	return command_P(PSTR("+CFUN=0")) // minimum func
	    /*&& GSM::command_P(PSTR("+CSCLK=1"))*/; //  sleep - we can't wake up
    } else {
        pulseDTR();

        byte try_count=10;
        do {	       
            bool f = false;
	    if(syncSpeed()){
                   command_P(PSTR("+CNETLIGHT=1")); // LED on
                f= command_P(PSTR("+CFUN=1")) && // work - +CFUN=1,1 to reset
                   command_P(s_cpin_q,s_cpin_a) && // SIM ok?
                   cregWait();   // wait for NET registered
		   
                if(f) return f;
                GSM::command_P(PSTR("+CFUN=1,1")); // reset
            } else {
                pulseDTR();	    	
		pulseReset();
	    }
            delay_1000();	// delay 1 seconds
	} while(try_count-- > 0);
    }

    return false;
}


#if defined(USE_GPRS)
void GSM::doOnDisconnect(){
    if(GSM::isTransparent) { // были в коннекте и что-то отвалилось
	// надо сбросить модуль и уйти на setup()
	pulseDTR();
	pulseReset();
	GSM::isTransparent=false;
	
	GSM::command_P(PSTR("+CFUN=1,1")); // reset
	__asm__ __volatile__ (    // Jump to RST vector
            "clr r30\n"
            "clr r31\n"
            "ijmp\n"
        );
    }
}
#endif

void GSM::readOut() { // Clean the input buffer from last answer and unsolicit answers
    char c;

    while(available_S()) {
#ifdef GSM_DEBUG
debug.print_P(PSTR("< "));
#endif
	while( available_S()) {
	    c=read_S();    
#ifdef GSM_DEBUG
debug.print(c);	    
#endif
#if defined(USE_GPRS)
            check_disconnect(c);
#endif
	}
	delay_10(); // wait for next char
    }

}


// отправка AT-команд
// usage  command_P(PSTR("+SAPBR=3,1,\"CONTYPE\",\"GPRS\""), [PSTR("OK")], [10000]);
uint8_t GSM::command_P(const char* cmd, const char* answer, const char* answer2, uint16_t time){
    readOut();

#ifdef GSM_DEBUG
debug.printf_P(PSTR("# want: %S"),answer);
if(answer2) debug.printf_P(PSTR(" or %S"),answer2);
debug.print_P(PSTR("\n#> AT"));
debug.println_P(cmd);
#endif

    gsm.print_P(s_at);     // Send the AT command
    gsm.println_P(cmd);    // Send the rest of command

    return wait_answer(answer, answer2, time)==1; // 10 sec
}

// 
uint8_t GSM::command(char* cmd, const char* answer2, uint16_t time){

    readOut();

#ifdef GSM_DEBUG
debug.print_P(PSTR("# want: OK or "));
debug.println_P(answer2);
debug.print_P(PSTR("#> AT"));
debug.println_P(cmd);
#endif

    gsm.print_P(s_at);    // Send the AT command
    gsm.println(cmd);     // Send the command

    return wait_answer(s_ok, answer2, time)==1; // 10 sec
}





uint8_t GSM::wait_answer(const char* answer, const char* answer2, unsigned int timeout){
    char * cp = GSM_response;
    unsigned long deadtime = millis() + timeout*100; // time in 0.1s
    char has_answer=0;
    result2_ptr=0;

    byte hasAnswer2 = (answer2 == 0);

    Red_LED_ON; 	// при работе с GSM красный мыргает на каждую команду и зеленый при получении данных
    delay_100(); // модуль не особо шустрый
    Red_LED_OFF; 

#ifdef GSM_DEBUG
debug.print_P(PSTR("\n"));
#endif
    do { // this loop waits for the answer
        delay_10(); // чуть подождать чтобы что-то пришло


        if(available_S()) {	// за время ожидания что-то пришло: 38400 бод - 4800 байт/с, за 10мс придет 48 байт
    	    Green_LED_ON;
    	    do {
        	char c;
        	*cp++ = c = read_S();
        	*cp=0;// string is always closed
#ifdef GSM_DEBUG
debug.print(c); 
#endif
    	    } while(available_S()); // вычитать все что есть, а потом проверять будем

    	    Green_LED_OFF;

	    // данные закончились, можно и проверить, если еще ответ не получен
	    if(!has_answer) { 
                // check if the desired answer  is in the response of the module
                if((result_ptr=strstr_P(GSM_response, answer)) != NULL)  { // окончательный ответ
                    has_answer = 1;
#ifdef GSM_DEBUG
debug.print_P(PSTR("="));
#endif
                } else
                if((result_ptr=strstr_P(GSM_response, PSTR("ERROR"))) != NULL)  { // окончательная ошибка
                    has_answer = 3;
#ifdef GSM_DEBUG
debug.print_P(PSTR(" ERR"));
#endif
                }
#if defined(USE_GPRS)
                if((result_ptr=strstr_P(GSM_response, PSTR("DEACT"))) != NULL)  { // окончательная ошибка
                    doOnDisconnect();
		}
#endif

            }

            if( answer2 !=NULL) {
                result2_ptr=strstr_P(GSM_response, answer2); // промежуточный ответ
                hasAnswer2 = (result2_ptr!=0);
#ifdef GSM_DEBUG
debug.printf_P(PSTR("#2< "),result2_ptr);
#endif
            }

            // TODO: контролировать разбиение на строки?
        } else if(has_answer && hasAnswer2) // за время ожидания ничего не пришло - данные кончились, если ответ получен - готово
    	    break;
    } while( millis() < deadtime ); // Waits for the asnwer with time out

    
#ifdef GSM_DEBUG
debug.println("\n# done");
#endif

    return lastError=has_answer;
}

uint8_t GSM::wait_answer(const char* answer, unsigned int timeout){
    return wait_answer(answer, NULL, timeout);
}


byte GSM::sendSMS(const char * phone, const char * text) {
    if(!strlen(phone)) return true;	// если номер не задан то считаем что все ОК
    
    command_P(PSTR("+CMGF=1")); // text mode
    command_P(PSTR("+CMGD=1")); // delete all messages
//  command_P(PSTR("+CSMS=1")); // mode 1

    gsm.print_P(PSTR("AT+CMGS=\""));
    gsm.print(phone);
    readOut();
    gsm.println_P(PSTR("\""));

    if(1 != wait_answer(PSTR(">"),30)) return false;

    gsm.print(text);
    gsm.println('\x1a');

    bool ok = wait_answer(s_ok,PSTR("+CMGS:"),300) == 1; // 30s

    return ok && result2_ptr!=NULL;
}

char * GSM::getRSSI(){
    static const PROGMEM char patt[]="+CSQ:";
    byte f= command_P(PSTR("+CSQ"), s_ok, patt);   // NET registered?
                
    if(!f) return 0; // command_P failed

    return result2_ptr+sizeof(patt)-1;
}

static const char PROGMEM patt_minus[] = "\x1c\x38\x3d\x43\x41\x3a"; //1c 38 3d 43 41 3a - Минус в UTF без старшего байта




byte GSM::sendUSSD(uint16_t text) {
    readOut();
    byte flg=0;
    char str[32];

again:
    bs.begin(str);
    bs.print_P(PSTR("+CUSD=1,\""));
    bs.write(flg==0?'#':'*');
    bs.print(text);
    bs.print_P(PSTR("#\""));


#if defined(USSD_DEBUG)
debug.print_P(PSTR("USSD ["));
debug.print(str);
debug.println_P(PSTR("]"));
#endif


  static const char PROGMEM patt[]="+CUSD:";

    if(1!=command(str,patt,200)) { // 20s
#ifdef USSD_DEBUG
debug.print_P(PSTR("No ans "));
#endif

	if(flg==0){
	    flg++;
	    goto again;
	}
#ifdef USSD_DEBUG
debug.print_P(PSTR("ERROR flg=1 "));
#endif
	return false;
    }
  
    delay_300();
  
    byte cnt=0;
    bool fIn=false;
    uint16_t num=0;
    //  0, "002D0037002E003000320440002E003004310430043B002E041D04300431043504400438044204350020002A00310030003600230020041E0431043504490430043D043D044B04390020043F043B0430044204350436002E00200418043D0444043E00200030003000300036", 72
    char *bp;
    byte pp = 0;
  
    bp = (char *)buf;

#ifdef USSD_DEBUG
debug.printf_P(PSTR("\nans: %s flg=%d "), result2_ptr, flg);
#endif

    for(char *cp = result2_ptr + sizeof(patt)-1;;){
        char c=*cp++;
    
        if(!c){
#ifdef USSD_DEBUG
debug.print_P(PSTR("\n EOL "));
#endif
        
    	     break;	// end of line
    	}
        if(c=='"'){

#ifdef USSD_DEBUG
debug.print_P(PSTR("\n kaw "));
debug.println(cp);
#endif

	    if(fIn) {
#ifdef USSD_DEBUG
debug.print_P(PSTR("\n close kaw "));
debug.println(cp);
debug.println((char *)buf);
#endif
		break; // внутри строки - строка кончилась
	    } else fIn=true;
        }else{
	    if(!fIn) continue; // пропускаем все перед кавычкой

	    if(flg){	// parse unicode
#ifdef USSD_DEBUG
debug.printf_P(PSTR("#\nUTF c='%c' "),c);
#endif

	        num <<= 4;
	        if(c>0x60) c-=0x20;
	        c-='0';
	        if(c>9) c+='0' - 'A' + 10;
	        num += c;
	
	        if(++cnt==4) {// конец
#ifdef USSD_DEBUG
//debug.print(num,16);
//debug.println();
//debug.println(cp);
// 1c 38 3d 43 41 3a - Минус в UTF без старшего байта
#endif

	            cnt=0;
	            c= (byte)num;
	
	            if(c == patt_minus[pp]) {
		        pp++;
	            } else if(pp==sizeof(patt_minus)-1) { // при совпадении запишем минус
		        *bp++='-';
		        *bp=0;
	            } else 
		        pp=0; // reset on difference
	
	            if(num<0x100){  // skip all unicode
		        *bp++=c;
		        *bp=0;
		    }
	            num=0;
		}
	    
	    } else  { //just write
#ifdef USSD_DEBUG
debug.printf_P(PSTR("c=%c\n"),c);
#endif
	        *bp++=c;
	        *bp=0;
	    }
	}
    }
  
    if(!fIn) {
#ifdef USSD_DEBUG
debug.printf_P(PSTR(" not fIn!!! cp=%s\n"),(char *)buf);
#endif
	return 0;
    }

#ifdef USSD_DEBUG
debug.printf_P(PSTR(" !!!=%s\n"),(char *)buf);
#endif

    return 1;
}


int GSM::balance(byte n){
    if(!sendUSSD(n)) {
	return 0; //
    }
    
    // in Buf

    char data[14];
    char *cp=data;
    bool fNeg=false;
    char *ptr=(char *)buf;

    for(*cp=0;;){
	char c=*ptr++;
	if(!c) break;
	if(c=='-')   fNeg=true;
	else if(c>='0' && c <= '9') { // digits
	    *cp++ = c;
	    *cp=0;

//serial.println(ptr);
//serial.println(data);

	}
	else if(c=='.' || c==',') break;
	// skip others
    }
//serial.println(data);

    if(!strlen(data)) return 0;

    int v=(int) atol(data);
    if(fNeg) {
	if(v==0) v=-1;
	else
	    v=-v;
    }
    return v;
}


// есть телеметрия, которая и так передает координаты, к тому же GSM на борту не сильно полезен остальным приборам
#if 0

void gprs_point() {  //Процедура начальной инициализации GSM модуля
 
    bs.begin(buf);
    bs.print_P(PSTR("ll="));
    
 //Установка настроек подключения
    command_P(PSTR("+SAPBR=1,1"));     //             Открыть несущую (Carrier)
    command_P(PSTR("+SAPBR=3,1,\"CONTYPE\",\"GPRS\"")); //тип подключения - GPRS  
    command_P(PSTR("+SAPBR=3,1,\"APN\",\"" APN "\""));
//     command_P(PSTR("+SAPBR=3,1,\"USER\",\"user\""));
//     command_P(PSTR("+SAPBR=3,1,\"PWD\",\"pass\""));
    command_P(PSTR("+SAPBR=1,1"));  // Устанавливаем GPRS соединение
    command_P(PSTR("+HTTPINIT"));  //  Инициализация http сервиса
    command_P(PSTR("+HTTPPARA=\"CID\",1"));  //Установка CID параметра для http сессии
    gsm.print_P(PSTR("+HTTPPARA=\"URL\",\""));
    gsm.print_P(PSTR(URL)); //     http:/????????.ru/gps/track.php?id=?N&ll=    //Собственно URL, после sprintf с координатами
    readOut();
    gsm.print(coord.lat);
    gsm.print(',');
    gsm.print(coord.lon);
    gsm.println_P(PSTR("\""));

    if(1 != wait_answer(patt,15000)) {
//serial.print_P(PSTR("No ans"));
	return false;
    }
 
    command_P(PSTR("+HTTPACTION=0"));    //Запросить данные методом GET
    command_P(PSTR("+HTTPREAD"));   //дождаться ответа
    command_P(PSTR("+HTTPTERM"));    //остановить HTTP
}
#endif

/*
Для получения страницы по определенному URL нужно послать следующие команды:
AT+SAPBR=1,1    //Открыть несущую (Carrier)
AT+SAPBR=3,1,"CONTYPE","GPRS"   //тип подключения - GPRS
AT+SAPBR=3,1,"APN","internet.beeline.ru" //APN, для Билайна - internet
AT+HTTPINIT    //Инициализировать HTTP 
AT+HTTPPARA="CID",1    //Carrier ID для использования.
AT+HTTPPARA="URL","http:/????????.ru/gps_tracker/gps_tracker1.php?id_avto=?N&lat=XXXXXlon=YYYYY"    //Собственно URL, после sprintf с координатами
AT+HTTPACTION=0    //Запросить данные методом GET
AT+HTTPREAD   //дождаться ответа
AT+HTTPTERM    //остановить HTTP
*/

/*
AT+SAPBR=3,1,CONTYPE,GPRS,  //Установка настроек подключения
AT+SAPBR=3,1,APN,internet,
AT+SAPBR=3,1,USER,gdata,
AT+SAPBR=3,1,PWD,gdata,
AT+SAPBR=1,1",  //Устанавливаем GPRS соединение
*/


bool GSM::initGPRS(){
    command_P(PSTR("+GSMBUSY=1")); // disable incoming calls
    command_P(PSTR("+SAPBR=3,1,\"CONTYPE\",\"GPRS\"")); //тип подключения - GPRS  

    command_P(PSTR("+CIPDPDP=1,10")); // check every 10 seconds for disconnect

    return
	command_P(PSTR("+CREG=2")) && // Enable network registration messages in extended format
	command_P(PSTR("+CMEE=2")) && // enable CMEE error reporting in extended format
	command_P(PSTR("+CR=1"))   && // enable intermediate result code
	command_P(PSTR("+CRC=1"))  && // enable extended format of incoming call indicator
	command_P(PSTR("+CSNS=4")) && // Single Numbering Scheme mode=data
	command_P(PSTR("+CSMINS=1")) && // enable SIM status report
	command_P(PSTR("+CSCLK=0")) && // disable slow clock
	command_P(PSTR("+CIURC=1")) && // enable URC presentation
	command_P(PSTR("+CGEREP=1"))&& // GPRS error reporting: 0 disable, 1 enable
	command_P(PSTR("+CIPMUX=0"))&& // Single channel communication (ie only one socket can be opened)
	command_P(PSTR("+CIPMODE=1"))&& // Transparent bridge mode
//(NmRetry:3-8),(WaitTm:2-10),(SendSz:1-1460),(esc:0,1) ,(Rxmode:0,1), (RxSize:50-1460),(Rxtimer:20-1000
	command_P(PSTR("+CIPCCFG=8,10,300,0,0,460,50")) &&  // GPRS params
	command_P(PSTR("+CGATT=1"),750); // Attach GPRS - 75s
	//                 mode,subset,portspeed(4->57600),frame size, ack time,
//	command_P(PSTR("+CMUX=0,0,4,32768,10,3,30,10,2")); // GPRS/IP params


//AT+CIPCSGP

}

bool GSM::setAPN(char *apn) {
    bool f;
    char str[80];
    static const PROGMEM char patt[]="CGATT:";

    if(command_P(PSTR("+CGATT?"),s_ok,patt,600) ){ // Make sure GPRS is Attached, 60s
        char *cp=result2_ptr + sizeof(patt);
        while(*cp){
            if(*cp!=' ') break;
            cp++;
        }
        byte v=(byte)atol(cp);
        isGPRS = (v!=0);
    }

    //if(!f) return f;


//AT+SAPBR=3,1,"APN","internet.tele2.ru"
    bs.begin(str);
    bs.print_P(PSTR("+SAPBR=3,1,\"APN\",\""));
    bs.print(apn);
    bs.print_P(PSTR("\""));

    f=command(str);
    if(!f) return f;


//        command_P(PSTR("+CSTT=\"internet\",\"\",\"\"")); 
    bs.begin(str);
    bs.print_P(PSTR("+CSTT=\"")); // AT+CSTT="APN","username","password" - login to service provider/carrier
    bs.print(apn);
    bs.print_P(PSTR("\",\"\",\"\"")); 
    
    return 1==command(str);
}


bool GSM::initUDP(uint16_t port){
    char str[64];
    
    bool f=
        command_P(PSTR("+CIICR"),30) && // Connect!
        command_P(PSTR("+CIFSR"),PSTR("."), s_ok); // Get IP address (for info only);
    
    if(!f) return f;
    
    readOut();


    bs.begin(str);
    bs.print_P(PSTR("+CLPORT=\"UDP\","));  // Prep UDP Port 8888
    bs.print(port);

#ifdef GSM_DEBUG
debug.print(str);
#endif

    return 1==command(str);
}

bool GSM::connectUDP(char *url, uint16_t port){
    readOut();

    char str[64];
 
    bs.begin(str);

    // AT+CIPSTART="protocol","ip address or domain","port #"
    bs.print_P(PSTR("+CIPSTART=\"UDP\",\"")); 
    bs.print(url);
    bs.print_P(PSTR("\","));
    bs.print(port);


#ifdef GSM_DEBUG
debug.print_P(PSTR("connect ["));
debug.print(str);
debug.println_P(PSTR("]"));
#endif

    if(1==command(str,PSTR("CONNECT"),1600)) { // 160s max
	isTransparent=1;
	return true;
    }
    return false;
    
}

