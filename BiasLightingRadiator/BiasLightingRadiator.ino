/// 
/// @mainpage	BiasLightingRadiator 
///
/// @details	Uses IR and Serial read to drive a bias lighting
/// @n 		
/// @n 
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		Marco Aurelio Valtas Cunha
/// @author		___FULLUSERNAME___
/// @date		4/21/14 12:23 PM
/// @version	<#version#>
/// 
/// @copyright	(c) Marco Aurelio Valtas Cunha, 2014
/// @copyright	GNU General Public License
///
/// @see		ReadMe.txt for references
///


///
/// @file		BiasLightingRadiator.ino
/// @brief		Main sketch
///
/// @details	<#details#>
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		Marco Aurelio Valtas Cunha
/// @author		___FULLUSERNAME___
/// @date		4/21/14 12:23 PM
/// @version	<#version#>
/// 
/// @copyright	(c) Marco Aurelio Valtas Cunha, 2014
/// @copyright	GNU General Public License
///
/// @see		ReadMe.txt for references
/// @n
///


// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430 G2 and F5529, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#include "IRremote.h"
#else // error
#error Platform not defined
#endif


int BUITIN_LED = 13; // Arduino's builtin led

// IR remote commands for RBG led strip which are IR NEC format
// format: 0x00ff[command][command ^ 0xff]
// see: http://blog.allgaiershops.com/2012/05/10/reversing-an-rgb-led-remote/
// see: http://www.instructables.com/id/Reverse-Engineering-RGB-LED-Bulb-with-IR-remote/?ALLSTEPS
// see: http://www.sbprojects.com/knowledge/ir/nec.php
unsigned long IR_RED    = 0x00ff1ae5; // command for red color
unsigned long IR_GREEN  = 0x00ff9a65; // command for green color
unsigned long IR_YELLOW = 0x00ff2ad5; // command for yellow color

// IR send interface, uses pin 3 on Arduino
// see: http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html
IRsend irsend;

// hold the current state, arduino will keep issuing this
// command on a loop
unsigned long IR_CURRENT_STATE;

void setup() 
{
    Serial.begin(9600);
    pinMode(BUITIN_LED, OUTPUT);
    IR_CURRENT_STATE = IR_GREEN; // start green
}

void blinkBuiltInLed() {
    digitalWrite(BUITIN_LED, HIGH);
    delay(100);
    digitalWrite(BUITIN_LED, LOW);
}

unsigned long readCurrentState(char command) {
    switch (command) {
        case 'r':
            return IR_RED;
            break;
        case 'g':
            return IR_GREEN;
            break;
        case 'y':
            return IR_YELLOW;
            break;
        default:
            return IR_CURRENT_STATE;
    }
}

void sendIRCommand(unsigned long ircommand) {
    irsend.sendNEC(ircommand, 32);
    blinkBuiltInLed();
}

void loop()
{
    // read commands from serial 'r', 'g' or 'y'
    if(Serial.available() > 0) {
        char command[1];
        Serial.readBytesUntil('\n', command, 1);
        IR_CURRENT_STATE = readCurrentState(command[0]);
    }
    sendIRCommand(IR_CURRENT_STATE);
    delay(1000);
}