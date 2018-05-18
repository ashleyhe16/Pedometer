#ifndef __KEYREADERADAFRUIT_H__
#define __KEYREADERADAFRUIT_H__

#include "keys.h"
#include "Adafruit_RGBLCDShield.h"
#include "MCP23017.h"

class KeyReaderAdafruit
    : public Keys
{
public:
                        KeyReaderAdafruit(I2C & in_cI2C);
                            
    virtual uint8_t     readButtons();
    
protected:
            MCP23017    m_cMCP;
            Adafruit_RGBLCDShield m_cLCD;

};

#endif // __KEYREADERADAFRUIT_H__
