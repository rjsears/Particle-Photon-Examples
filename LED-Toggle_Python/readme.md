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
the variable information for the toggle function.

