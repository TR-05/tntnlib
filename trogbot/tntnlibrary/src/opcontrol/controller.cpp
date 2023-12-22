#include "../tntnlibrary/include/opcontrol/controller.h"
namespace tntnlib
{
    Button r1, r2, l1, l2, up, down, left, right, x, y, a, b;

    void updateButtons()
    {
        r1.update(Controller.ButtonR1.pressing());
        r2.update(Controller.ButtonR2.pressing());
        l1.update(Controller.ButtonL1.pressing());
        l2.update(Controller.ButtonL2.pressing());
        up.update(Controller.ButtonUp.pressing());
        down.update(Controller.ButtonDown.pressing());
        left.update(Controller.ButtonLeft.pressing());
        right.update(Controller.ButtonRight.pressing());
        x.update(Controller.ButtonX.pressing());
        y.update(Controller.ButtonY.pressing());
        a.update(Controller.ButtonA.pressing());
        b.update(Controller.ButtonB.pressing());
    }
} // namespace tntnlib
