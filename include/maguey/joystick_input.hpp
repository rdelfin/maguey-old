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

namespace maguey {

/**
 * Exception thrown when the joystick provided is past the maximum allowed
 * index.
 */
class invalid_joystick_exception : public std::exception {
public:
    invalid_joystick_exception(unsigned int index) : index(index) { }
    virtual const char* what() const throw() {
        return (std::string("Controller with index ") + std::to_string(index) +
                std::string(" is past max controller index ") + std::to_string(GLFW_JOYSTICK_LAST)).c_str();
    }

    unsigned int index;
};

/**
 * Exception thrown when the axis index is past the last axis index.
 */
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

/**
 * Exception thrown when the button index is past the last button index.
 */
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

/**
 * Class representing input for a single joystick. Each one provides interfaces
 * for either current button and joystick state, or diffs between the last and
 * current frames.
 *
 * Example:
 * ```
 * JoystickInput j1(GLFW_JOYSTICK_1);
 * std::cout << "Joystick 1 is type: " << j1.getJoystickName() << std::endl;
 *
 * // frame update method
 * void update(GameTime& gameTime) {
 *     j1.update();
 *
 *     // To get axis 0 with a dead center of 0.1
 *     std::cout << "J1 axis 1: " << j1.getAxis(0, 0.1) << std::endl;
 * }
 * ```
 */
class JoystickInput {
public:
    /**
     * Main constructor. Creates a new joystick object at a given index.
     *
     * @param index The index of the joystick, in the format `GLFW_JOYSTICK_#`,
     *              where # is the index of the joystick. The upper limit for
     *              this is `GLFW_JOYSTICK_LAST`.
     */
    JoystickInput(unsigned int index = GLFW_JOYSTICK_1);

    /**
     * Gets the position of a joystick axis, between -1 and 1, where 0 is the
     * center. A dead centre can be provided, and the values can also be
     * shifted to go from 0 to 1.
     *
     * @param axis_index  The index of the axis to get the value of. The range
     *                    is 0 <= axis_index <= getAxisCount(), where
     *                    `getAxisCount()` is the method in this class.
     * @param dead_centre Get axis will return 0 for all values where abs(x)
     *                    <= dead_centre, and scale quadratically from 0 to 1
     *                    between dead_centre and 1 (similarly for negative
     *                    values). This is since controllers tend to rest at
     *                    non-zero values.
     * @param shift_zero  It shifts raw values from the range [-1, 1] to
     *                    [0, 1]. It does this before applying the dead centre.
     */
    float getAxis(unsigned int axis_index, float dead_centre, bool shift_zero = false) const;

    /**
     * Gets the difference in raw axis values between this and the last frame.
     *
     * @param axis_index The index of the axis to get the value of. The range
     *                   is 0 <= axis_index <= getAxisCount(), where
     *                   `getAxisCount()` is the method in this class.
     */
    float getAxisDelta(unsigned int axis_index) const;

    /**
     * Returns whether a given button is pressed or not in this frame.
     *
     * @param button_index Index of the button being pressed. Indices take on
     *                     the range 0 <= button_index <= getAxisCount(), where
     *                     `getAxisCount()` is the method in the class.
     */
    bool isButtonPressed(unsigned int button_index) const;

    /**
     * Returns whether a given button is pressed in this frame and not in the
     * last frame.
     *
     * @param button_index Index of the button being pressed. Indices take on
     *                     the range 0 <= button_index <= getAxisCount(), where
     *                     `getAxisCount()` is the method in the class.
     */
    bool justButtonPressed(unsigned int button_index) const;

    /**
     * Returns whether a given button is not pressed in this frame and it is in
     * the last frame.
     *
     * @param button_index Index of the button being pressed. Indices take on
     *                     the range 0 <= button_index <= getAxisCount(), where
     *                     `getAxisCount()` is the method in the class.
     */
    bool justButtonReseased(unsigned int button_index) const;

    /**
     * A string representing the name of this joystick.
     */
    std::string getJoystickName() const;

    /**
     * Gets the number of buttons on the controller.
     */
    size_t getButtonCount() const;

    /**
     * Gets the number of axis on the controller.
     */
    size_t getAxisCount() const;

    /**
     * Returns whether the controller at the joystick index is connected or
     * not.
     */
    bool present() const;

    /**
     * Updates the values of each axis and button state. Should be called at or
     * near the top of the frame update function.
     */
    void update();

    ~JoystickInput();
private:
    unsigned int index;
    std::vector<bool> prevButtons, currButtons;
    std::vector<float> prevAxes, currAxes;
};

} // namespace maguey

#endif //BOIDS_JOYSTICK_INPUT_H
