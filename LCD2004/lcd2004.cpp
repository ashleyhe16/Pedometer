#include "mbed.h"
#include "lcd2004.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAY 0x04
#define LCD_CURSOR 0x02
#define LCD_BLINK 0x01

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

#define ADDR     0x4E
#define PIN_E    1<<2
#define PIN_RW   1<<1
#define PIN_RS   1<<0
#define PIN_D4   1<<4
#define PIN_D5   1<<5
#define PIN_D6   1<<6
#define PIN_D7   1<<7
#define PIN_BL   1<<3

Serial cPC(USBTX, USBRX);

const uint8_t k_aMapper[] =
{
    PIN_D4,
    PIN_D5,
    PIN_D6,
    PIN_D7,
};

LCD2004::LCD2004(I2C & in_cI2C)
    : LCD(in_cI2C)
    , m_nDisplayControl(LCD_DISPLAY)
{
    ::wait_ms(100);    
    write_reg(PIN_D5 | PIN_D4);
    ::wait_ms(5);
    write_reg(PIN_D5 | PIN_D4);
    ::wait_us(100);
    write_reg(PIN_D5 | PIN_D4);
    
    // IV
    write_reg(PIN_D5);
    write_reg(PIN_D5);
    write_reg(PIN_D7);
    write_reg(0);
    write_reg(PIN_D7);
    write_reg(0);
    write_reg(PIN_D4);
    write_reg(0);
    write_reg(PIN_D7 | PIN_D6 /* | PIN_D5 | PIN_D4 */); // D5 = cursor on D4 = BLINK
}

int LCD2004::_putc(int in_nValue)
{
    write_data(PIN_RS, in_nValue);    
    return 0;
}

void LCD2004::clear()
{
    write_data(0, LCD_CLEARDISPLAY);
}

uint8_t LCD2004::columns()
{
    return 20;
}

void LCD2004::createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    write_data(0, LCD_SETCGRAMADDR | (location << 3));
    
    for (int i=0; i<8; i++)
    {
        _putc(charmap[i]);
    }
    
    write_data(0, LCD_SETDDRAMADDR);  // unfortunately resets the location to 0,0
}

void LCD2004::home()
{
    write_data(0, LCD_RETURNHOME);
}

uint8_t LCD2004::read_reg(void)
{
    char nData = PIN_RW | PIN_BL;
    m_cI2C.write(ADDR,&nData,1);
    
    nData = PIN_RW | PIN_BL | PIN_E;
    m_cI2C.write(ADDR,&nData,1);

    ::wait_ms(0.05);

    char nValue = 0;    
    m_cI2C.read(ADDR,&nValue,1);

    nData = PIN_RW | PIN_BL;
    m_cI2C.write(ADDR,&nData,1);
    
    ::wait_ms(0.05);
    
    return nValue & (PIN_D7 | PIN_D6 | PIN_D5 | PIN_D4);
}

uint8_t LCD2004::remap(uint8_t in_nValue)
{
    uint8_t nValue = 0;
    for (size_t i = 0; i < 4; i++)
    {
        if (in_nValue & (1 << i)) nValue |= k_aMapper[i];
    }
    
    return nValue;
}

uint8_t LCD2004::rows()
{
    return 4;
}

void LCD2004::setCursor(uint8_t in_nX, uint8_t in_nY)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    // D7 = Set DDRAM address
    uint8_t nValue = LCD_SETDDRAMADDR | (in_nX + row_offsets[in_nY]);
    write_data(0, nValue);
}

void LCD2004::setDisplayControl(uint8_t in_nReg, bool in_bEnable)
{
    uint8_t nDisplayControl = m_nDisplayControl;
    
    if (in_bEnable)
    {
        m_nDisplayControl |= in_nReg;
    }
    else
    {
        m_nDisplayControl &= ~in_nReg;
    }

    if (nDisplayControl != m_nDisplayControl)
    {
        write_data(0,LCD_DISPLAYCONTROL | m_nDisplayControl);
    }
}

void LCD2004::showBlink(bool in_bShow)
{
    setDisplayControl(LCD_BLINK,in_bShow);
}

void LCD2004::showCursor(bool in_bShow)
{
    setDisplayControl(LCD_CURSOR, in_bShow);
}

void LCD2004::showDisplay(bool in_bShow)
{
    setDisplayControl(LCD_DISPLAY,in_bShow);
}

void LCD2004::write_data(uint8_t in_nReg, uint8_t in_nValue)
{
    write_reg(in_nReg | remap(in_nValue >> 4));
    write_reg(in_nReg | remap(in_nValue & 0x0F));    
}

void LCD2004::write_reg(uint8_t in_nValue)
{
    char nData = PIN_E | PIN_BL | in_nValue;
    m_cI2C.write(ADDR,&nData,1);
        
    ::wait_us(1450);
    
    nData = PIN_BL | in_nValue;
    m_cI2C.write(ADDR,&nData,1);    
}
