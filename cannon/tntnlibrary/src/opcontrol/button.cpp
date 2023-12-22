#include "../tntnlibrary/include/opcontrol/button.h"

namespace tntnlib
{
    Button::Button()
    {
        state = false;
        lastState = false;
    }
    bool Button::newPress()
    {
        if (state && !lastState)
        {
            lastState = state;
            return true;
        }
        lastState = state;
        return false;
    }

    void Button::update(bool state)
    {
        lastState = this->state;
        this->state = state;
    }

} // namespace tntnlib
