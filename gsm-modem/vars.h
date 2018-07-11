
struct Params {
    uint32_t vers;
    uint32_t port;
    uint32_t mode;
    uint32_t xx;

#define PARAMS_END 4

/* 4 .. 20 */
    char url[64];

/* 21 .. 37 */
    char apn[64]; // beeline.internet.ru


/* 63 END */

} p = { 
            EEPROM_VERSION,    
/* 0 */     DATA_PORT,
            0,          //TCP
            0,          // xx

/* 46 */    DATA_URL,

/* 57 */    "",  // beeline.internet.ru


};


struct StrParam {
    char *ptr;
    byte length;
};

const PROGMEM StrParam strParam[] = {
    { p.url, sizeof(p.url) },
    { p.apn, sizeof(p.apn) },
};

#define SPARAMS_END  (sizeof(strParam)/sizeof(StrParam))


// булевые флаги кучей
struct loc_flags {
    bool mavlink_got; 		// флаг получения пакета
    bool mavlink_active; 	// флаг активности (навсегда)
//    bool mavbeat;		// MAVLink session control

    bool callActive;	// недавно принят вызов
    bool data_link_active;
    bool data_mode;


    bool gsm_ok;	// инициализация удалась

    bool connected;    // в багдаде все спокойно, ничего не оторвалось и мы на внешнем питании - можно не скромничать
    bool motor_armed;
    bool pointDirty;
    bool crash;
    bool hasPower;
};

struct loc_flags lflags = {0,0,0,0,0,0,0}; // все булевые флаги кучей

struct Coord {
    long lat;
    long lon;
} coord;


#define SERIAL_BUFSIZE 128
byte buf[SERIAL_BUFSIZE+1]; // буфер команд с интерфейса и прочих надобностей


// MAVlink data
uint32_t     lastMAVBeat;               // время приема последнего пакета, ms
byte         mav_system;
uint8_t      mav_satellites_visible;    // number of satelites
uint8_t      mav_fix_type;              // GPS lock 0-1=no fix, 2=2D, 3=3D

// эти параметры не нужны пищалке, но могут помочь определить падение и отправить СМС *до* удара
uint8_t      mav_throttle;              // 0..100
long         mav_alt_rel;               // altitude - float from MAVlink!
long         mav_climb;                 // Current climb rate in meters/second
int16_t      mav_pitch;                 // pitch from DCM
int16_t      mav_roll;                  // roll from DCM

byte         mav_mode;
long         mav_alt_error;             // float from MAVlink!  // Current altitude error in meters * 100 

long         mav_baro_alt;              // altitude calculated from pressure * 100
int          mav_xacc;                  // X acceleration (m/s^2) * 1000
int          mav_yacc;                  // Y acceleration (m/s^2) * 1000
int          mav_zacc;                  // Z acceleration (m/s^2) * 1000

// напряжение с полетного контроллера, мв - для проверки собственной измерялки
uint16_t     mav_vbat_A;

// эти вообще не нужны
//byte         apm_mav_system;
//byte         apm_mav_component;
//long       mav_alt_gps;


long         baro_alt_start;              // home's altitude calculated from pressure * 100
long         last_baro_alt;               // to measure difference


// работа со статическими временными переменными намного проще чем со стеком
uint16_t crc; // статический буфер выгоднее 



//-----  EEPROM  --------------------------------------------------------------------

// стартовый адрес параметров в EEPROM
#define EEPROM_PARAMS sizeof(long) // от начала на 4 байта
#define MAX_PARAMS (64-1) // c запасом, перед параметрами - 32 бита CRC

#define EEPROM_CRC 0 // 4 байта с самого начала
#define EEPROM_VERS 2 // CRC занимает 2 байта

//стартовый адрес трека в EEPROM

#define CURRENT_VERSION 11
