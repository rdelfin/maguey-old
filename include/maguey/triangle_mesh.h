//
// Created by rdelfin on 12/4/16.
//

#ifndef PROCEDURALCITIES_TRIANGLEMESH_H
#define PROCEDURALCITIES_TRIANGLEMESH_H

#include <functional>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <maguey/camera.h>
#include <maguey/mesh_loader.h>
#include <maguey/renderable.h>
#include <maguey/program.h>

#include <maguey/std_shaders.hpp>

// TODO: Either disallow copy or regenerate data sources on copy
class TriangleMesh : public Renderable {
public:
    TriangleMesh();
    void load(const std::vector<glm::vec4>& vertices, const std::vector<glm::vec4>& normals, const std::vector<glm::uvec3>& faces,
         Camera& camera,
         const Shader& vertexShader = Shader(MESH_SHADER_VERT, false),
         const Shader& geometryShader = Shader(MESH_SHADER_GEOM, false),
         const Shader& fragmentShader = Shader(MESH_SHADER_VERT, false));
    void load(const std::string& path, const MeshLoader& loader, Camera& camera,
         const Shader& vertexShader = Shader(MESH_SHADER_VERT, false),
         const Shader& geometryShader = Shader(MESH_SHADER_GEOM, false),
         const Shader& fragmentShader = Shader(MESH_SHADER_FRAG, false));

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
                  Camera& camera,
                  const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader);

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
