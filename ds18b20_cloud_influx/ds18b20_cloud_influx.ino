/*
 * Project:     ds18b20-cloud-influx.ino
 * Description: Quick sketch to read and return temperature using connected DS18B20
 *              sensors. Written for a Moteino and converted to a Photon. This sketch
 *              also (if so configured) transmits the temperature datae to an Influx
 *              database via UDP (read InfluxDB docs to setup Influx), publishes the
 *              temperature to the Particle Console and exposes the temperature as a
 *              Patricle Variable (via the console or Python, etc). 
 *
 *              Anytime I use the DS18B20 temp probes I hard code their addresses so
 *              they don't move around on me when I reboot. You can look on my github
 *              (https://github.com/rjsears/Particle-Photon-Examples/tree/master/Temp-Sensor-Search)
 *              to grab a sketch that lists all of your sensor addresses.
 *
 * Author:      Richard J. Sears (richard@sears.net)
 * Date:        October 11th, 2018
 * Version:     1.0
 * Base Code:   Modified from Low Power Labs Moteino code 
 *
 * DS18B20 Connections:
 * Data wire is plugged into port D4 on the Photon
 * Ground is plugged into GND
 * Red is plugged into 3v3
 * NOTE: Because in my particular case my Photon is powered 24x7,
         I do not need to worry about powering the DS18B20 on and
         off so I have commented out the powerup/power down sections
         of this sketch. If you need power savings, you can uncomment
         them and use them as you see fit. I suspect you will need 
         additional power savings on top of just shutting down the
         DS19B20.
 */

#include <OneWire.h>
#include "DallasTemperature.h"

// Set Approperate settings here:
// 1 = ON, 0 = Off
#define DEBUG    1     // Do you want to output debug meaages?
#define INFLUXDB 1     // Are we using an InfluxDB?
#define PUBLISH  1     // Do we ant to publish events to the Particle Cloud?
#define EXT_ANT  1     // Are we using an External antenna on our Photon?
#define PART_VAR 1     // Publish a Particle Variable for use elsewhere?

// If we are going to use UDP, set yuur server info here:
UDP Udp;
IPAddress remoteIP(10, 200, 50, 12);
int port = 8888;

/*
 Monitoring configuration
 ========================
 
  - how long to wait between readings, in minutes
  - DS18B20 temperature precision:
      9bit: 0.5C,  10bit: 0.25C,  11bit: 0.1125C, 12bit: 0.0625C
  - Required delay when reading DS18B20
      9bit: 95ms,  10bit: 187ms,  11bit: 375ms,   12bit: 750ms
 */
const int MS_BETWEEN_READINGS = 15000; // miliseconds between readings (10000 = 10 Seconds, 30000 = 30 Seconds, 60000 = 1 Minunte)
const int TEMPERATURE_PRECISION = 11;
const int ASYNC_DELAY           = 375;

double externalTemp1;

// Define the number of busses we want to use based on number of sensors attached to Photon:
#define ONE_WIRE_BUS_A    D4     // First Temp Sensor Connected to D4 on the Photon
// #define ONE_WIRE_BUS_B    D5     // Second Temp Sensor
OneWire oneWireA(ONE_WIRE_BUS_A);
// OneWire oneWireB(ONE_WIRE_BUS_B);
DallasTemperature sensorsA(&oneWireA);    // First Sensor
// DallasTemperature sensorsB(&oneWireB);    // Second Sensor



/*
  External sensor addresses
  =========================
 Hardcoding these guarantees inputs won't flip around if you replace or add sensors.
 Use one of the address finding sketches to determine your sensors' unique addresses.
 https://github.com/rjsears/Particle-Photon-Examples/tree/master/Temp-Sensor-Search
 
 You MUST replace this DeviceAddress with the EHX address of YOUR sensor!!

 Very easy to extend if you have more sensors.
 */
 
DeviceAddress EXT_SENSOR1 = {
 0x28, 0x8F, 0xDF, 0x6A, 0x1F, 0x13, 0x01, 0x98 };
// DeviceAddress EXT_SENSOR2 = {
//  0x28, 0xFF, 0xDD, 0x10, 0x76, 0x15, 0x01, 0xB1 };

boolean EXT_SENSOR1_PRESENT;
// boolean EXT_SENSOR2_PRESENT;




// Begin Setup
void setup() {
  if (DEBUG)
    print_welcome_message();
      
  if (EXT_ANT)
    setup_external_antenna();

  if (INFLUXDB)
    Udp.begin(8888);

  // Initialise sensors
  initialise_EXTERNAL_DS18B20();
}

/**
 * Get temperature
 */
void loop()
{
  // External temperature readings - if the sensor is connected:
  if (EXT_SENSOR1_PRESENT)
    take_ds18b20_reading();

  if (DEBUG)
    print_payload();
      
  if (PUBLISH)
    transmit_payload_to_particle();
    
  if (INFLUXDB)
    transmit_payload_to_influxdb();
    
  if (PART_VAR)
    publish_temperature_variable();
    
  delay(MS_BETWEEN_READINGS);
}


