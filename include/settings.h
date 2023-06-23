//
// Created by Hubert on 23.06.2023.
//

#ifndef CARINFO_SETTINGS_H
#define CARINFO_SETTINGS_H

#include <Arduino.h>

#include "bootlegUniquePtr.h"

#define HTTP_SERVER // Enables http server
#define DEBUG       // Enables debug serial print
#define USE_LCD

#ifdef DEBUG
#define SERIAL_INIT(baud) Serial.begin(baud)
#define LOG(...) Serial.println(__VA_ARGS__)
#else
#define SERIAL_INIT(baud) NOP()
#define LOG(...) NOP()
#endif

static constexpr float TEMP_VOLTAGE_MULTIPLIER = 4.096f; // 4.096 mV in 100 °C

#ifdef HTTP_SERVER
static const char* SSID = "SSID";
static const char* PASSWORD = "PASSWORD";
#endif

#ifndef ESP32_DEPRECATED
#include <driver/adc.h>
#include <esp_adc_cal.h>

static constexpr adc1_channel_t EGT_SENSOR_PIN = ADC1_CHANNEL_0;   // ADC1 PIN1
static constexpr adc_bits_width_t ADC_WIDTH    = ADC_WIDTH_BIT_13; // bits
static constexpr adc_atten_t ADC_DB            = ADC_ATTEN_DB_0;   // 0 mV -> 750 mV
static constexpr uint32_t ADC_MAX_VOLTAGE      = 750;              // mV
static constexpr uint32_t ADC_MAX_READING      = 4095;             // 13 bits, continuous read mode

static esp_adc_cal_characteristics_t* gADCCharacteristic = nullptr;

static void initializeADC()
{
    /*if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == 0)
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_DB, ADC_WIDTH, 0, gADCCharacteristic);*/

    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(EGT_SENSOR_PIN, ADC_DB);
}

static float getADCVoltage()
{
    if (gADCCharacteristic)
        return (float)esp_adc_cal_raw_to_voltage(adc1_get_raw(EGT_SENSOR_PIN), gADCCharacteristic);

    return (float)adc1_get_raw(EGT_SENSOR_PIN) * ADC_MAX_VOLTAGE / ADC_MAX_READING;
}

#ifdef USE_LCD
#include <SSD1306Wire.h>

static constexpr bool DISPLAY_FLIPPED = true;

std::unique_ptr<SSD1306Wire> gDisplay = nullptr;

static void displayText(const String& data, int16_t x = 0, int16_t y = 0)
{
    if (!gDisplay)
    {
        LOG("Display is not initialized!");
        return;
    }

    gDisplay->drawString(x, y, data);
    gDisplay->display();
}

static void initializeDisplay()
{
    gDisplay = make_unique<SSD1306Wire>(0x3C, SDA, SCL, GEOMETRY_128_32);

    gDisplay->init();
    delay(250);
    gDisplay->clear();
    delay(100);

    if (DISPLAY_FLIPPED)
        gDisplay->flipScreenVertically();

    gDisplay->setFont(ArialMT_Plain_10);
    gDisplay->setColor(WHITE);
    delay(100);
    displayText("Initializing...", 0, 10);
}

static void clearDisplay()
{
    if (!gDisplay)
        return;

    gDisplay->clear();
    delay(200);
}
#endif //USE_LCD
#else
static constexpr int EGT_SENSOR_PIN       = 36;      // ADC1 CH0
static constexpr int VOLTAGE_INCREASE     = 100;     // mV
static constexpr int ADC_WIDTH            = 12;      // bits
static constexpr adc_attenuation_t ADC_DB = ADC_0db; // 100 mV -> 950 mV
static constexpr int ADC_MAX_VOLTAGE      = 950;     // mV
static constexpr int ADC_MAX_READING      = 4095;    // 12 bits

static void initializeADC()
{
    analogSetWidth(ADC_WIDTH);
    analogSetAttenuation(ADC_DB);
}

static float getADCVoltage()
{
    return analogReadMilliVolts(EGT_SENSOR_PIN) - VOLTAGE_INCREASE;
}

#ifdef  USE_LCD
#include <../lib/TinyLiquidCrystal_I2C/TinyLCI2C.h>

static constexpr int LCD_ADDRESS = 0x27; // 0x20?
static constexpr int LCD_LINES   = 2;

static std::unique_ptr<TinyLCI2C> gDisplay = nullptr;

static void displayText(const char* data, int x = 0, int y = 0)
{
    if (gDisplay)
        gDisplay->print(data);
    else
        LOG("Display is not initialized!");
}

static void initializeDisplay()
{
    gDisplay = make_unique<TinyLCI2C>(LCD_ADDRESS, LCD_LINES);
    gDisplay->print("Initializing");
}

static void clearDisplay()
{
    return;
}
#endif //USE_LCD
#endif //ESP32_S2
#ifndef USE_LCD
static void displayText(const char* data, int x = 0, int y = 0) {}
static void initializeDisplay() {}
static void clearDisplay() {}
#endif //USE_LCD
#endif //CARINFO_SETTINGS_H
