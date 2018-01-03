//
// Created by rdelfin on 7/23/17.
//

#ifndef BOIDS_CUBEMAP_HPP
#define BOIDS_CUBEMAP_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include <maguey/program.hpp>
#include <maguey/camera.hpp>

class Skybox {
public:
    Skybox();
    void load(const std::string& right, const std::string& left,
            const std::string& top, const std::string& bottom,
            const std::string& back, const std::string& front,
            Camera& c);

    void draw();

    ~Skybox();
private:
    GLuint textureId;
    Program program;

    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;

    std::vector<unsigned> vbo;
    unsigned vao;


    void generateCube();

    static const std::string VERTEX_VBO_NAME;
    static const std::string FRAGMENT_COLOR_NAME;
    static const std::string NORMAL_VBO_NAME;
};


#endif //BOIDS_CUBEMAP_HPP
