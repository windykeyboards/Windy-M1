#include <ButtonHandler.h>
#include <Bounce2.h>

ButtonHandler::ButtonHandler(ButtonConfig config) : _config(config) {
    for (unsigned int i = 0; i < sizeof(config.buttonModes)/sizeof(config.buttonModes[0]); i++) {
        if (config.buttonModes[i] != ButtonMode::DISCONNECTED) {
            _activeButtonConfigIndex[_activeButtonCount] = i;
            _activeButtonCount++;
        }
    }
}

// Setting the callback function used to emit events to
void ButtonHandler::start(EventListener listener) 
{
    _listener = listener;    

    // Attach debouncers
    for (int i = 0; i < _activeButtonCount; i++) {
        _debouncer[i] = Bounce();
        _debouncer[i].attach(_config.buttonPins[_activeButtonConfigIndex[i]], INPUT_PULLUP);
        _debouncer[i].interval(25);
    }
}

// Loop passthrough to handle event dispatching of queued items
void ButtonHandler::onLoop()
{
    for (int i = 0; i < _activeButtonCount; i++) {
        _debouncer[i].update();

        if (_debouncer[i].rose()) {
            // Button down, press
            _listener((ButtonEvent){ /* one-indexed buttons */ i + 1, BUTTON_PRESS });
        }

        if (_debouncer[i].fell()) {
            // Button up - TODO: Handle this for modifiers
        }
    }
}