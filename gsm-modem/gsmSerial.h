class gsmSerial: public SingleSerial {
public:

    gsmSerial(){};

    static uint8_t read_S(void);
    static uint8_t available_S(void);

    static inline void gsmMode(byte b){ _gsm_mode=b;  }
    static inline bool is_gsmMode(){ return _gsm_mode; }

private:
    static byte _gsm_mode;
};


byte gsmSerial::_gsm_mode=0;

extern gsmSerial serial;
void check_disconnect(char b);

void readGSM() {         // трансляция всего с GSM
    if(gsm.available_S()) { // Relay All GSM Module communication to Autopilot and Debug ( Debug for monitor/debug only)
        char c;
        Green_LED_ON;
         c=gsm.read_S();
         SingleSerial::write_S(c);
//      debug.print(c);
         check_disconnect(c);
        Green_LED_OFF;
    }
}

uint8_t gsmSerial::read_S(void){
    Green_LED_ON;
      char c=SingleSerial::read_S();
      if(_gsm_mode)
          gsm.write_S(c);             // all input translated to GSM
//      debug.print(c);
    Green_LED_OFF;
    return c;
}
uint8_t gsmSerial::available_S(void){
    if(_gsm_mode) readGSM();                  // all GSM input goes to serial
    return SingleSerial::available_S();
}

