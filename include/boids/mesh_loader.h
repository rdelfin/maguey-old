//
// Created by rdelfin on 7/2/17.
//

#ifndef BOIDS_MESHLOADER_HPP
#define BOIDS_MESHLOADER_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>


class MeshLoader {
public:
    MeshLoader() {

    }

    virtual bool loadFile(const std::string& path,
                          std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& normals,
                          std::vector<glm::uvec3>& faces) const = 0;

    ~MeshLoader() {

    }
};


#endif //BOIDS_MESHLOADER_HPP
