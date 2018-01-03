//
// Created by rdelfin on 7/23/17.
//

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>
#include <maguey/debuggl.h>
#include <iostream>

#include <maguey/skybox.hpp>
#include <maguey/std_shaders.hpp>


namespace maguey {

const std::string Skybox::VERTEX_VBO_NAME = "vertex_position";
const std::string Skybox::FRAGMENT_COLOR_NAME = "vertex_color";

Skybox::Skybox() { }

void load_cube_map_face(GLenum side_target, const char* file) {
    int width, height, nrChannels;
    unsigned char* data;
    data = SOIL_load_image(file, &width, &height, &nrChannels, false);
    CHECK_GL_ERROR(glTexImage2D(side_target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    SOIL_free_image_data(data);
}

void Skybox::load(const std::string &right, const std::string &left, const std::string &top, const std::string &bottom,
                 const std::string &back, const std::string &front, Camera& c) {
    generateCube();

    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);

    load_cube_map_face(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front.c_str());
    load_cube_map_face(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back.c_str());
    load_cube_map_face(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top.c_str());
    load_cube_map_face(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom.c_str());
    load_cube_map_face(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, right.c_str());
    load_cube_map_face(GL_TEXTURE_CUBE_MAP_POSITIVE_X, left.c_str());

    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    std::vector<ShaderUniform> uniforms = {
            ShaderUniform("projection", BINDER_MATRIX4_F, c.projectionMatrixDataSource()),
            ShaderUniform("view", BINDER_MATRIX4_F, c.viewMatrixDataSource())
    };

    // Generate VAO
    CHECK_GL_ERROR(glGenVertexArrays(1, &vao));
    CHECK_GL_ERROR(glBindVertexArray(vao));

    // Generate VBO
    vbo.resize(2);
    CHECK_GL_ERROR(glGenBuffers(2, vbo.data()));

    program = Program(uniforms, Shader(SKYBOX_SHADER_VERT, false),
                                Shader(SKYBOX_SHADER_GEOM, false),
                                Shader(SKYBOX_SHADER_FRAG, false));
    GLint programId = program.getProgramId();

    // Bind vertices
    GLuint verticesPosition = 0;
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo[verticesPosition]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(vbo[verticesPosition], 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(vbo[verticesPosition]));
    CHECK_GL_ERROR(glBindAttribLocation(programId, vbo[verticesPosition], VERTEX_VBO_NAME.c_str()));

    // Add a vertex_color output
    CHECK_GL_ERROR(glBindFragDataLocation(programId, 0, FRAGMENT_COLOR_NAME.c_str()));

    // Link the program
    program.link();

    // Add the index (aka the faces)
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]));
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * this->faces.size(), this->faces.data(), GL_STATIC_DRAW));

    // Add uniform locations
    program.addUniformLocations();

}

void Skybox::draw() {
    CHECK_GL_ERROR(glDepthMask(GL_FALSE));

    program.use();
    program.bindUniforms();

    CHECK_GL_ERROR(glBindVertexArray(vao));

    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, textureId));
    CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0));
    CHECK_GL_ERROR(glDepthMask(GL_TRUE));
}


void Skybox::generateCube() {
    vertices = {
            // front
            {-10000.0, -10000.0,  10000.0, 1.0},
            { 10000.0, -10000.0,  10000.0, 1.0},
            { 10000.0,  10000.0,  10000.0, 1.0},
            {-10000.0,  10000.0,  10000.0, 1.0},
            // back
            {-10000.0, -10000.0, -10000.0, 1.0},
            { 10000.0, -10000.0, -10000.0, 1.0},
            { 10000.0,  10000.0, -10000.0, 1.0},
            {-10000.0,  10000.0, -10000.0, 1.0}
    };

    faces = {

            {0, 1, 2}, {2, 3, 0}, // FRONT
            {1, 5, 6}, {6, 2, 1}, // TOP
            {7, 6, 5}, {5, 4, 7}, // BACK
            {4, 0, 3}, {3, 7, 4}, // BOTTOM
            {4, 5, 1}, {1, 0, 4}, // LEFT
            {3, 2, 6}, {6, 7, 3}, // RIGHT
    };
}

Skybox::~Skybox() {

}

} // namespace maguey
