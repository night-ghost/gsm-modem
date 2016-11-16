class gsmSerial: public SingleSerial {
public:

        /// Constructor
    gsmSerial(){};

/*    static inline void begin(long baud) { trueSerial.begin(baud); }
    static void end(void)               { trueSerial.end(); }
    static inline uint8_t available(void) { return  available_S(); }
    static inline uint8_t read(void) { return read_S(); }
    static inline size_t write(uint8_t c) { return write_S(c); }     
    static inline uint8_t peek(void) { return trueSerial.peek(); }
    static inline void flush(void) { trueSerial.flush(); }
    static inline void wait() { trueSerial.wait(); }
    static inline size_t write_S(uint8_t c) { return trueSerial.write_S(c); }     
*/
    static uint8_t read_S(void);
    static uint8_t available_S(void);

    static inline void gsmMode(byte b){ _gsm_mode=b;  }
/*
    static inline void print(int n) { trueSerial.print(n); }
    static inline void println() { trueSerial.println(); }
    static inline void println(long l) { trueSerial.println(l); }
    static inline void println(char *cp) { trueSerial.println(cp); }
    static inline void print(char *cp) { trueSerial.print(cp); }
    static inline void print_P(PGM_P s) { trueSerial.print_P(s); }
    
    static void   _printf_P(const prog_char *, ...){
        va_list ap;

        va_start(ap, fmt);
        _vprintf(1, fmt, ap);
        va_end(ap);
    }
*/
private:
    static byte _gsm_mode;
};


byte gsmSerial::_gsm_mode=0;

extern gsmSerial serial;
void check_disconnect(char b);

void readGSM() {         // трансляция всего с GSM
    if(gsm.available_S()) { // Relay All GSM Module communication to Autopilot and Debug ( Debug for monitor/debug only)
        char c;
        Red_LED_ON;
         c=gsm.read_S();
         SingleSerial::write_S(c);
//      debug.print(c);
         check_disconnect(c);
        Red_LED_OFF;
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

