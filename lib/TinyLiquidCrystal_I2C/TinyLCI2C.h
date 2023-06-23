// https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.cpp
// Based on the work by DFRobot

#ifndef CARINFO_TINYLCI2C_H
#define CARINFO_TINYLCI2C_H

#include <cstdio>

class TinyLCI2C
{
public:
    TinyLCI2C(uint8_t lcdAddress, uint8_t lcdRows);
    ~TinyLCI2C() = default;

    void print(const char* str);
private:
    static constexpr uint8_t ENABLE_BIT         = 0x04;
    static constexpr uint8_t REGISTER_SELECT    = 0x01;

    static constexpr uint8_t LCD_2LINE          = 0x08;
    static constexpr uint8_t LCD_RETURNHOME     = 0x02;
    static constexpr uint8_t LCD_FUNCTIONSET    = 0x20;
    static constexpr uint8_t LCD_CLEARDISPLAY   = 0x01;
    static constexpr uint8_t LCD_DISPLAYON      = 0x04;
    static constexpr uint8_t LCD_DISPLAYCONTROL = 0x08;
    static constexpr uint8_t LCD_ENTRYLEFT      = 0x02;
    static constexpr uint8_t LCD_ENTRYMODESET   = 0x04;
#ifdef LCD_NOBACKLIGHT
    static constexpr uint8_t LCD_BACKLIGHTVAL   = 0x00;
#else
    static constexpr uint8_t LCD_BACKLIGHTVAL   = 0x08;
#endif

    uint8_t mLcdAddress;
    uint8_t mLcdRows;
    uint8_t mLcdFunctions;

    void send(uint8_t data, uint8_t mode);

    void expanderWrite(uint8_t data) const;
    void pulseEnable(uint8_t data);
    void write4bits(uint8_t data);
};


#endif //CARINFO_TINYLCI2C_H
