//
// Created by rdelfin on 7/4/17.
//

#ifndef BOIDS_MOUSEINPUT_HPP
#define BOIDS_MOUSEINPUT_HPP

#define TOTAL_MOUSE_BUTTONS (GLFW_MOUSE_BUTTON_8+1)

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace maguey {

/**
 * Represents input from the computer's mouse. The class contains information
 * on each button's current frame state, the last frame's state, as well as the
 * mouse's current and change in position.
 */
class MouseInput {
public:
    /**
     * Main constructor for the mouse input. Initializes the button and
     * position states.
     *
     * @param window The GLFW Window associated with this mouse input. It is
     *               used to set up the mouse button and position callback.
     */
    MouseInput(GLFWwindow* window);

    /**
     * Updates the current and previous frame mouse states, including buttons
     * and the mouse position.
     */
    void update();

    /**
     * Returns whether a given mouse button is pressed or not. Returns false if
     * the button is an invalid code.
     *
     * @param button The button code for the mouse button being checked. For
     *               this, use the GLFW key codes. These include:
     *               GLFW_MOUSE_BUTTON_# (where # is an integer where
     *               1 <= # <= 8), GLFW_MOUSE_BUTTON_LAST,
     *               GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_MIDDLE and
     *               GLFW_MOUSE_BUTTON_RIGHT
     */
    bool isPressed(int button) const;

    /**
     * Returns whether a given mouse button is pressed in this frame and not in
     * the last frame. Returns false if the button is an invalid code.
     *
     * @param button The button code for the mouse button being checked. For
     *               this, use the GLFW key codes. These include:
     *               GLFW_MOUSE_BUTTON_# (where # is an integer where
     *               1 <= # <= 8), GLFW_MOUSE_BUTTON_LAST,
     *               GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_MIDDLE and
     *               GLFW_MOUSE_BUTTON_RIGHT
     */
    bool justPressed(int button) const;

    /**
     * Returns whether a given mouse button is not pressed in this frame and
     * was pressed in the last frame. Returns false if the button is an invalid
     * code.
     *
     * @param button The button code for the mouse button being checked. For
     *               this, use the GLFW key codes. These include:
     *               GLFW_MOUSE_BUTTON_# (where # is an integer where
     *               1 <= # <= 8), GLFW_MOUSE_BUTTON_LAST,
     *               GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_MIDDLE and
     *               GLFW_MOUSE_BUTTON_RIGHT
     */
    bool justReleased(int button) const;

    /**
     * Returns the position of the mouse, normalized to values (x,y) in the
     * range [0, 1]. These can be returned to screen coordinates by using the
     * glfwGetWindowSize() function, or the getWindowSize() method in the
     * maguey::Game class and multiplying the two vectors.
     */
    glm::vec2 cursorPos() const;

    /**
     * Returns the difference in the position of the mouse between the last and
     * current frames, normalized to values in the range [0, 1]. These can be
     * returned to screen coordinates by using the glfwGetWindowSize()
     * function, or the getWindowSize() method in the maguey::Game class and
     * multiplying the two vectors.
     */
    glm::vec2 cursorDelta() const;

    ~MouseInput();
private:
    static bool validButton(int button);

    bool buttonStateNow[TOTAL_MOUSE_BUTTONS];
    bool buttonStatePrev[TOTAL_MOUSE_BUTTONS];

    glm::vec2 currCursor, prevCursor;
};

} // namespace maguey


#endif //BOIDS_MOUSEINPUT_HPP
