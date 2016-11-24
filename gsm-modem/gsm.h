#ifndef gsm_h
#define gsm_h


/*
    простенький класс для работы с SIM800

*/

#define RESPONCE_LENGTH 200 // ответ на USSD бывает весьма длинным


/*
для работы с GSM используется слегка модифицированная библиотека AltSoftSerial

изменения: наследование не от Stream а от BetterStream
добавлена возможность опроса состояния
некоторая оптимизация
статические read() write() available()

*/

extern byte buf[];

class GSM: public AltSoftSerial
{
  public:
    GSM(void);
    static bool begin();
    static bool begin(long speed);
    static void end();

    static void initGSM(void);
    static void readOut(void);
    static bool set_sleep(byte mode);
    static void turnOff();
    static uint8_t wait_answer(const char* answer, const char* answer2, unsigned int timeout);
    static uint8_t wait_answer(const char* answer, unsigned int timeout);
    
    static uint8_t command_P(const char* cmd, const char* answer, const char* answer2=NULL, uint16_t time=1000);

    static uint8_t command_P(const char* cmd, const char* answer, uint16_t time){
        return command_P(cmd, answer, NULL, time);
    }


    static uint8_t command_P(const char* cmd){
        return command_P(cmd, s_ok);
    }

    static uint8_t command_P(const char* cmd, uint16_t time){
        return command_P(cmd, s_ok, time);
    }

    static uint8_t command(char* cmd, const char* answer2=NULL, uint16_t time=1000);


    static byte sendSMS(const char * phone, const char * text);
    static byte sendUSSD(uint16_t text);
    static char * getRSSI(void);
    static int balance(byte n);
    static bool initGPRS();
    static bool setAPN(char*);
    static bool initUDP(uint16_t);
    static bool connectUDP(char *url, uint16_t port);
    static void doOnDisconnect();
    static void pulseDTR();
    static void pulseReset();
    static bool cregWait();
    static bool syncSpeed();

//  private:
    static char GSM_response[RESPONCE_LENGTH];
    static byte lastError;
    static byte isTransparent;
    static byte isActive;
    static const PROGMEM char s_ok[];
    
    static byte isGPRS;
};

#endif