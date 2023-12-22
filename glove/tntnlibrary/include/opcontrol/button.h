#pragma once

namespace tntnlib
{
    class Button
    {
    public:
        Button();
        bool newPress();
        void update(bool state);
        bool state = false;
        bool lastState = false;
    }; // class Button
    extern Button r1, r2, l1, l2, up, down, left, right, x, y, a, b;
} // namespace tntnlib