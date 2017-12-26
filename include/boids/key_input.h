//
// Created by rdelfin on 7/2/17.
//

#ifndef BOIDS_KEYINPUT_HPP
#define BOIDS_KEYINPUT_HPP

#include <GLFW/glfw3.h>

#define TOTAL_KEYS (GLFW_KEY_LAST+1)

class KeyInput {
public:
    KeyInput(GLFWwindow* window);

    void update();
    bool isPressed(int key) const;
    bool justPressed(int key) const;
    bool justReleased(int key) const;

    ~KeyInput();
private:
    static bool validKey(int key);

    bool keyStateNow[TOTAL_KEYS];
    bool keyStatePrev[TOTAL_KEYS];
};

#endif //BOIDS_KEYINPUT_HPP
