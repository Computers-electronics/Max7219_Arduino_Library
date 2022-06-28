#include"Max7219.h"
#ifdef ARDUINO 
#include <Arduino.h>
#endif
Max7219::Max7219(/*moduleType_t mod, */ uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices) :
    _dataPin(dataPin), _clkPin(clkPin), _csPin(csPin),
    _hardwareSPI(false), _spiRef(SPI), _maxDevices(numDevices), _updateEnabled(true)
#if defined(__MBED__) && !defined(ARDUINO)
    , _spi((PinName)dataPin, NC, (PinName)clkPin), _cs((PinName)csPin)
#endif
{
    
}
void Max7219::command(int dev, int opcode, int param) {
    spiClearBuffer();
    _spiData[SPI_OFFSET(dev, 0)] = opcode;
    _spiData[SPI_OFFSET(dev, 1)] = param;
    spiSend();
}
inline void Max7219::spiClearBuffer(void)
{
    memset(_spiData, NOOP, SPI_DATA_SIZE);
}
void Max7219::spiSend(void)
{
#ifdef ARDUINO
    // initialize the SPI transaction
    if (_hardwareSPI)
        _spiRef.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);

    // shift out the data
    if (_hardwareSPI)
    {
        for (uint16_t i = 0; i < SPI_DATA_SIZE; i++)
            _spiRef.transfer(_spiData[i]);
    }
    else  // not hardware SPI - bit bash it out
    {
        for (uint16_t i = 0; i < SPI_DATA_SIZE; i++)
            shiftOut(_dataPin, _clkPin, MSBFIRST, _spiData[i]);
    }

    // end the SPI transaction
    digitalWrite(_csPin, HIGH);
    if (_hardwareSPI)
        _spiRef.endTransaction();
#else
    _cs = 0;
    _spi.write((const char*)_spiData, SPI_DATA_SIZE, nullptr, 0);
    _cs = 1;
#endif
}
void Max7219::begin(void)
{
    // initialize the SPI interface
#ifdef ARDUINO
    if (_hardwareSPI)
    {
        //PRINTS("\nHardware SPI");
        _spiRef.begin();
    }
    else
    {
        //PRINTS("\nBitBang SPI")
            pinMode(_dataPin, OUTPUT);
        pinMode(_clkPin, OUTPUT);
    }

    // initialize our preferred CS pin (could be same as SS)
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
#else
    _cs = 1;
#endif

    //setShiftDataInCallback(nullptr);
    //setShiftDataOutCallback(nullptr);

    _matrix = (deviceInfo_t*)malloc(sizeof(deviceInfo_t) * _maxDevices);
    _spiData = (uint8_t*)malloc(SPI_DATA_SIZE);
    for (int i = 0; i < _maxDevices;i++) {
        command(i,DISPLAYTEST, 0);
        command(i,SCANLIMIT, ROW_SIZE - 1);
        command(i,INTENSITY, MAX_INTENSITY / 2);
        command(i,DECODEMODE, 0);
        clear();
        command(i,SHUTDOWN, 1);
    }
}
void Max7219::clear(void){
    for (int a = 0; a < _maxDevices; a++) {
        for (int b = 0; b < ROW_SIZE; b++) {
            command(a, b+1, 0);
        }
    }
}