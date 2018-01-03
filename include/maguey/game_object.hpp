//
// Created by rdelfin on 7/12/17.
//

#ifndef BOIDS_GAME_OBJECT_HPP
#define BOIDS_GAME_OBJECT_HPP

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include <maguey/game_time.hpp>
#include <maguey/mesh_loader.hpp>
#include <maguey/triangle_mesh.hpp>


// TODO: Implement equals operator to either create a copy of the vector and remove from the one being copied, to disable it, or switch to shared_ptr
class GameObject {
public:
    GameObject();
    GameObject(TriangleMesh* m);
    GameObject(std::vector<TriangleMesh*> m);

    void addMesh(TriangleMesh* m);

    virtual void update(GameTime& gameTime);
    virtual void draw();

    void setPosition(glm::vec3);
    void setVelocity(glm::vec3);
    void setAcceleration(glm::vec3);
    void setRotation(glm::vec3 forward, glm::vec3 up);

    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    glm::vec3 getAcceleration();
    glm::vec3 getForward();
    glm::vec3 getUp();

    /**
     * Roll as defined by the airplane model (rotation around the forward axis)
     * @param radians Radians to rotate by
     */
    void roll(float radians);

    /**
     * Pitch as defined by the airplane model (rotation around the right axis)
     * @param radians Radians to rotate by
     */
    void pitch(float radians);

    /**
     * Yaw as defined by the airplane model (rotation around the up axis)
     * @param radians Radians to rotate by
     */
    void yaw(float radians);

    void rotateAroundAxis(glm::vec3 axis, float radians);

    virtual ~GameObject();
private:
    std::vector<TriangleMesh*> meshes;
    glm::vec3 pos, v, a;
    glm::vec3 forward, up;
};


#endif //BOIDS_GAME_OBJECT_HPP
