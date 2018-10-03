// -------------------------------------------------------
// Controlling LEDs over the Internet via a Python Script
// -------------------------------------------------------
// Richard J. Sears (richard@sears.net)
// October 3rd, 2018
//
// Simple sketch to interact with a python script to toggle 
// the LED on and off based on on/off variable.
// This also works with a node-red flow that I built as well.

// This is based on the web-connected-led sketch provided by Particle.io - 
// All that I have done is added the variable led_status and then I set the
// variable based on the LED being on or off. Thy python scripts reads the 
// variable and then reacts.



// Identify the onboard LED
int led2 = D7;

//setup for our led_status variable used to toggle led via pythin scripts
String led_status;

// Set our antenna to external for better range (assuming you have the external antenna)
STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));

// Declare pings and setup our ledToggle Function
void setup()
{

   // Here's the pin configuration, set D7 (led2) to output mode:
   pinMode(led2, OUTPUT);

   // We are also going to declare a Particle.function so that we can turn the LED on and off.
   Particle.function("led",ledToggle);

   // Let's also make sure the LEDs are off when we start:
   digitalWrite(led2, LOW);
   
   // Here is where we setup a Variable to track the condition of the LED:
   Particle.variable("led_status", led_status);
   

}

// Since we are calling a function, we do not need to do anything in our "loop":

void loop()
{
   // Nothing to do here
}

// This is the functin that gets called via the particle api to toggle the LED:

int ledToggle(String command) {
    /* Particle.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    It also sets our variable that we can then query so we know if the led is on or off.
    */

    if (command=="on") {
        digitalWrite(led2,HIGH);
        led_status = "on";
        return 1;
    }
    else if (command=="off") {
        digitalWrite(led2,LOW);
        led_status = "off";
        return 0;
    }
    else {
        return -1;
    }
}
