# LED Toggle Python

Simple set of scripts to toggle the LED on a Particle Photon via a Node-Red flow or simple pything script. 

# Requirements
- Particle [Node](https://www.npmjs.com/package/node-red-contrib-particle) for Node-RED if you are going to use the NR flow.
- Requests and Json for Python if you are going to use Python script
- Particle Photon
- Free Particle Account and API Access Code

# Installation
- Copy and paste the sketch into the particle builder site and upload it to your particle
- For Node-RED: Import the flow into NR and edit your account information (device_id and access_token)
- For Pythin: Copy script to your local system, update device_id and access_token variables
- Have Fun!

The sketch is based off the web-connected-led.ino provided by Particle with minor modifications to include
the variable information for the toggle function. In the Node-RED flow you can either toggle the LED, this goes
out to the Photon and get the variable telling us if the led is on or off and then toggles it the other way, or you can simply turn it on or off.
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
