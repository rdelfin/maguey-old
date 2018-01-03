//
// Created by rdelfin on 7/5/17.
//

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <maguey/debuggl.h>
#include <maguey/game.hpp>

#include <iostream>
#include <string>

namespace maguey {

void scaleToMonitor(GLFWmonitor* monitor, glm::ivec2& window_size);

Game::Game(const std::string& windowName, glm::ivec2 window_size, bool fullscreen)
    : window_size(window_size) {
    if (!glfwInit()) exit(EXIT_FAILURE);
    //glfwSetErrorCallback(ErrorCallback);

    // Ask an OpenGL 4.0 core profile context
    // It is required on OSX and non-NVIDIA Linux
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = (fullscreen ? glfwGetPrimaryMonitor() : nullptr);

    if(monitor)
        scaleToMonitor(monitor, window_size);

    this->window = glfwCreateWindow(window_size.x, window_size.y,
                                    windowName.data(), monitor, nullptr);
    CHECK_SUCCESS(this->window != nullptr);
    glfwMakeContextCurrent(this->window);
    glewExperimental = GL_TRUE;

    CHECK_SUCCESS(glewInit() == GLEW_OK);
    glGetError();  // clear GLEW's error for it
    glfwSwapInterval(1);

    const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
    const GLubyte* version = glGetString(GL_VERSION);    // version as a string
    std::cout << "Renderer: " << renderer << "\n";
    std::cout << "OpenGL version supported:" << version << "\n";

    keyInput = new KeyInput(window);
    mouseInput = new MouseInput(window);
}

void Game::run() {
    gameTime.reset();
    updateLoop();
}

Game::~Game() {
    delete keyInput;
    delete mouseInput;

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Game::addJoystick(unsigned int index) {
    if(index >= GLFW_JOYSTICK_LAST)
        throw invalid_joystick_exception(index);
    if(joysticks.count(index) == 0)
        joysticks[index] = JoystickInput(index);
}


const KeyInput& Game::key() { return *keyInput; }
const MouseInput& Game::mouse() { return *mouseInput; }
GLFWwindow* Game::getWindow() { return window; }

const JoystickInput& Game::joystick(unsigned int index) {
    if(joysticks.count(index) == 0)
        throw invalid_joystick_exception(index);

    return joysticks[index];
}

glm::ivec2 Game::getWindowSize() {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    return glm::ivec2(w, h);
}

void Game::updateLoop() {
    while (!glfwWindowShouldClose(window)) {
        gameTime.update();
        keyInput->update();
        mouseInput->update();
        for(auto& jInput : joysticks)
            jInput.second.update();

        this->update(gameTime);

        // Setup some basic window stuff.
        glfwGetFramebufferSize(window, &window_size.x, &window_size.y);
        glViewport(0, 0, window_size.x, window_size.y);
        glClearColor(0.390625f, 0.58203125f, 0.92578125f, 1.0f);           // Cornflower blue which, according to Microsoft, is the best background color
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        //glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);

        this->draw();

        // Poll and swap.
        glfwPollEvents();
        glfwSwapBuffers(window);

    }
}

void scaleToMonitor(GLFWmonitor* monitor, glm::ivec2& window_size) {
    const GLFWvidmode * mode = glfwGetVideoMode(monitor);
    glm::ivec2 monitor_size(mode->width, mode->height);

    glm::ivec2 scaled_x(monitor_size.x, window_size.y * monitor_size.x / window_size.x);
    glm::ivec2 scaled_y(window_size.x * monitor_size.y / window_size.y, monitor_size.y);

    if(monitor_size.x < window_size.x && monitor_size.y < window_size.y) {
        if(scaled_x.y <= monitor_size.y)
            window_size = scaled_x;
        else
            window_size = scaled_y;
    }

    else if(monitor_size.x < window_size.x)
        window_size = scaled_x;
    else if(monitor_size.y < window_size.y)
        window_size = scaled_y;
}


} // namespace maguey
