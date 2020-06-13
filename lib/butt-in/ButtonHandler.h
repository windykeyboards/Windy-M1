/**
    This file, along with the accompanying implementation file contain code to handle button press events
    from simple buttons, where a HI logic level represents a button pressed.
*/

#ifndef ButtonHandler_H
#define ButtonHandler_H

#include <Arduino.h>
#include <avr/pgmspace.h>

// Length of buffer containing current events
#define EVENT_BUFFER_LENGTH 50

// These differ due to the fact that the atomic section of the button press ISR
// for a modifier key contains a call to digitalRead, which takes time
#define NORMAL_DEBOUNCE_INTERVAL_MILLIS 100
#define MODIFIER_DEBOUNCE_INTERVAL_MILLIS 1

#define SUPPRESS_UNUSED(param) (void)param 

#define ATOMIC(param) noInterrupts();param;interrupts();

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

// static function for getting a String describing the enum constant
const static String getEventTypeName(ButtonEventType type) 
{
    SUPPRESS_UNUSED(getEventTypeName);

    switch (type) {
        case BUTTON_DOWN:
            return "BUTTON_DOWN";
        case BUTTON_UP:
            return "BUTTON_UP";
        case BUTTON_PRESS:
            return "BUTTON_PRESS";
        case UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

// Global function pointer typedefs
typedef void (*Isr)(void);
typedef void (*EventListener)(ButtonEvent event);

class ButtonHandler 
{

    public:
        // Constructor
        ButtonHandler(ButtonConfig config);

        // Public reference to current config
        ButtonConfig _config;

        // For setting pointer to listener
        void setListener(EventListener listener);

        void onPress(int buttonNumber);

        void onModifier(int buttonNumber);

        // For dispatching queued events
        void onLoop();

    private:  
        // Buffer of events, maintains a queue-like implementation when items are added or removed
        ButtonEvent _events[EVENT_BUFFER_LENGTH];        
        // For queue implementation
        int _lastIndex = 0; 

        // listener for new events to be emitted to
        EventListener _listener;

        // Does as the name implies
        boolean isEventsEmpty();
        
        // Shifts item indices in the buffer such that the first item is the current action
        void resizeEvents();

        // Adds event to queue
        void addEvent(ButtonEvent event);

        // Returns pointer to ISR function
        Isr getISR(int buttonIndex);

        // Used for debouncing interrupts
        elapsedMillis _buttonTimeout[ButtonConfig::MAX_BUTTONS];

        bool _modifierActive = false;
};

// Extern as defined globally in c++ implementation file
extern ButtonHandler * _buttonHandler;

// Template function called for each interrupt 
template<int N>
void handleButton() 
{
    _buttonHandler->onPress(N+1);    
}   

// Exploiting the fact that the last ISR set is the one called
template<int N>
void handleModifier() 
{
    _buttonHandler->onModifier(N+1);    
}   

// The following two structs are used to recursively construct and attach interrupt handlers
template<int N> struct InterruptHelper 
{
    static void attach(ButtonConfig config) 
    {
        if (config.buttonModes[N] != DISCONNECTED) 
        {
            pinMode(config.buttonPins[N], INPUT);

            attachInterrupt(digitalPinToInterrupt(config.buttonPins[N]), handleButton<N>, RISING);

            if (config.buttonModes[N] == MODIFIER) 
            {
                attachInterrupt(digitalPinToInterrupt(config.buttonPins[N]), handleModifier<N>, FALLING);
            }
        }

        InterruptHelper<N-1>::attach(config);        
    }
};

template<> struct InterruptHelper<0> 
{
    static void attach(ButtonConfig config) 
    {
        if (config.buttonModes[0] != DISCONNECTED) 
        {
            pinMode(config.buttonPins[0], INPUT);
            
            attachInterrupt(digitalPinToInterrupt(config.buttonPins[0]), handleButton<0>, RISING);

            if (config.buttonModes[0] == MODIFIER) {
                attachInterrupt(digitalPinToInterrupt(config.buttonPins[0]), handleModifier<0>, FALLING);
            }
        }
    }
};

#endif