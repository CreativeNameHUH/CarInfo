//
// Created by Hubert on 23.06.2023.
//

#ifndef CARINFO_SETTINGS_H
#define CARINFO_SETTINGS_H

#include <Arduino.h>

static constexpr int EGT_SENSOR_PIN       = 36;      // ADC1 CH0
static constexpr int VOLTAGE_INCREASE     = 100;     // mV
static constexpr int V_MAX                = 950;     // mV
static constexpr int ADC_WIDTH            = 12;      // bits
static constexpr int ADC_RESOLUTION       = 4095;    // 12 bits
static constexpr adc_attenuation_t ADC_DB = ADC_0db; // 100 mV -> 950 mV

static constexpr float TEMP_VOLTAGE_MULTIPLIER = 4.096f; // 4.096 mV in 100 °C

static constexpr int LCD_ADDRESS = 0x27; // 0x20?
static constexpr int LCD_LINES   = 2;

static const char* SSID = "SSID";
static const char* PASSWORD = "PASSWORD";

#endif //CARINFO_SETTINGS_H
