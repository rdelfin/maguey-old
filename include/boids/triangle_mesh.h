//
// Created by rdelfin on 12/4/16.
//

#ifndef PROCEDURALCITIES_TRIANGLEMESH_H
#define PROCEDURALCITIES_TRIANGLEMESH_H

#include <functional>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <boids/camera.h>
#include <boids/mesh_loader.h>
#include <boids/renderable.h>
#include <boids/program.h>

// TODO: Either disallow copy or regenerate data sources on copy
class TriangleMesh : public Renderable {
public:
    TriangleMesh();
    void load(const std::vector<glm::vec4>& vertices, const std::vector<glm::vec4>& normals, const std::vector<glm::uvec3>& faces,
         const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
         Camera& camera);
    void load(const std::string& path, const MeshLoader& loader,
         const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
         Camera& camera);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 forward, glm::vec3 up);
    void setScale(glm::vec3 scale);

    glm::vec3 getPosition();
    glm::vec3 getForward();
    glm::vec3 getUp();
    glm::vec3 getScale();

    virtual void draw();

    virtual ~TriangleMesh();
protected:
    void loadImpl(const std::vector<glm::vec4>& vertices, const std::vector<glm::vec4>& normals, const std::vector<glm::uvec3>& faces,
                  const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
                  Camera& camera);

    std::function<const void*()> modelMatrixDataSource();

    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec3> faces;
    std::vector<unsigned> vbo;
    unsigned vao;

    Program program;

    glm::vec3 position;
    glm::vec3 forward, up;
    glm::vec3 scale;

    glm::mat4 model;

    static const std::string VERTEX_VBO_NAME;
    static const std::string FRAGMENT_COLOR_NAME;
    static const std::string NORMAL_VBO_NAME;
};


#endif //PROCEDURALCITIES_TRIANGLEMESH_H
