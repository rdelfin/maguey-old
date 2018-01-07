//
// Created by rdelfin on 12/7/16.
//

#ifndef PROCEDURALCITIES_CAMERA_H
#define PROCEDURALCITIES_CAMERA_H

#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace maguey {


/**
 * Creates a camera control class, with view, position and projection matrices
 * generated from the actions applied on said camera.
 */
class Camera {
public:
    /**
     * Used to allow constructors to create a window and then set this. Do not
     * use without following it up with a call to the main constructor below.
     */
    Camera();

    /**
     * Main constructor. Creates a camera associated with a given GLFW window.
     */
    Camera(GLFWwindow* window);

    /**
     * Moves camera entirely to a new position. If all three arguments are not
     * orthonormal, they'll be made so using the Gram-Schmidt method and
     * normalization.
     * @param pos The new camera position
     * @param dir A direction vector in which to look (NOT the point the camera is looking at)
     * @param up  The upwards direction of the camera.
     */
    void reposition(glm::vec3 pos, glm::vec3 dir, glm::vec3 up);

    /**
     * Rotation around the y axis.
     * @param dir Ammount of rotation, in radians. Positive to the left,
     *            negative to the right
     */
    void yaw(float dir);

    /**
     * Rotation around the z axis.
     * @param dir Ammount of rotation, in radians. Positive to the right (cw),
     *            negative to the left (ccw).
     */
    void pitch(float dir);

    void fpsYaw(float dir);
    void fpsPitch(float dir);

    /**
     * Rotation around the x axis.
     * @param dir Ammount of rotation, in radians. Positive is up, negative is
     *            down.
     */
    void roll(float dir);

    /**
     * Moves the camera in the direction of the dir vector, relative to the
     * camera frame (defined by the dir and up vectors).
     * @param dir Direction and ammount of pan.
     */
    void pan(glm::vec3 dir);

    /**
     * Camera rotation around the center point (aka what the camera is looking
     * at).
     * @param dir Direction and ammount of pan, relative to the zoom.
     */
    void strave(glm::vec3 dir);

    void yPan(glm::vec3 dir);
    void yStrave(glm::vec3 dir);

    /**
     * Zoom in and out, by reducing distance between center and camera
     * @param dir Ammount of zoom, in world units.
     */
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

} // namespace maguey


#endif //PROCEDURALCITIES_CAMERA_H
