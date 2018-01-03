//
// Created by rdelfin on 7/4/17.
//

#ifndef BOIDS_MOUSEINPUT_HPP
#define BOIDS_MOUSEINPUT_HPP

#define TOTAL_MOUSE_BUTTONS (GLFW_MOUSE_BUTTON_8+1)

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace maguey {

class MouseInput {
public:
    MouseInput(GLFWwindow* window);

    void update();
    bool isPressed(int button) const;
    bool justPressed(int button) const;
    bool justReleased(int button) const;

    glm::vec2 cursorPos() const;
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
