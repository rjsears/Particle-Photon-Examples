# Particle & InfluxDB Connected DS18B20 Temperature Sensor

Simple sketch that reads the temperature from a DS18B20 temp sensor and (optionally) outputs serially, 
to the Particle cloud/console via Particle.publish(), as a published particle variable via Particle.variable(),
and to an Influx database for graphing with Grafana or similar. 

# Requirements
- OneWire library	 - http://www.pjrc.com/teensy/td_libs_OneWire.html
- DallasTemperature	library - http://download.milesburton.com/Arduino/MaximTemperature
- DS18B20 Temperature Sensor
- Particle Photon
- Free Particle Account and API Access Code

# Installation
- Copy and paste the sketch into the particle builder site and upload it to your particle
- Connect your DS18B20 to your Particle:
> Data wire is plugged into port D4 on the Photon
> Ground is plugged into GND
> Red is plugged into 3v3
- Have Fun!

<br><br>
![alt tag](https://github.com/rjsears/Particle-Photon-Examples/blob/master/LED-Toggle_Python/Node-Red_Photon.png?raw=true)

<br><br>
## Author

* **Richard J. Sears** - *richard@sears.net* - [The RS Technical Group, Inc.](http://github.com/rjsears)

## License

This project is licensed under the MIT License - see the MIT License for details

## Acknowledgments
* Particle for making a cool device to play with.....
* My amazing family that puts up with all of my coding projects!
