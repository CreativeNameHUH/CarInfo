# CarInfo
### **ESP32-S2** EGT sensor reader.
Currently, supports a simple website and an OLED display on Wemos S2 Pico for displaying EGT and EGT sensor voltage.

### ESP32 Support:
It is possible to use it with the regular ESP32 but an addition hardware is needed to increase the voltage of the sensor which is usually bellow 100 mV.
To enable it ESP32_DEPRECATED has to be defined.
> #define ESP32_DEPRECATED

**Any further changes can break ESP32 support!** 
## Usefull stuff:
### Typical [voltage output](https://www.omega.com/en-us/resources/k-type-thermocouples) for the K type Thermocouple
 