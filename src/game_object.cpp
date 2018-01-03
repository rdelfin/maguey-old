//
// Created by rdelfin on 7/12/17.
//

#define GLM_FORCE_RADIANS

#include <glm/gtx/rotate_vector.hpp>

#include <maguey/game_object.hpp>

namespace maguey {

GameObject::GameObject() : forward(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0) {

}


GameObject::GameObject(TriangleMesh* m) : forward(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0) {
    meshes.push_back(m);
}

GameObject::GameObject(std::vector<TriangleMesh*> m) : forward(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0) {
    meshes.insert(meshes.end(), m.begin(), m.end());
}

void GameObject::addMesh(TriangleMesh* m) {
    meshes.push_back(m);
}

void GameObject::setPosition(glm::vec3 p) { this->pos = p; }
void GameObject::setVelocity(glm::vec3 v) { this->v = v; }
void GameObject::setAcceleration(glm::vec3 a) { this->a = a; }
void GameObject::setRotation(glm::vec3 forward, glm::vec3 up) {
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    this->forward = forward;
    this->up = glm::normalize(glm::cross(right, forward));

}

glm::vec3 GameObject::getPosition() { return this->pos; }
glm::vec3 GameObject::getVelocity() { return this->v; }
glm::vec3 GameObject::getAcceleration() { return this->a; }
glm::vec3 GameObject::getForward() { return this->forward; }
glm::vec3 GameObject::getUp() { return this->up; }


void GameObject::roll(float radians) {
    this->up = glm::normalize(glm::rotate(up, radians, this->forward));
    this->forward = glm::normalize(glm::rotate(forward, radians, this->forward));
}

void GameObject::pitch(float radians) {
    glm::vec3 right = glm::normalize(glm::cross(this->forward, this->up));
    this->forward = glm::normalize(glm::rotate(this->forward, radians, right));
    this->up = glm::normalize(glm::rotate(this->up, radians, right));
}

void GameObject::yaw(float radians) {
    this->forward = glm::normalize(glm::rotate(this->forward, radians, this->up));
    this->up = glm::normalize(glm::rotate(this->up, radians, this->up));
}

void GameObject::rotateAroundAxis(glm::vec3 axis, float radians) {
    this->forward = glm::normalize(glm::rotate(this->forward, radians, axis));
    this->up = glm::normalize(glm::rotate(this->up, radians, axis));
}

void GameObject::update(GameTime& gameTime) {
    glm::vec3 right = glm::cross(forward, up);

    v += a*(float)(gameTime.getMillisecondsPerFrame()/1000.0f);
    pos += glm::mat3(right, up, forward) * v*(float)(gameTime.getMillisecondsPerFrame()/1000.0f);

    for(auto it = meshes.begin(); it != meshes.end(); it++) {
        (*it)->setPosition(pos);
        (*it)->setRotation(forward, up);
    }
}

void GameObject::draw() {
    for(auto it = meshes.begin(); it != meshes.end(); ++it)
        (*it)->draw();
}

GameObject::~GameObject() {
    for(auto it = meshes.begin(); it != meshes.end(); ++it)
        delete *it;
}

} // namespace maguey
