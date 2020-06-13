#include <ButtonHandler.h>

ButtonHandler * _buttonHandler;

ButtonHandler::ButtonHandler(ButtonConfig config) : _config(config)
{
    // Global reference needed for global ISR pointer
    _buttonHandler = this;
    InterruptHelper<ButtonConfig::MAX_BUTTONS - 1>::attach(_config);
}

// Setting the callback function used to emit events to
void ButtonHandler::setListener(EventListener listener) 
{
    _listener = listener;    
}

// Loop passthrough to handle event dispatching of queued items
void ButtonHandler::onLoop()
{
    if (!isEventsEmpty()) {
        _listener(_events[0]);

        ATOMIC(
            resizeEvents();
        )
    }
}

// Returns whether or not the event queue is empty
boolean ButtonHandler::isEventsEmpty() 
{
    return _lastIndex == 0;
}

// Shifts event array down an indices such that index 1 -> index 0 etc.
void ButtonHandler::resizeEvents() 
{
    for (int i = 0; i < _lastIndex; i++) {
        _events[i] = _events[i+1];
    }

    _lastIndex--;
}

// Adds event to the queue if there is room in the queue.
// If the consumer of the library is not processing an event, and the 
// events queue is empty then simply emit the event, and queue others
void ButtonHandler::addEvent(ButtonEvent event) {
    if (_lastIndex < EVENT_BUFFER_LENGTH) {
        // Add event to buffer and increment last index
        _events[_lastIndex] = event;            
        _lastIndex++;
    }
}

// Handles button press event. Simply adds event to queue
void ButtonHandler::onPress(int number) 
{
    if (_buttonTimeout[number-1] > NORMAL_DEBOUNCE_INTERVAL_MILLIS) {
        ATOMIC(
            addEvent((ButtonEvent) {number, BUTTON_PRESS});
        )

        Serial.println("Button pressed");

        _buttonTimeout[number-1] = 0;
    }
}

// Handles modifier event. Determines if the modifier has been pressed or released, then 
// adds the event to the queue
void ButtonHandler::onModifier(int number)
{
     if (_buttonTimeout[number-1] > MODIFIER_DEBOUNCE_INTERVAL_MILLIS) {
        ATOMIC(
            delay(1); // Delay is required to allow an accurate pin read
            _modifierActive = digitalRead(_config.buttonPins[number-1]);

            addEvent((ButtonEvent) {number, _modifierActive ? BUTTON_DOWN : BUTTON_UP});
        )

        _buttonTimeout[number-1] = 0;
    }
}

