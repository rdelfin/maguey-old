//
// Created by rdelfin on 9/17/17.
//

#include <maguey/joystick_input.hpp>

#include <string>
#include <tgmath.h>

#define sign(x) (x < 0 ? -1.0f : (x > 0 ? 1.0f : 0.0f))

namespace maguey {

JoystickInput::JoystickInput() { }

JoystickInput::JoystickInput(unsigned int index) : index(index) {
    if(index > GLFW_JOYSTICK_LAST)
        throw invalid_joystick_exception(index);
}

bool JoystickInput::present() const {
    return (bool)glfwJoystickPresent(index);
}


float JoystickInput::getAxis(unsigned int axis_index, float dead_centre, bool shift_zero) const {
    if(axis_index >= (unsigned int) currAxes.size())
        throw invalid_axis_exception(axis_index, index, (unsigned int) currAxes.size());

    float val = shift_zero ? (currAxes[axis_index] + 1.0f) / 2.0f : currAxes[axis_index];
    return fabs(val) < dead_centre ? 0.0f : sign(val)*(1.0f/(float)pow((1-dead_centre), 2) * (float)pow(fabs(val)-dead_centre, 2));
}

float JoystickInput::getAxisDelta(unsigned int axis_index) const {
    if(axis_index >= currAxes.size())
        throw invalid_axis_exception(axis_index, index, (unsigned int) currAxes.size());

    return currAxes[axis_index] - prevAxes[axis_index];
}

bool JoystickInput::isButtonPressed(unsigned int button_index) const {
    if(button_index >= (unsigned int) currButtons.size())
        throw invalid_button_exception(button_index, index, (unsigned int) currButtons.size());

    return currButtons[button_index];
}


bool JoystickInput::justButtonPressed(unsigned int button_index) const {
    if(button_index >= currButtons.size())
        throw invalid_button_exception(button_index, index, (unsigned int) currButtons.size());

    return currButtons[button_index] && !prevButtons[button_index];
}

bool JoystickInput::justButtonReseased(unsigned int button_index) const {
    if(button_index >= currButtons.size())
        throw invalid_button_exception(button_index, index, (unsigned int) currButtons.size());

    return !currButtons[button_index] && prevButtons[button_index];
}

std::string JoystickInput::getJoystickName() const {
    return glfwGetJoystickName(index);
}

size_t JoystickInput::getButtonCount() const {
    return currButtons.size();
}

size_t JoystickInput::getAxisCount() const {
    return currAxes.size();
}

void JoystickInput::update() {
    int axesCount, buttonCount;
    const float* axes = glfwGetJoystickAxes(index, &axesCount);
    const unsigned char* buttons = glfwGetJoystickButtons(index, &buttonCount);

    if(prevButtons.size() != (unsigned int) buttonCount)
        currButtons.assign(buttons, buttons + buttonCount);

    if(prevAxes.size() != (unsigned int) axesCount)
        currAxes.assign(axes, axes + axesCount);

    prevButtons.assign(currButtons.begin(), currButtons.end());
    prevAxes.assign(currAxes.begin(), currAxes.end());

    currButtons.assign(buttons, buttons + buttonCount);
    currAxes.assign(axes, axes + axesCount);
}

JoystickInput::~JoystickInput() {

}

} // namespace maguey