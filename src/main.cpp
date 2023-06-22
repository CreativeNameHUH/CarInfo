#include <Arduino.h>
#include <../lib/TinyLiquidCrystal_I2C/TinyLCI2C.h>

#include "webServerHandler.h"

// Wiring diagram for increasing the EGT sensor voltage output by 100 mV:
// https://esp32.com/download/file.php?id=1205&sid=b2a025eb132bba33aadf4628eb28b5c8

// Voltages of the EGT sensors, should be checked with the actual sensor datasheet:
// https://www.omega.com/en-us/resources/k-type-thermocouples

// SETTINGS:
constexpr int EGT_SENSOR_PIN       = 36;      // ADC1 CH0
constexpr int VOLTAGE_INCREASE     = 100;     // mV
constexpr int V_MAX                = 950;     // mV
constexpr int ADC_WIDTH            = 12;      // bits
constexpr int ADC_RESOLUTION       = 4095;    // 12 bits
constexpr adc_attenuation_t ADC_DB = ADC_0db; // 100 mV -> 950 mV

constexpr float TEMP_VOLTAGE_MULTIPLIER = 4.096f; // 4.096 mV in 100 °C

constexpr int LCD_ADDRESS    = 0x27; // 0x20?
constexpr int LCD_CHARACTERS = 16;
constexpr int LCD_LINES      = 2;

const char* SSID = "SSID";
const char* PASSWORD = "PASSWORD";


std::unique_ptr<WebServerHandler> serverHandler = nullptr;
std::unique_ptr<TinyLCI2C> lcd = nullptr;

static float getEgtVoltage()
{
    return analogRead(EGT_SENSOR_PIN) * V_MAX * ADC_RESOLUTION;
}

static float getEgtTemp(float egtVoltage)
{
    return (egtVoltage * TEMP_VOLTAGE_MULTIPLIER * 100) - VOLTAGE_INCREASE;
}

void setup()
{
    Serial.begin(115200);
    analogSetWidth(ADC_WIDTH);
    analogSetAttenuation(ADC_DB);

    serverHandler = WebServerHandler::gen(SSID, PASSWORD);

    lcd = make_unique<TinyLCI2C>(LCD_ADDRESS, LCD_LINES);
    lcd->print("Initializing");

    delay(1000);
}

void loop()
{
    float egtVoltage      = getEgtVoltage();
    uint32_t egtVoltage2  = analogReadMilliVolts(EGT_SENSOR_PIN) - VOLTAGE_INCREASE;
    float egtTemp         = getEgtTemp(egtVoltage);

    std::string data = "EGT Voltage: " + std::to_string(egtVoltage) + " V\n" +
                       "EGT Voltage2: " + std::to_string(egtVoltage2) + " mV\n" +
                       "EGT Temp: " + std::to_string(egtTemp) + " °C";

    Serial.println(data.c_str());

    if (serverHandler->isConnected())
        serverHandler->pushData(data);

    // TODO: Temporary solution, this should be change when the button is pressed
    data = "V1: " + std::to_string(egtVoltage);
    lcd->print(data.c_str());
    delay(500);

    data = "V2: " + std::to_string(egtVoltage2);
    lcd->print(data.c_str());
    delay(500);

    data = "Temp " + std::to_string(egtTemp) + " C";
    lcd->print(data.c_str());
}