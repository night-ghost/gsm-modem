
#define TELEMETRY_SPEED 57600
#define USE_MAVLINK 1
#define AUTOBAUD 1

#define USE_GPRS 1

#define GREEN_LED 12 // All Good LED Turns green when data is being sent over UDP Connection
#define RED_LED 11   // Error LED Turns Red when error is encountered, turns off if all is okay

#define resetPin 3 // Set reset pin


// --------- GSM section

#define GSM_DTR  6
//#define GSM_RING 7
#define GSM_RX   8
#define GSM_TX   9
#define GSM_EN   10
//#define GSM_INT  PD3 // pin 1

#define GSM_SPEED 38400// 9600 // 19200 //

//#define GSM_SMS_URL "maps.google.ru/maps?q="     // форматировать координвты под ссылку на карты гугля
#define GSM_SMS_URL "ykoctpa.ru/map?q="          // а тут короче и есть Яндекс



#define DEBUG_TX_PIN 4 // нога вывода отладки

#ifdef DEBUG
  #define DBG_PRINTLN(x)     { serial.print_P(PSTR("#" x)); serial.println();  serial.wait();  }.
  #define DBG_PRINTVARLN(x)  { serial.write('#'); serial.print(#x); serial.print(": "); serial.println(x);  serial.wait();  }
  #define DBG_PRINTVAR(x)    { serial.write('#'); serial.print(#x); serial.print(": "); serial.print(x); serial.print(" ");  }
  #define DBG_PRINTF(x,...)  { serial.printf_P(PSTR("#" x),## __VA_ARGS__); serial.wait();  }
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
