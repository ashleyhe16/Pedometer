#include "mbed.h"
#include "keyreadernull.h"

KeyReaderNull::KeyReaderNull(I2C & in_cI2C)
    : Keys(in_cI2C)
{
}

uint8_t KeyReaderNull::readButtons()
{
    return eButtonNone;
}
