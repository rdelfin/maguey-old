//
// Created by rdelfin on 12/4/16.
//

#ifndef PROCEDURALCITIES_TRIANGLEMESH_H
#define PROCEDURALCITIES_TRIANGLEMESH_H

#include <functional>
#include <unordered_map>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest_prod.h>

#include <maguey/camera.hpp>
#include <maguey/renderable.hpp>
#include <maguey/program.hpp>
#include <maguey/std_shaders.hpp>


namespace maguey {

/**
 * Represents a basic triangle mesh, with basic rendering information,
 * positioning, scaling and rotation functionality..
 */
class TriangleMesh : public Renderable {
public:
    /**
     * Default constructor for the TriangleMesh. Should only be used if an
     * object needs to be initialized without parameters and should be replaced
     * by an object using the main constructor.
     */
    TriangleMesh();

    /**
     * Main constructor for the TriangleMesh class. Creates triangle mesh only
     * with the basic data structures. Must be followed by a call to any load()
     * method overload.  The shaders will be provided with the uniforms:
     * `projection` (projection matrix, mat4), `model` (the model matrix,
     * mat4), view (the  view matrix, mat4) and `camera_position` (the camera
     * position, vec3). They will also be provided the vertex VBO's under the
     * name `vertex_position`, the normal VBO's under `normal`, and the vertex
     * color output must be provided under the name `vertex_color`.
     * 
     * @param vertices       A vector containing a list of vertices.
     * @param normals        A vector containing a list of normals, each one
     *                       corresponding to each vertex in vertices.
     * @param faces          A vector of faces, each one containing the indices
     *                       of the vertices making up said face.
     * @param vertexShader   The vertex shader to be used for this mesh.
     * @param geometryShader The geometry shader to be used for this mesh.
     * @param fragmentShader The fragment shader to be used for this mesh.
     */
    TriangleMesh(const std::vector<glm::vec4>& vertices,
                 const std::vector<glm::vec4>& normals,
                 const std::vector<glm::uvec3>& faces,
                 const Shader& vertexShader = Shader(MESH_SHADER_VERT, false),
                 const Shader& geometryShader = Shader(MESH_SHADER_GEOM, false),
                 const Shader& fragmentShader = Shader(MESH_SHADER_VERT, false));

    /**
     * Loads a new triangle mesh into OpenGL using a camera object.
     *
     * @param camera The camera object associated with this rendering.
     */
    void load(Camera& camera);

    /**
     * Sets the position of the mesh in world coordinates.
     *
     * @param position the 3D vector representing the mesh's position.
     */
    void setPosition(glm::vec3 position);

    /**
     * Sets the rotation of the vector by defining a forward and up vectors.
     * If these are not orthonormal, they are made so using the gram-schmidt
     * method. The forward vector's direction is kept intact.
     *
     * @param forward The forward direction of the camera.
     * @param up      The upwards direction of the camera.
     */
    void setRotation(glm::vec3 forward, glm::vec3 up);

    /**
     * Scales the mesh up or down on any of the three directions.
     *
     * @param scale The ammount to scale the mesh by on each axis.
     */
    void setScale(glm::vec3 scale);

    /**
     * Returns the vec3 representing the mesh's world position
     */
    glm::vec3 getPosition();

    /**
     * Returns the vec3 for the forward vector.
     */
    glm::vec3 getForward();

    /**
     * Returns the vec3 for the up vector.
     */
    glm::vec3 getUp();

    /**
     * Returns the vec3 for the scaling on each axis.
     */
    glm::vec3 getScale();

    /**
     * This method will render the mesh on the screen.
     */
    virtual void draw() override;

    virtual ~TriangleMesh();

    //*********************************************
    //*           Functions for tests             *
    //*********************************************

    /**
     * Getter for the vertices. Used only for test purposes.
     */
    const std::vector<glm::vec4>& getVertices();

    /**
     * Getter for the vertices. Used only for test purposes.
     */
    const std::vector<glm::vec4>& getNormals();

    /**
     * Getter for the vertices. Used only for test purposes.
     */
    const std::vector<glm::uvec3>& getFaces();

protected:
    void loadImpl(Camera& camera);

    std::function<const void*()> modelMatrixDataSource();

    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec3> faces;
    std::vector<unsigned> vbo;
    unsigned vao;

    Shader vertexShader, geometryShader, fragmentShader;
    Program program;

    glm::vec3 position;
    glm::vec3 forward, up;
    glm::vec3 scale;

    glm::mat4 model;

    static const std::string VERTEX_VBO_NAME;
    static const std::string FRAGMENT_COLOR_NAME;
    static const std::string NORMAL_VBO_NAME;

    
};

} // namespace maguey

#endif //PROCEDURALCITIES_TRIANGLEMESH_H
