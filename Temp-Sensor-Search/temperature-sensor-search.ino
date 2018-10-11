/*
 * Project:     temperature-sensor-search.ino
 * Description: Quick sketch to read and return HEX address of connected DS18B20
 *              sensors so that you can hard code the addresses into sketches
 *              with multiple sensors.
 * Author:      Richard J. Sears (richard@sears.net)
 * Date:        October 11th, 2018
 * Version:     1.0
 * Base Code:   Modified from EmonCMS Temp Sensor Probe (emonTH) to Photon. 
 */

// DS18B20 temperature sensor address discovery script
// - for finiding hardware addresses of one or more DS18B20 sensors connected to  one-wire bus

// Connections:
// Data wire is plugged into port D4 on the Photon
// Ground is plugged into GND
// Red is plugged into 3v3

// THIS SKETCH REQUIRES:

// Libraries:
//
//	- OneWire library	http://www.pjrc.com/teensy/td_libs_OneWire.html
//	- DallasTemperature	http://download.milesburton.com/Arduino/MaximTemperature

#include "DallasTemperature.h"
#include "OneWire.h"



#define ONE_WIRE_BUS D4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("Temperature search");
  Serial.println("waiting 6 seconds before printing");
  delay(6000);

  sensors.begin();
  
  DeviceAddress tmp_address;
  int numberOfDevices = sensors.getDeviceCount();
  
  for(int i=0;i<numberOfDevices; i++)
  {
    sensors.getAddress(tmp_address, i);
    printAddress(tmp_address);
    Serial.println();
  }

}

void loop()
{ 
 // Nothing to see here, move along....
}


void printAddress(DeviceAddress deviceAddress)
{
  Serial.print("{ ");
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    Serial.print("0x");
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i<7) Serial.print(", ");
    
  }
  Serial.print(" }");
}
