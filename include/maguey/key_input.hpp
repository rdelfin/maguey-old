//
// Created by rdelfin on 7/2/17.
//

#ifndef BOIDS_KEYINPUT_HPP
#define BOIDS_KEYINPUT_HPP

#include <GLFW/glfw3.h>

#define TOTAL_KEYS (GLFW_KEY_LAST+1)

namespace maguey {

/**
 * Represents input from the computer's keyboard. The class contains
 * information on both each key's current frame state and the last frame's
 * state.
 */
class KeyInput {
public:
    /**
     * Main constructor for the keyboard input. Initializes all key states.
     *
     * @param window The GLFW Window associated with this keyboard input. It is
     *               used to set up the keyboard callback.
     */
    KeyInput(GLFWwindow* window);

    /**
     * Updates the current and previous frame pressed states.
     */
    void update();

    /**
     * Returns whether a given key is pressed or not.
     *
     * @param key The key code for the key being checked. The GLFW key codes
     *            are used for this. For example, for the A key, use GLFW_KEY_A
     *            and for the left control key use GLFW_KEY_LEFT_CONTROL.
     */
    
    bool isPressed(int key) const;

    /**
     * Returns whether a given key is pressed in this frame and not in the last
     * frame.
     *
     * @param key The key code for the key being checked. The GLFW key codes
     *            are used for this. For example, for the A key, use GLFW_KEY_A
     *            and for the left control key use GLFW_KEY_LEFT_CONTROL.
     */
    bool justPressed(int key) const;

    /**
     * Returns whether a given key is not pressed in this frame and was pressed
     * in the last frame.
     *
     * @param key The key code for the key being checked. The GLFW key codes
     *            are used for this. For example, for the A key, use GLFW_KEY_A
     *            and for the left control key use GLFW_KEY_LEFT_CONTROL.
     */
    bool justReleased(int key) const;

    ~KeyInput();
private:
    static bool validKey(int key);

    bool keyStateNow[TOTAL_KEYS];
    bool keyStatePrev[TOTAL_KEYS];
};

} // namespace maguey

#endif //BOIDS_KEYINPUT_HPP
