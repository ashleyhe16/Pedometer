#ifndef __LCDADAFRUIT_H__
#define __LCDADAFRUIT_H__

#include "lcd.h"
#include "Adafruit_RGBLCDShield.h"
#include "MCP23017.h"

class LCDadafruit
    : public LCD
{
public:
                        LCDadafruit(I2C & in_cI2C);
    virtual int         _putc(int c);
    
    virtual uint8_t     columns();
    virtual void        clear();
    virtual void        createChar(uint8_t location, uint8_t charmap[]);
    virtual void        home();
    virtual uint8_t     rows();
    virtual void        setCursor(uint8_t in_nX, uint8_t in_nY);
    virtual void        showBlink(bool in_bBlink);
    virtual void        showCursor(bool in_bShow);
    virtual void        showDisplay(bool in_bBlink);
    
protected:
            MCP23017    m_cMCP;
            Adafruit_RGBLCDShield m_cLCD;
};

#endif // __LCDADAFRUIT_H__
