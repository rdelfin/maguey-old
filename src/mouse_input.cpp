//
// Created by rdelfin on 7/4/17.
//

#define GLM_FORCE_RADIANS

#include <maguey/mouse_input.hpp>

void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

glm::vec2 CURSOR_POSITION;
bool BUTTON_STATES[TOTAL_MOUSE_BUTTONS];
static bool MOUSE_BINDED = false;

namespace maguey {

MouseInput::MouseInput(GLFWwindow* window) {
    if(!MOUSE_BINDED) {
        MOUSE_BINDED = true;
        glfwSetCursorPosCallback(window, MousePosCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
    }
    prevCursor = currCursor = CURSOR_POSITION;
}

void MouseInput::update() {
    prevCursor = currCursor;
    currCursor = CURSOR_POSITION;
    for(int i = 0; i < TOTAL_MOUSE_BUTTONS; i++) {
        buttonStatePrev[i] = buttonStateNow[i];
        buttonStateNow[i] = BUTTON_STATES[i];
    }
}

bool MouseInput::isPressed(int button) const {
    return validButton(button) && buttonStateNow[button];
}

bool MouseInput::justPressed(int button) const {
    return validButton(button) && buttonStateNow[button] && !buttonStatePrev[button];
}

bool MouseInput::justReleased(int button) const {
    return validButton(button) && !buttonStateNow[button] && buttonStatePrev[button];
}

glm::vec2 MouseInput::cursorPos() const {
    return currCursor;
}

glm::vec2 MouseInput::cursorDelta() const {
    return currCursor - prevCursor;
}

bool MouseInput::validButton(int button) {
    return button >= 0 && button < TOTAL_MOUSE_BUTTONS;
}

MouseInput::~MouseInput() {

}

void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    CURSOR_POSITION = glm::vec2(mouse_x / width, mouse_y / height);
}


void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_PRESS)
        BUTTON_STATES[button] = true;
    if(action == GLFW_RELEASE)
        BUTTON_STATES[button] = false;
}

} // namespace maguey
