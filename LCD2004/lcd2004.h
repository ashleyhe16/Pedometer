#ifndef __LCD2004_H__
#define __LCD2004_H__

#include "lcd.h"

class LCD2004
    : public LCD
{
public:
                        LCD2004(I2C & in_cI2C);
    virtual int         _putc(int c);
    
    virtual uint8_t     columns();
    virtual void        clear();
    virtual void        createChar(uint8_t location, uint8_t charmap[]);
    virtual void        home();
    virtual uint8_t     rows();
    virtual void        setCursor(uint8_t in_nX, uint8_t in_nY);
    virtual void        showBlink(bool in_bShow);
    virtual void        showCursor(bool in_bShow);
    virtual void        showDisplay(bool in_bShow);

protected:
            uint8_t     read_reg(void);
            uint8_t     remap(uint8_t in_nValue);
            void        setDisplayControl(uint8_t in_nReg, bool in_bEnable);
            void        write_data(uint8_t in_nReg, uint8_t in_nValue);
            void        write_reg(uint8_t in_nValue);
    
protected:
            uint8_t     m_nDisplayControl;
};

#endif // __LCD2004_H__
