/*
 * SimpleSender.cpp
 *
 *  Demonstrates sending IR codes in standard format with address and command
 *  An extended example for sending can be found as SendDemo.
 *
 *  Copyright (C) 2020-2021  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 *  MIT License
 */
#include <Arduino.h>

//#define SEND_PWM_BY_TIMER
//#define USE_NO_SEND_PWM
//#define NO_LED_FEEDBACK_CODE // saves 418 bytes program memory

#define TV_ADDRESS (0x707)
#define TV_SHOW_BAR (0x79)
#define TV_MENU_UP (0x60)
#define TV_MENU_DOWN (0x61)

#define COMMAND_EMPTY (-1)

#include "PinDefinitionsAndMore.h" //Define macros for input and output pin etc.
#include <IRremote.hpp>

int g_command = COMMAND_EMPTY;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    /*
     * The IR library setup. That's all!
     */
    IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin

    Serial.print(F("Ready to send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
	IrSender.sendSamsung(TV_ADDRESS, TV_SHOW_BAR, 1);
}

String g_buffer;
bool g_builtin_value = false;
void loop() {
    /*
     * Print current send values
     */
	if (g_command != COMMAND_EMPTY)
	{
    	Serial.println("Enter Command number (121 for menu)");
		g_command = COMMAND_EMPTY;

	}
	if (Serial.available() > 0 )
	{
		g_buffer += (char)Serial.read();
		digitalWrite(LED_BUILTIN, g_builtin_value);
		g_builtin_value = !g_builtin_value;
	}
	if (g_buffer.endsWith("\r\n"))
	{
		g_command = g_buffer.toInt();
		Serial.print("Sending command ");
		Serial.println(g_command);
		IrSender.sendSamsung(TV_ADDRESS, g_command, 5);
    	delay(10);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
		g_buffer = "";
	}
}