#pragma once
#define ARDUINO
#if defined(__MBED__) && !defined(ARDUINO)
#include "mbed.h"
#define delay   ThisThread::sleep_for
#ifndef PROGMEM
#define PROGMEM
#endif
#ifndef boolean
#define boolean bool
#endif
#ifndef bitRead
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#endif
#ifndef bitSet
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#endif
#ifndef bitClear
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#endif
#ifndef bitWrite
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#endif
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif
#else
#include <Arduino.h>
#endif
#ifndef Max7219_h
#define Max7219_h
#include <SPI.h>
#define SPI_DATA_SIZE (sizeof(uint8_t)*_maxDevices*2)
#define SPI_OFFSET(i,x) (((LAST_BUFFER-(i))*2)+(x))
#define MAX_INTENSITY 15
#define NOOP       0
#define DIGIT0     1
#define DIGIT1     2
#define DIGIT2     3
#define DIGIT3     4
#define DIGIT4     5
#define DIGIT5     6
#define DIGIT6     7
#define DIGIT7     8
#define DECODEMODE  9
#define INTENSITY   10
#define SCANLIMIT   11
#define SHUTDOWN    12
#define DISPLAYTEST 15
#define ROW_SIZE  8
#define COL_SIZE  8
#define FIRST_BUFFER 0
#define LAST_BUFFER  (_maxDevices-1)
class Max7219
{
public:
    enum moduleType_t
    {
        GENERIC_HW,
        FC16_HW,
        PAROLA_HW,
        ICSTATION_HW,

        DR0CR0RR0_HW,
        DR0CR0RR1_HW,
        DR0CR1RR0_HW,
        DR0CR1RR1_HW,
        DR1CR0RR0_HW,
        DR1CR0RR1_HW,
        DR1CR1RR0_HW,
        DR1CR1RR1_HW
    };
    Max7219(/*moduleType_t mod,*/ uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices = 1);
    void begin(void);
    void clear(void);
    void command(int dev, int opcode, int param);
private:
    #if defined(__MBED__) && !defined(ARDUINO)
        SPI   _spi;           // Mbed SPI object
        DigitalOut _cs;
    #endif
    typedef struct
    {
        uint8_t dig[ROW_SIZE];  // data for each digit of the MAX72xx (DIG0-DIG7)
        uint8_t changed;        // one bit for each digit changed ('dirty bit')
    } deviceInfo_t;
    moduleType_t _mod;  // The module type from the available list
    bool _hwDigRows;    // MAX72xx digits are mapped to rows in on the matrix
    bool _hwRevCols;    // Normal orientation is col 0 on the right. Set to true if reversed
    bool _hwRevRows;    // Normal orientation is row 0 at the top. Set to true if reversed
    uint8_t _dataPin;     // DATA is shifted out of this pin ...
    uint8_t _clkPin;      // ... signaled by a CLOCK on this pin ...
    uint8_t _csPin;       // ... and LOADed when the chip select pin is driven HIGH to LOW
    bool    _hardwareSPI; // true if SPI interface is the hardware interface
    SPIClass& _spiRef;    // reference to the SPI object to use for hardware comms 
    uint8_t _maxDevices;  // maximum number of devices in use
    deviceInfo_t* _matrix;// the current status of the LED matrix (buffers)
    uint8_t* _spiData;   // data buffer for writing to SPI interface
    bool    _updateEnabled;
    void spiSend(void);
    inline void spiClearBuffer(void);
};
#endif
