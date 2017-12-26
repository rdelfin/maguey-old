//
// Created by rdelfin on 7/2/17.
//

#define GLM_FORCE_RADIANS

#include <boids/key_input.h>

bool KEY_STATES[TOTAL_KEYS];
static bool KEYS_BINDED = false;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

KeyInput::KeyInput(GLFWwindow* window) {
    if(!KEYS_BINDED) {
        KEYS_BINDED = true;
        glfwSetKeyCallback(window, KeyCallback);
    }

    for(int i = 0; i < TOTAL_KEYS; i++)
        keyStatePrev[i] = keyStateNow[i] = KEY_STATES[i];
}

void KeyInput::update() {
    for(int i = 0; i < TOTAL_KEYS; i++) {
        keyStatePrev[i] = keyStateNow[i];
        keyStateNow[i] = KEY_STATES[i];
    }
}

bool KeyInput::isPressed(int key) const {
    return validKey(key) && keyStateNow[key];
}

bool KeyInput::justPressed(int key) const {
    return validKey(key) && keyStateNow[key] && !keyStatePrev[key];
}

bool KeyInput::justReleased(int key) const {
    return validKey(key) && !keyStateNow[key] && keyStatePrev[key];
}

bool KeyInput::validKey(int key) {
    return key >= 0 && key < TOTAL_KEYS;
}

KeyInput::~KeyInput() {

}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS)
        KEY_STATES[key] = true;
    if(action == GLFW_RELEASE)
        KEY_STATES[key] = false;

}
