/**
    This file, along with the accompanying implementation file contain code to handle button press events
    from simple buttons, where a HI logic level represents a button pressed.
*/

#ifndef ButtonHandler_H
#define ButtonHandler_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Bounce2.h>

// Defining event types for button presses, where up correspond to hold events
PROGMEM enum ButtonEventType 
{   
    UNKNOWN,
    BUTTON_DOWN,
    BUTTON_UP,
    BUTTON_PRESS
};

// Defines modes for which buttons can be initialised to. STANDARD only considers single press
// events, whereas MODIFIER considers both down, and up press action
//
// DISCONNECTED used for debugging to avoid unwanted events
PROGMEM enum ButtonMode
{
    STANDARD,
    MODIFIER,
    DISCONNECTED
};

// Struct for maintaining information regarding button events
struct ButtonEvent 
{
    int number;
    ButtonEventType pressType;
};

// Struct holding config information, i.e button pins and modes
struct ButtonConfig
{
    // Defines maximum amount of buttons able to be used, which is required
    // due to the use of fixed ISRs for each button event. NOTE: this number
    // is limited by the size of the vector containing pointers to ISRs, usually 8
    static const PROGMEM int MAX_BUTTONS = 6;

    int buttonPins[MAX_BUTTONS];
    ButtonMode buttonModes[MAX_BUTTONS];
};

// Global function pointer typedefs
typedef void (*EventListener)(ButtonEvent event);

class ButtonHandler 
{

    public:
        // Constructor
        ButtonHandler(ButtonConfig config);

        // Public reference to current config
        ButtonConfig _config;

        // Called in the setup function
        void start(EventListener listener);

        // Called in loop function
        void onLoop();

    private:  
        // Count of active buttons
        int _activeButtonCount = 0;
        int _activeButtonConfigIndex[ButtonConfig::MAX_BUTTONS];

        // Buffer of debouncers for each key
        Bounce _debouncer[ButtonConfig::MAX_BUTTONS];        

        // listener for new events to be emitted to
        EventListener _listener;
};

#endif