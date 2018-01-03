//
// Created by rdelfin on 7/5/17.
//

#ifndef BOIDS_GAME_HPP
#define BOIDS_GAME_HPP

#include <maguey/skybox.hpp>
#include <maguey/game_time.hpp>
#include <maguey/joystick_input.hpp>
#include <maguey/key_input.hpp>
#include <maguey/mouse_input.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Game {
public:
    Game(const std::string& windowName, glm::ivec2 window_size = glm::ivec2(800, 600), bool fullscreen = false);

    void run();

    virtual ~Game();

protected:
    virtual void update(GameTime& gameTime) = 0;

    virtual void draw() = 0;

    void addJoystick(unsigned int index);

    const KeyInput& key();
    const MouseInput& mouse();
    const JoystickInput& joystick(unsigned int index);
    GLFWwindow* getWindow();
    glm::ivec2 getWindowSize();

private:
    void updateLoop();

    GLFWwindow* window;
    KeyInput* keyInput;
    MouseInput* mouseInput;
    std::unordered_map<unsigned int, JoystickInput> joysticks;
    glm::ivec2 window_size;

    GameTime gameTime;
};


#endif //BOIDS_GAME_HPP
