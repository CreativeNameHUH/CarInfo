#include <Arduino.h>
#include <../lib/TinyLiquidCrystal_I2C/TinyLCI2C.h>

#include "webServerHandler.h"
#include "settings.h"

// Wiring diagram for increasing the EGT sensor voltage output by 100 mV:
// https://esp32.com/download/file.php?id=1205&sid=b2a025eb132bba33aadf4628eb28b5c8

// Voltages of the EGT sensors, should be checked with the actual sensor datasheet:
// https://www.omega.com/en-us/resources/k-type-thermocouples

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