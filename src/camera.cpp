//
// Created by rdelfin on 12/7/16.
//

#define GLM_FORCE_RADIANS

#include <maguey/camera.hpp>


#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

namespace maguey {

Camera::Camera() { }

Camera::Camera(GLFWwindow* window) : window(window) { }

void Camera::reposition(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) {
    eye_ = pos;
    center_ = pos + dir;
    look_ = glm::normalize(dir);
    right_ = glm::normalize(glm::cross(look_, up));
    up_ = glm::normalize(glm::cross(right_, look_));
    eyeRotateMat = glm::mat4(1.0f);
    centerRotateMat = glm::mat4(1.0f);
    eyeTranslateMat = glm::mat4(1.0f);
    centerRotateMat = glm::mat4(1.0f);
}

void Camera::yaw(float dir) {
    // Rotate around up vector (for yaw)
    eyeRotateMat = glm::rotate(dir, up_) * eyeRotateMat;
    centerRotateMat = glm::rotate(dir, up_) * centerRotateMat;

    glm::vec3 newEye(eyeRotateMat * glm::vec4(eye_, 1));
    // Generate new look, right and up vectors based on new rotation
    look_ = glm::normalize(center_ - newEye);
    right_ = glm::normalize(glm::cross(look_, up_));
    up_ = glm::normalize(glm::cross(right_, look_));
}

void Camera::pitch(float dir) {
    // Rotate around right pointing vector (for pitch)
    eyeRotateMat = glm::rotate(dir, right_) * eyeRotateMat;
    centerRotateMat = glm::rotate(dir, right_) * centerRotateMat;

    glm::vec3 newEye(eyeRotateMat * glm::vec4(eye_, 1));
    // Generate new look, right and up vectors based on new rotation
    look_ = glm::normalize(center_ - newEye);
    right_ = glm::normalize(glm::cross(look_, up_));
    up_ = glm::normalize(glm::cross(right_, look_));
}


void Camera::roll(float dir) {
    glm::mat4 rollRotate = glm::rotate(dir, look_);
    right_ = glm::normalize(glm::vec3(rollRotate * glm::vec4(right_, 0.0f)));
    up_ = glm::normalize(glm::vec3(rollRotate * glm::vec4(up_, 0.0f)));
}

void Camera::fpsPitch(float dir) {
    glm::mat4 pitchRotate = glm::rotate(dir, right_);
    right_ = glm::normalize(glm::vec3(pitchRotate * glm::vec4(right_, 0.0f)));
    look_ = glm::normalize(glm::vec3(pitchRotate * glm::vec4(look_, 0.0f)));
    up_ = glm::normalize(glm::vec3(pitchRotate * glm::vec4(up_, 0.0f)));
    centerRotateMat = pitchRotate * centerRotateMat;
}

void Camera::fpsYaw(float dir) {
    glm::mat4 yawRotate = glm::rotate(dir, up_);
    right_ = glm::normalize(glm::vec3(yawRotate * glm::vec4(right_, 0.0f)));
    look_ = glm::normalize(glm::vec3(yawRotate * glm::vec4(look_, 0.0f)));
    up_ = glm::normalize(glm::vec3(yawRotate * glm::vec4(up_, 0.0f)));
    centerRotateMat = yawRotate * centerRotateMat;
}


void Camera::strave(glm::vec3 dir) {
    centerTranslateMat  = glm::translate(dir.x*right_ + dir.y*up_ + dir.z*look_) * centerTranslateMat;
}

void Camera::pan(glm::vec3 dir) {
    centerTranslateMat  = glm::translate(dir.x*right_ + dir.y*up_ + dir.z*look_) * centerTranslateMat;
    eyeTranslateMat  = glm::translate(dir.x*right_ + dir.y*up_ + dir.z*look_) * eyeTranslateMat;
}


void Camera::yPan(glm::vec3 dir) {
    centerTranslateMat  = glm::translate(glm::vec3(dir.x, dir.y, dir.z)) * centerTranslateMat;
    eyeTranslateMat  = glm::translate(glm::vec3(dir.x, dir.y, dir.z)) * eyeTranslateMat;
}

void Camera::yStrave(glm::vec3 dir) {
    centerTranslateMat  = glm::translate(glm::vec3(dir.x, dir.y, dir.z)) * centerTranslateMat;
}

void Camera::zoom(float dir) {
    camera_distance_ += dir;
    eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
}

const glm::mat4& Camera::getViewMatrix() {
    glm::vec3 eye = glm::vec3(eyeTranslateMat * eyeRotateMat * glm::vec4(eye_, 1));
    glm::vec3 center = glm::vec3(centerTranslateMat * centerRotateMat * glm::vec4(center_, 1));

    viewMatrix = glm::lookAt(eye, center, up_);

    return viewMatrix;
}


const glm::vec3& Camera::getCameraPosition() {
    finalEye = glm::vec3(eyeTranslateMat * eyeRotateMat * glm::vec4(eye_, 1));
    return finalEye;
}

std::function<const void*()> Camera::viewMatrixDataSource() {
    return [&]() -> const void* {
        return &this->getViewMatrix();
    };
}

std::function<const void*()> Camera::positionDataSource() {
    return [&]() -> const void* {
        return &this->getCameraPosition();
    };
}

std::function<const void*()> Camera::projectionMatrixDataSource() {
    return [&]() -> const void* {
        int window_width, window_height;
        glfwGetWindowSize(window, &window_width, &window_height);
        static float aspect_ = static_cast<float>(window_width) / window_height;
        static glm::mat4 perspectiveMat = glm::perspective((float) (45.0f * (M_PI / 180.0f)), aspect_, 0.1f, 35000.0f);
        return &perspectiveMat;
    };
}

Camera::~Camera() { }

} // namespace maguey
