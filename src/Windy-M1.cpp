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

// B U T T O N S
#define SERIAL_BAUD 115200
#define BUTTON_PINS {2, 3, 5, 6, 7, 8}
#define BUTTON_MODES {STANDARD, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED}

ButtonConfig buttonConfig = (ButtonConfig){BUTTON_PINS, BUTTON_MODES};
ButtonHandler buttonHandler(buttonConfig);
Macrun runner;

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

  buttonHandler.start(onEvent);

  commander.attach("version", onVersionCommand);
  commander.attach("incoming_file", onFileIncoming);
}

void loop() {
  buttonHandler.onLoop();
  commander.onLoop();
}

void onEvent(ButtonEvent event) {
  String buttonNumber = String(event.number);
  runner.execute(buttonNumber);
}

void onVersionCommand() {
  commander.write(FW_VERSION);
  commander.eot();
}

void onFileIncoming() {
  fileparser.initiateParse();
}
