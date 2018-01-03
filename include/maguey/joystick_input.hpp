//
// Created by rdelfin on 9/17/17.
//

#ifndef BOIDS_JOYSTICK_INPUT_H
#define BOIDS_JOYSTICK_INPUT_H

#include <exception>
#include <string>
#include <sstream>
#include <vector>

#include <GLFW/glfw3.h>

class invalid_joystick_exception : public std::exception {
public:
    invalid_joystick_exception(unsigned int index) : index(index) { }
    virtual const char* what() const throw() {
        return (std::string("Controller with index ") + std::to_string(index) +
                std::string(" is past max controller index ") + std::to_string(GLFW_JOYSTICK_LAST)).c_str();
    }

    unsigned int index;
};

class invalid_axis_exception : public std::exception {
public:
    invalid_axis_exception(unsigned int axis, unsigned int index, unsigned int max)
            : index(index), max(max), axis(axis) { }
    virtual const char* what() const throw() {
        return (std::string("Axis ") + std::to_string(axis) + std::string(" on controller ") + std::to_string(index) +
               std::string(" past the max axis ") + std::to_string(max)).c_str();
    }
    unsigned int index, max, axis;
};

class invalid_button_exception : public std::exception {
public:
    invalid_button_exception(unsigned int button, unsigned int index, unsigned int max)
            : index(index), max(max), button(button) { }
    virtual const char* what() const throw() {
        return (std::string("Button ") + std::to_string(button) + std::string(" on controller ") + std::to_string(index) +
               std::string(" is past the max axis ") + std::to_string(max)).c_str();
    }
    unsigned int index, max, button;
};

class JoystickInput {
public:
    JoystickInput();
    JoystickInput(unsigned int index);

    float getAxis(unsigned int axis_index, float dead_centre, bool shift_zero = false) const;
    float getAxisDelta(unsigned int axis_index) const;
    bool isButtonPressed(unsigned int button_index) const;
    bool justButtonPressed(unsigned int button_index) const;
    bool justButtonReseased(unsigned int button_index) const;

    std::string getJoystickName() const;
    size_t getButtonCount() const;
    size_t getAxisCount() const;

    bool present() const;

    void update();

    ~JoystickInput();
private:
    unsigned int index;
    std::vector<bool> prevButtons, currButtons;
    std::vector<float> prevAxes, currAxes;
};


#endif //BOIDS_JOYSTICK_INPUT_H
