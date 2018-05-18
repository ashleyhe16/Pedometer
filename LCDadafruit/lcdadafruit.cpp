#include "mbed.h"
#include "lcdadafruit.h"

LCDadafruit::LCDadafruit(I2C & in_cI2C)
    : LCD(in_cI2C)
    , m_cMCP(in_cI2C, 0x20 << 1)
    , m_cLCD(m_cMCP)
{
    m_cLCD.begin(16,2);
}

int LCDadafruit::_putc(int in_nValue)
{
    return m_cLCD._putc(in_nValue);
}

void LCDadafruit::clear()
{
    m_cLCD.clear();
}

uint8_t LCDadafruit::columns()
{
    return 16;
}

void LCDadafruit::createChar(uint8_t location, uint8_t charmap[])
{
    m_cLCD.createChar(location,charmap);
}

void LCDadafruit::home()
{
    m_cLCD.home();
}

uint8_t LCDadafruit::rows()
{
    return 2;
}

void LCDadafruit::setCursor(uint8_t in_nX, uint8_t in_nY)
{
    m_cLCD.setCursor(in_nX,in_nY);
}

void LCDadafruit::showBlink(bool in_bShow)
{
    if (in_bShow) m_cLCD.blink(); else m_cLCD.noBlink();    
}

void LCDadafruit::showCursor(bool in_bShow)
{
    if (in_bShow) m_cLCD.cursor(); else m_cLCD.noCursor();
}

void LCDadafruit::showDisplay(bool in_bShow)
{
    if (in_bShow) m_cLCD.display(); else m_cLCD.noDisplay();
}
