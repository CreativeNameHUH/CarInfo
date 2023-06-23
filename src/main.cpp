// Voltages of the EGT sensors, should be checked with the actual sensor datasheet:
// https://www.omega.com/en-us/resources/k-type-thermocouples

#include <string>

#include "settings.h"

#ifdef HTTP_SERVER
#include "webServerHandler.h"
std::unique_ptr<WebServerHandler> serverHandler = nullptr;
static void initializeServer()
{
    serverHandler = WebServerHandler::gen(SSID, PASSWORD);
}

static void pushDataToServer(const std::string& data)
{
    if (serverHandler->isConnected())
        serverHandler->pushData(data);
    else
        LOG("Could not upload the data");
}
#else
static void initializeServer() {}

static void pushDataToServer(const std::string& data)
{
    LOG("Server is disabled");
}
#endif

static float getEgtTemp(float egtVoltage)
{
    return (float)egtVoltage * (100 / TEMP_VOLTAGE_MULTIPLIER);
}

void setup()
{
    SERIAL_INIT(115200);

    initializeDisplay();
    initializeServer();
    delay(250);
    initializeADC();


    delay(1000);
}

void loop()
{

    clearDisplay();

    float egtVoltage = getADCVoltage();
    const float egtTemp = getEgtTemp(egtVoltage);

    std::string data = "EGT Voltage: " + std::to_string(egtVoltage) + " mV<br>" +
                       "EGT Temp: " + std::to_string(egtTemp) + " &deg;C";

    LOG(data.c_str());
    pushDataToServer(data);

    // TODO: Temporary solution, this should be change when the button is pressed
    data = "V1: " + std::to_string(egtVoltage);
    displayText(data.c_str(), 0, 0);
    data = "Temp " + std::to_string(egtTemp) + " C";
    displayText(data.c_str(), 0, 20);

    egtVoltage++;
    delay(1000);
}
