#include "mbed.h"
#include "keyreaderadafruit.h"

KeyReaderAdafruit::KeyReaderAdafruit(I2C & in_cI2C)
    : Keys(in_cI2C)
    , m_cMCP(in_cI2C, 0x20 << 1)
    , m_cLCD(m_cMCP)
{
    m_cLCD.begin(16,2);
}

uint8_t KeyReaderAdafruit::readButtons()
{
    int nKeys = m_cLCD.readButtons();
    uint8_t nKeysOut = eButtonNone;
    
    if (nKeys & BUTTON_UP) nKeysOut |= eButtonUp;
    if (nKeys & BUTTON_DOWN) nKeysOut |= eButtonDown;
    if (nKeys & BUTTON_LEFT) nKeysOut |= eButtonLeft;
    if (nKeys & BUTTON_RIGHT) nKeysOut |= eButtonRight;
    if (nKeys & BUTTON_SELECT) nKeysOut |= eButtonSelect;
    
    return nKeysOut;
}
