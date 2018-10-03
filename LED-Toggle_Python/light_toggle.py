#!/usr/bin/python

# Simple python script that connects to a Particle Photon, reads
# a variable to determine if the onboard LED is on or off and
# toggles it the other way.

__author__ = 'Richard J. Sears'
VERSION = "V1 (2018-10-03)"
# richard@sears.net


import requests
import json

# Replace the device_id and the access_token with the information for your photon
device_id = 'Photon_01'
access_token = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'

def get_led_status():
    global json
    led_status = requests.get('https://api.particle.io/v1/devices/{}/led_status?access_token={}'.format(device_id, access_token))
    json = json.loads(led_status.text)
    photon_led_status = json.get('result')
    return photon_led_status

def toggle_led():
    photon_led_status = get_led_status()
    if photon_led_status == "on":
        r = requests.post('https://api.particle.io/v1/devices/%s/led' % device_id, data={'args': "off", 'access_token':access_token})
    else:
        r = requests.post('https://api.particle.io/v1/devices/%s/led' % device_id, data={'args': "on", 'access_token':access_token})


def main():
    toggle_led()

if __name__=='__main__':
    main()
