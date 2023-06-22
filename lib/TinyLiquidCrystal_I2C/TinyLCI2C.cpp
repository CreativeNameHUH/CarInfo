// https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.cpp
// Based on the work by DFRobot

#include <Wire.h>

#include "TinyLCI2C.h"

TinyLCI2C::TinyLCI2C(uint8_t lcdAddress,  uint8_t lcdRows)
{
    mLcdAddress = lcdAddress;
    mLcdRows    = lcdRows;

    Wire.begin();
    delay(50);

    if (mLcdRows > 1)
        mLcdFunctions = LCD_2LINE;
    else
        mLcdFunctions = 0x00;

    expanderWrite(0);
    delay(1000);

    for (int i = 2; i >= 0; i--)
    {
        write4bits(0x03 << 4);
        delayMicroseconds(4500);
    }

    write4bits(0x02 << 4);

    send(LCD_FUNCTIONSET | mLcdFunctions, 0);
    send(LCD_DISPLAYCONTROL | LCD_DISPLAYON, 0);

    send(LCD_CLEARDISPLAY, 0);
    delayMicroseconds(2000);

    send(LCD_ENTRYMODESET | LCD_ENTRYLEFT, 0);

    send(LCD_RETURNHOME, 0);
    delayMicroseconds(2000);
}

void TinyLCI2C::print(const char *str)
{
    if (str == nullptr)
        return;

    const uint8_t* buffer = (const uint8_t*) str;

    size_t size = strlen(str);
    while (size--)
        send(*buffer++, REGISTER_SELECT);
}

void TinyLCI2C::send(uint8_t data, uint8_t mode)
{
    uint8_t halfData = data & 0xF0;
    write4bits(halfData | mode);

    halfData = (data << 4) & 0xF0;
    write4bits(halfData | mode);
}

void TinyLCI2C::expanderWrite(uint8_t data) const
{
    Wire.beginTransmission(mLcdAddress);
    Wire.write((int) data | LCD_BACKLIGHTVAL);
    Wire.endTransmission();
}

void TinyLCI2C::pulseEnable(uint8_t data)
{
    expanderWrite(data | ENABLE_BIT);
    delayMicroseconds(1);

    expanderWrite(data & ~ENABLE_BIT);
    delayMicroseconds(50);
}

void TinyLCI2C::write4bits(uint8_t data)
{
    expanderWrite(data);
    pulseEnable(data);
}
