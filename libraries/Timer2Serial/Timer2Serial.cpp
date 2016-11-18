/*

*/

// When set, _DEBUG co-opts pins 11 and 13 for debugging with an
// oscilloscope or logic analyzer.  Beware: it also slightly modifies
// the bit times, so don't rely on it too much at high baud rates
#define _DEBUG 0
#define _DEBUG_PIN1 11
#define _DEBUG_PIN2 13
// 
// Includes
// 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Timer2Serial.h>
#include <util/delay_basic.h>

//
// Statics
//


volatile uint8_t Timer2Serial::_transmitBitMask;
volatile uint8_t Timer2Serial::_transmitInvMask;

volatile uint8_t * Timer2Serial::_transmitPortRegister;
volatile uint8_t Timer2Serial::_tx_delay;

volatile uint8_t Timer2Serial::fInt;
volatile uint8_t Timer2Serial::bit;

byte  Timer2Serial::prescaler;

uint16_t Timer2Serial::subtract_cap(uint16_t num, uint16_t sub) {
  if (num > sub)
    return num - sub;
  else
    return 1;
}



//
// Constructor
//
Timer2Serial::Timer2Serial(uint8_t receivePin, uint8_t transmitPin) {

  setTX(transmitPin);

}

void Timer2Serial::setTX(uint8_t tx)
{
  // First write, then set output. If we do this the other way around,
  // the pin would be output low for a short while before switching to
  // output hihg. Now, it is input with pullup for a short while, which
  // is fine. With inverse logic, either order is fine.
  digitalWrite(tx, HIGH);
  pinMode(tx, OUTPUT);
  
  _transmitBitMask = digitalPinToBitMask(tx);
  _transmitInvMask = ~_transmitBitMask;
  uint8_t port = digitalPinToPort(tx);
  _transmitPortRegister = portOutputRegister(port);
}



//
// Public methods
//

void Timer2Serial::begin(long speed){
    byte div=1;

    switch(speed) {
    case 57600:
	prescaler= (1<<CS21); // prescaler = 8, 1MHz, T=1
	div=8; 
	// _tx_delay = 17;
	break;

    case 38400:
	div=8;
	prescaler= (1<<CS21); // prescaler = 8, 1MHz, T=1
	_tx_delay = 52; // странный глюк Ардуины - работает с 212 то есть в 4 раза быстрее - 153600
	return;
    
	
    case 19200:
    case 9600:
	prescaler= (1<<CS21) | (1<<CS20); // prescaler = 32, 0.25MHz, T=4
	div=32;
	break;
    
    default:
	prescaler= (1<<CS20); // prescaler = 8, 1MHz, T=1
	div=1;
	break;
    }

    // Precalculate the various delays, in number of 4-cycle delays
    _tx_delay = ((F_CPU + (speed/2)) / speed) / div;

    fInt=1; // all clear!
}


uint8_t Timer2Serial::peek() { return 0; }
uint8_t Timer2Serial::available() {return false; }
uint8_t Timer2Serial::read() { return 0; }




ISR(TIMER2_COMPA_vect) {
// таймер в режиме CTC - сам сбросится

    if (Timer2Serial::bit) // choose bit
      *Timer2Serial::_transmitPortRegister |= Timer2Serial::_transmitBitMask; // send 1
    else
      *Timer2Serial::_transmitPortRegister &= Timer2Serial::_transmitInvMask; // send 0

    Timer2Serial::fInt = 1; // было прерывание
}

size_t Timer2Serial::write(uint8_t b){
    if (_tx_delay == 0) {
        setWriteError();
        return 0;
    }

    uint8_t delay = _tx_delay;

    bit=0; // Write the start bit

    TCNT2 = 0;
    OCR2A = delay; // отложим на немного

    TCCR2A = (1<<WGM21);      // CTC mode
    TCCR2B = prescaler;

    TIFR2  |= (1<<OCF2A);   // clear flag
    TIMSK2 |= (1<<OCIE2A);  // разрешим compare interrupt - все время передачи байта пусть тикает

    fInt=0; while(!fInt); // wait for interrupt - begin of start bit

    // Write each of the 8 bits
    for (uint8_t i = 8; i > 0; --i) {
        bit =  (b & 1); // choose bit
        b >>= 1;
        fInt=0; while(!fInt); // wait for interrupt
    }

    // restore pin to natural state - stop bit
    bit = 1;
    fInt=0; while(!fInt); // wait for interrupt - finish of last bit

    fInt=0; while(!fInt); // wait for interrupt - finish of stop bit

    TIMSK2 = 0; // выключить таймер
    TCCR2B = 0;
  
    return 1;
}

void Timer2Serial::flush(){
  // There is no tx buffering, simply return
}


