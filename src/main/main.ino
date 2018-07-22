/* ------------------------------------------------------------------------------------
 *                                Windy M1 Firmware
 * ------------------------------------------------------------------------------------
 */

#define FW_VERSION "0.1"

#include <Commander.h>
#include <ButtonHandler.h>

// B U T T O N S
#define SERIAL_BAUD 115200
#define BUTTON_PINS {2, 3, 5, 6, 7, 8}
#define BUTTON_MODES {STANDARD, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED}

ButtonConfig buttonConfig = (ButtonConfig){BUTTON_PINS, BUTTON_MODES};
ButtonHandler handler(buttonConfig);

// C O M M A N D S
#define COMMAND_ID "!!"
#define DEVICE_ID "windy_m1"

Commander commander(Serial, COMMAND_ID, DEVICE_ID);

void setup() {
  Serial.begin(SERIAL_BAUD);
  handler.setListener(onEvent);
  commander.attach("version", onVersionCommand);
}

void loop() {
  handler.onLoop();
  commander.onLoop();
}

void onEvent(ButtonEvent event) {
  Serial.println("Event: button number: " + String(event.number) + "; Event type: " + getEventTypeName(event.pressType));
}

void onVersionCommand() {
  commander.write(FW_VERSION);
  commander.eot();
}

