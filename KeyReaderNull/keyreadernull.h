#ifndef __KEYREADERNULL_H__
#define __KEYREADERNULL_H__

#include "keys.h"

class KeyReaderNull
    : public Keys
{
public:
                        KeyReaderNull(I2C & in_cI2C);
                            
    virtual uint8_t     readButtons();
};

#endif // __KEYREADERNULL_H__
