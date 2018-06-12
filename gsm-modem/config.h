
/*
    board type
    0 - arduino pro mini + Sim800
    1 - CoolBeacon board 
*/
#define HARDWARE_TYPE 0

#define DEBUG

#define TELEMETRY_SPEED 57600

#define USE_MAVLINK 1
#define AUTOBAUD 1

#define USE_GPRS 1


#if HARDWARE_TYPE==0 // Arduino

 #define GREEN_LED 12 // All Good LED Turns green when data is being sent over UDP Connection
 #define RED_LED 11   // Error LED Turns Red when error is encountered, turns off if all is okay

 #define resetPin 3 // Set reset pin


// --------- GSM section

 #define GSM_DTR  3
 //#define GSM_RING 7
 #define GSM_RX   8
 #define GSM_TX   9
 #define GSM_EN   17
//#define GSM_INT  PD3 // pin 1

 #define DEBUG_TX_PIN 13 // нога вывода отладки

#elif HARDWARE_TYPE==1 // CoolBeacon board

 #define GREEN_LED 7 // All Good LED Turns green when data is being sent over UDP Connection
 #define RED_LED 5   // Error LED Turns Red when error is encountered, turns off if all is okay

// #define resetPin 3 // Set reset pin


// --------- GSM section
 #define GSM_DTR  6  // PD6
 #define GSM_RING A2  
 #define GSM_RX   8  // PB0
 #define GSM_TX   9  // PB1
 #define GSM_EN   10 // PB2

 #define DEBUG_TX_PIN 4 // PD4 нога вывода отладки

#elif HARDWARE_TYPE==2 // CoolBeacon board


 #define GSM_DTR  6  // PD6
 #define GSM_RING A2  
 #define GSM_RX   8  // PB0
 #define GSM_TX   9  // PB1
 #define GSM_EN   10 // PB2


 //-------- Chute section

 #define CHUTE_PIN 4 // PD4 нога 2

  #define Red_LED   5  // PD5    // Error LED Turns Red when error is encountered, turns off if all is okay
  #define Green_LED 7  // PD7    // All Good LED Turns green when data is being sent over UDP Connection

 #define resetPin 3 // Set reset pin

 #define DEBUG_TX_PIN 13 // нога вывода отладки

#else
    #error "Bad Hardware Type"
#endif


#define GSM_SPEED 57600  //38400// 9600 // 19200 //

#define DATA_URL "ykoctpa.ru"
#define DATA_PORT 8888


#define DEBUG_SPEED 57600

#ifdef DEBUG
  #define DBG_PRINTLN(x)     { { debug.print_P(PSTR(x)); debug.println();  debug.wait();  } }
  #define DBG_PRINTVARLN(x)  { { debug.print(#x); debug.print(": "); debug.println(x);  debug.wait();  } }
  #define DBG_PRINTVAR(x)    { { debug.print(#x); debug.print(": "); debug.print(x); debug.print(" ");  } }
  #define DBG_PRINTF(x,...)  { { debug.printf_P(PSTR(x),## __VA_ARGS__); debug.wait();  } }
#else
  #define DBG_PRINTLN(x)     {}
  #define DBG_PRINTVAR(x)    {}
  #define DBG_PRINTVARLN(x)  {}
  #define DBG_PRINTF(x,...)  {}
#endif

#define Red_LED_ON   digitalWrite(RED_LED,HIGH);
#define Red_LED_OFF  digitalWrite(RED_LED,LOW);

#define Green_LED_ON digitalWrite(GREEN_LED,HIGH)
#define Green_LED_OFF digitalWrite(GREEN_LED,LOW)
