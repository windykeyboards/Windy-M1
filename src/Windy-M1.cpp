/* ------------------------------------------------------------------------------------
 *                                Windy M1 Firmware
 * ------------------------------------------------------------------------------------
 */

#define FW_VERSION "0.69"

#include <Arduino.h>
#include <Commander.h>
#include <ButtonHandler.h>
#include <Thoroughfare.h>
#include <SD.h>
#include <Macrun.h>
#include <Bounce2.h>

// B U T T O N S
#define SERIAL_BAUD 115200
#define BUTTON_PINS {2, 3, 5, 6, 7, 8}
#define BUTTON_MODES {STANDARD, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED}

ButtonConfig buttonConfig = (ButtonConfig){BUTTON_PINS, BUTTON_MODES};
//ButtonHandler handler(buttonConfig);
Macrun runner;

Bounce debouncer = Bounce(); 

// C O M M A N D S
#define COMMAND_ID "!!"
#define DEVICE_ID "windy_m1"

Commander commander(Serial, COMMAND_ID, DEVICE_ID);
Thoroughfare fileparser(Serial);

// Custom functions
void onEvent(ButtonEvent event);
void onVersionCommand();
void onFileIncoming();

void setup() {
  Serial.begin(SERIAL_BAUD);
  SD.begin(10);
  //handler.setListener(onEvent);

  debouncer.attach(2,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer.interval(25); // Use a debounce interval of 25 milliseconds
  
  commander.attach("version", onVersionCommand);
  commander.attach("incoming_file", onFileIncoming);
}

void loop() {
  debouncer.update(); // Update the Bounce instance
   
  if (debouncer.rose()) { 
   onEvent((ButtonEvent) {1, BUTTON_PRESS});
  }

  commander.onLoop();
}

void onEvent(ButtonEvent event) {
  String buttonNumber = String(event.number);
  Serial.println("Button pressed");
  runner.execute(buttonNumber);
}

void onVersionCommand() {
  commander.write(FW_VERSION);
  commander.eot();
}

void onFileIncoming() {
  fileparser.initiateParse();
}
