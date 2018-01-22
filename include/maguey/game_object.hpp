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
#include <maguey/program.hpp>
#include <maguey/triangle_mesh.hpp>

namespace maguey {

/**
 * Represents a renderable, moveable GameObject. Each GameObject can contain
 * multiple TriangleMesh'es, all of which will be moved and rotated together.
 *
 * Example:
 * ```
 * TriangleMesh mesh(Create a new triangle mesh...);
 * GameObject game_object;
 * game_object.addMesh(mesh);
 * ...
 * public YourGame::update(GameTime& gameTime) {
 *     ...
 *     game_object.update(gameTime);
 *     ...
 * }
 *
 * public YourGame::draw() {
 *     ...
 *     game_object.draw();
 *     ...
 * }
 * ```
 */
class GameObject {
public:
    /**
     * Default constructor creates an empty GameObject
     */
    GameObject();

    /**
     * Constructor creates GameObject and adds mesh
     * @param m TriangleMesh pointer to add
     */
    GameObject(TriangleMesh* m);

    /**
     * Constructor created GameObjct and adds all TriangleMeshes.
     * @param m Vector of TriangleMesh pointers to add.
     */
    GameObject(std::vector<TriangleMesh*> m);

    /**
     * Add additional TriangleMesh
     * @param m Mesh to add.
     */
    void addMesh(TriangleMesh* m);

    /**
     * Adds all meshes corresponding to a 3D model file.
     *
     * @param filePath       File path to load.
     * @param meshLoader     Loader to use for this file format.
     * @param camera         Camera object, used to for the data sources.
     * @param vertexShader   Vertex shader used for all meshes in file, unless
     *                       a different shader is specified in the file.
     * @param geometryShader Geometry shader used for all meshes in file,
     *                       unless a different shader is specified in the
     *                       file.
     * @param fragmentShader Fragment shader used for all meshes in file,
     *                       unless a different shader is specified in the
     *                       file.
     */
    void loadFile(const std::string& filePath, const MeshLoader& meshLoader,
                  Camera& camera,
                  const Shader& vertexShader = Shader(MESH_SHADER_VERT, false),
                  const Shader& geometryShader = Shader(MESH_SHADER_GEOM, false),
                  const Shader& fragmentShader = Shader(MESH_SHADER_FRAG, false));

    /**
     * Run every frame to update movement
     * @param gameTime GameTime object used to make fps-indepemdent mvoement
     */
    virtual void update(GameTime& gameTime);

    /**
     * Call to render all meshes on the window
     */
    virtual void draw();

    /**
     * Set the position of the object
     * @param position 3D vector of the position
     */
    void setPosition(glm::vec3 position);
    /**
     * Set the velocity of the object
     * @param velocity Velocity in units/second
     */
    void setVelocity(glm::vec3 velocity);
    /**
     * Set the acceleration of the object
     * @param acceleration Acceleration in units/(second)^2
     */
    void setAcceleration(glm::vec3 acceleration);
    /**
     * Set rotation in radians. Forward and up vectors are
     * made ortho-normal by the Gram–Schmidt process
     * @param forward Direction the camera is facing to
     * @param up      Vector for the top of the camera
     */
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

} // namespace maguey

#endif //BOIDS_GAME_OBJECT_HPP