/**
 * Find the expected DS18B20 sensors
 * You will need your sensors' unique address (obtain using one of the sketches designed for this purpose).
 * Straightforward to add support for additional sensors by extending this function.
 */
void initialise_EXTERNAL_DS18B20()
{
  sensorsA.begin();
  // Disable automatic temperature conversion to reduce time spent awake, instead we sleep for ASYNC_DELAY
  // see http://harizanov.com/2013/07/optimizing-ds18b20-code-for-low-power-applications/
  sensorsA.setWaitForConversion(false);
  
  // Is the sensor present?
  EXT_SENSOR1_PRESENT = sensorsA.getAddress(EXT_SENSOR1, 0);
//  EXT_SENSOR2_PRESENT = sensorsA.getAddress(EXT_SENSOR2, 1);

  EXT_SENSOR1_PRESENT = EXT_SENSOR1_PRESENT ? EXT_SENSOR1_PRESENT : sensorsA.getAddress(EXT_SENSOR1, 1);

  // No luck?
  if (DEBUG){
    
    if (!EXT_SENSOR1_PRESENT)
      Serial.println("Unable to find address for DS18B20 External 1... check hard coded address");
    else
      Serial.println("Found DS18B20 External 1");
      
/*
    if (!EXT_SENSOR2_PRESENT)
      Serial.println("Unable to find address for DS18B20 External 2... check hard coded address");
    else
      Serial.println("Found DS18B20 External 2");
  */    
    // .. and for sensor 3, etc...
  }

}

/**
 * Convenience method; read from all DS18B20s
 * You will need to extend this to read from any extra sensors.
 */
void take_ds18b20_reading ()
{
  // Set precision to desired value
  sensorsA.setResolution(EXT_SENSOR1, TEMPERATURE_PRECISION);
  //sensorsB.setResolution(EXT_SENSOR2, TEMPERATURE_PRECISION);
  
  // Get readings. We must wait for ASYNC_DELAY due to power-saving (waitForConversion = false)
  sensorsA.requestTemperatures();
  delay(ASYNC_DELAY);
  float temp1=(sensorsA.getTempF(EXT_SENSOR1)); //was float

  // Payload will maintain previous reading unless the temperature is within range.
  // Keeps out errors
  if (temperature_in_range(temp1))
      externalTemp1 = temp1; // Round to nice even numbers....
}


/**
 * For debugging purposes: print the payload as it will shortly be sent
 * Wise to extend this if you have extra sensors wired in.
 */
void print_payload()
{
 
  Serial.println("Temperature payload: ");
  Serial.print("  External sensor 1: ");

  if (EXT_SENSOR1_PRESENT){
    Serial.print( externalTemp1);
    Serial.println("F");
  
  if (PUBLISH)
    Serial.println("Temp Published to Photon Cloud");
  
  if (INFLUXDB)
    Serial.println("Temp Sent to INFLUXDB");
    
  if (PART_VAR)
    Serial.println("Temperature Variable published to Photon Cloud");
     
  }
  else {
    Serial.println(" not present");
  }
/*    
  Serial.print("  External sensor 2: ");

  if (EXT_SENSOR2_PRESENT){
    Serial.print( externalTemp2/10.0);
    Serial.println("F");
  }
  else {
    Serial.println(" not present");
  }
*/
  Serial.println();
  delay(100);
}


void transmit_payload_to_particle()
{ 
  // Now that we have the readings, we can publish them to the cloud
  String temperature = String(externalTemp1); // store temp in "temperature" string
  Particle.publish("temperature", temperature, PRIVATE); // publish to cloud
}

// Here is where we transmit our temperature reading to an InfluxDB
// utilizing UDP.
void transmit_payload_to_influxdb()
{
   String udp_temp2 = String(externalTemp1);
   String udptemp = String("temperature value=" + udp_temp2);
   Udp.beginPacket(remoteIP, port);
   Udp.write(udptemp);
   Udp.endPacket();
}


void publish_temperature_variable()
 {
   Particle.variable("externalTemp1", externalTemp1);
 }

/**
 * Dumps useful intro to serial
 */
void print_welcome_message()
{
  Serial.begin(9600);
  Serial.println("Temperature Monitor w/DS18B20");
  delay(100);
}


/**
 * validate that the provided temperature is within acceptable bounds.
 */
boolean temperature_in_range(float temp)
{
  // Only accept the reading if it's within a desired range.
  float minimumTemp = -40.0;
  float maximumTemp = 125.0;

  return temp > minimumTemp && temp < maximumTemp;
}


void setup_external_antenna()
{
    STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));
    if (PUBLISH)
        Particle.publish("ANT_EXTERNAL SET", NULL, PRIVATE); // publish to cloud
}
