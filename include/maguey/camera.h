//
// Created by rdelfin on 12/7/16.
//

#ifndef PROCEDURALCITIES_CAMERA_H
#define PROCEDURALCITIES_CAMERA_H

#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
    Camera();   // Used to allow constructors to create a window and then set this.
    Camera(GLFWwindow* window);

    void reposition(glm::vec3 pos, glm::vec3 dir, glm::vec3 up);

    void yaw(float dir);
    void pitch(float dir);
    void fpsYaw(float dir);
    void fpsPitch(float dir);
    void roll(float dir);
    void pan(glm::vec3 dir);
    void strave(glm::vec3 dir);
    void yPan(glm::vec3 dir);
    void yStrave(glm::vec3 dir);
    void zoom(float dir);

    const glm::mat4& getViewMatrix();
    const glm::vec3& getCameraPosition();

    std::function<const void*()> viewMatrixDataSource();
    std::function<const void*()> positionDataSource();
    std::function<const void*()> projectionMatrixDataSource();

    ~Camera();
private:
    float camera_distance_ = 3.0;
    glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 center_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
    glm::vec3 right_ = glm::vec3(1.0f, 0.0, 0.0f);
    glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);

    glm::mat4 eyeTranslateMat = glm::mat4(1.0f);
    glm::mat4 centerTranslateMat = glm::mat4(1.0f);
    glm::mat4 eyeRotateMat = glm::mat4(1.0f);
    glm::mat4 centerRotateMat = glm::mat4(1.0f);

    glm::vec3 finalEye = glm::vec3(0.0f, 0.0f, camera_distance_);

    glm::mat4 viewMatrix = glm::mat4();

    GLFWwindow* window;
};


#endif //PROCEDURALCITIES_CAMERA_H
