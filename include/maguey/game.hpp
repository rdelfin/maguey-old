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

namespace maguey {


/**
 * A base class to extend to create a basic game with a window. It provides a
 * GameTime object, key input, mouse input, multiple joysticks, and update/draw
 * methods.
 */
class Game {
public:
    /**
     * Main constructor for the game. Provides basic information to create the
     * window.
     *
     * @param window_name What to set as the window name (visible in windowed
     *                    mode).
     * @param window_size The size, in pixels, of the window.
     * @param fullscreen  Whether the window should be fullscreen or not.
     */
    Game(const std::string& window_name, glm::ivec2 window_size = glm::ivec2(800, 600), bool fullscreen = false);

    /**
     * Called to begin running the game and showing the window.
     */
    void run();

    virtual ~Game();

protected:

    /**
     * Called every frame before rendering. All game logic should be run here.
     *
     * @param gameTime A GameTime object with information regarding the
     *                 duration of the game, frame rate, etc.
     */
    virtual void update(GameTime& gameTime) = 0;

    /**
     * Called every frame after the update. All object rendering should happen
     * here.
     */
    virtual void draw() = 0;

    /**
     * Add a new joystick to the list of joysticks to check for.
     *
     * @param index The index of the joystick, in the format `GLFW_JOYSTICK_#`,
     *              where # is the index of the joystick. The upper limit for
     *              this is `GLFW_JOYSTICK_LAST`.
     */
    void addJoystick(unsigned int index);

    /**
     * Returns the KeyInput object associated with this game window.
     */
    const KeyInput& key();

    /**
     * Returns the MouseInput object associated with this game window.
     */
    const MouseInput& mouse();

    /**
     * Returns the JoystickInput object associated with a given index. The
     * `addJoystick()` method must be called before this one with the
     * appropriate index. Otherwise a `invalid_joystick_exception` will be
     * thrown.
     */
    const JoystickInput& joystick(unsigned int index);

    /**
     * Returns the associated GLFWwindow with this game window.
     */
    GLFWwindow* getWindow();

    /**
     * Returns the window size in pixels. Equivalent to using the
     * `glfwGetWindowSize()` method.
     */
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

} // namespace maguey

#endif //BOIDS_GAME_HPP
