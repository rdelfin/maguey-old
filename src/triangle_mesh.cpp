//
// Created by rdelfin on 12/4/16.
//

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <maguey/debuggl.h>
#include <maguey/triangle_mesh.hpp>
#include <maguey/mesh_loader.hpp>
#include <maguey/program.hpp>

#include <iostream>
#include <vector>
#include <glm/gtx/transform.hpp>

using namespace std;

// TODO: Either disallow copy or regenerate data sources on copy

namespace maguey {

const std::string TriangleMesh::VERTEX_VBO_NAME = "vertex_position";
const std::string TriangleMesh::NORMAL_VBO_NAME = "normal";
const std::string TriangleMesh::FRAGMENT_COLOR_NAME = "vertex_color";


TriangleMesh::TriangleMesh() {

}

void TriangleMesh::load(const std::vector<glm::vec4>& vertices, const std::vector<glm::vec4>& normals, const std::vector<glm::uvec3>& faces,
                        Camera& camera,
                        const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader) {
    loadImpl(vertices, normals, faces, camera, vertexShader, geometryShader, fragmentShader);
}

void TriangleMesh::loadImpl(const std::vector<glm::vec4> &vertices, const std::vector<glm::vec4> &normals,
                            const std::vector<glm::uvec3> &faces, Camera& camera, 
                            const Shader &vertexShader,
                            const Shader &geometryShader, const Shader &fragmentShader) {
    // Set default model position
    this->forward = glm::vec3(1.0f, 0.0f, 0.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);

    //Create uniforms
    std::vector<ShaderUniform> uniforms = { ShaderUniform("projection", BINDER_MATRIX4_F, camera.projectionMatrixDataSource()),
                                            ShaderUniform("model", BINDER_MATRIX4_F, modelMatrixDataSource()),
                                            ShaderUniform("view", BINDER_MATRIX4_F, camera.viewMatrixDataSource()),
                                            ShaderUniform("camera_position", BINDER_VECTOR3_F, camera.positionDataSource()) };

    // Set variables
    this->vertices = vertices;
    this->normals = normals;
    this->faces = faces;
    // Generate VAO
    CHECK_GL_ERROR(glGenVertexArrays(1, &vao));
    CHECK_GL_ERROR(glBindVertexArray(vao));

    // Generate VBO
    vbo.resize(3);
    CHECK_GL_ERROR(glGenBuffers(3, vbo.data()));


    program = Program(uniforms, vertexShader, geometryShader, fragmentShader);
    GLint programId = program.getProgramId();

    // Bind vertices
    GLuint verticesPosition = 0;
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo[verticesPosition]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(verticesPosition, 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(verticesPosition));
    CHECK_GL_ERROR(glBindAttribLocation(programId, verticesPosition, VERTEX_VBO_NAME.c_str()));

    // Bind normals
    GLuint normalsPosition = 1;
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo[normalsPosition]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->normals.size(), this->normals.data(), GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(normalsPosition, 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(normalsPosition));
    CHECK_GL_ERROR(glBindAttribLocation(programId, normalsPosition, NORMAL_VBO_NAME.c_str()));


    // Add a vertex_color output
    CHECK_GL_ERROR(glBindFragDataLocation(programId, 0, FRAGMENT_COLOR_NAME.c_str()));

    // Link the program
    program.link();

    // Add the index (aka the faces)
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]));
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * this->faces.size(), this->faces.data(), GL_STATIC_DRAW));

    // Add uniform locations
    program.addUniformLocations();

}

void TriangleMesh::setPosition(glm::vec3 position) { this->position = position; }
void TriangleMesh::setScale(glm::vec3 scale) { this->scale = scale; }
void TriangleMesh::setRotation(glm::vec3 forward, glm::vec3 up) {
    this->forward = forward;
    this->up = up;
}

glm::vec3 TriangleMesh::getPosition() { return position; }
glm::vec3 TriangleMesh::getForward() { return forward; }
glm::vec3 TriangleMesh::getUp() { return up; }
glm::vec3 TriangleMesh::getScale() { return scale; }

std::function<const void*()> TriangleMesh::modelMatrixDataSource() {
    return [&]() -> const void* {
        glm::vec3 right = glm::normalize(glm::cross(this->forward, this->up));
        glm::vec3 up = glm::normalize(glm::cross(right, this->forward));
        glm::vec3 forward = glm::normalize(this->forward);
        model = glm::translate(position)
                * glm::mat4(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
                * glm::scale(scale);
        return &model;
    };
}

void TriangleMesh::draw() {
    CHECK_GL_ERROR(glBindVertexArray(vao));
    program.use();
    program.bindUniforms();

    // Draw vertices
    CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0));
}

TriangleMesh::~TriangleMesh() {

}

const std::vector<glm::vec4>& TriangleMesh::getVertices() { return this->vertices; }
const std::vector<glm::vec4>& TriangleMesh::getNormals() { return this->normals; }
const std::vector<glm::uvec3>& TriangleMesh::getFaces() { return this->faces; }

} // namespace maguey
