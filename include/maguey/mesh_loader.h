//
// Created by rdelfin on 7/2/17.
//

#ifndef BOIDS_MESHLOADER_HPP
#define BOIDS_MESHLOADER_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <vector>


class MeshLoader {
public:
    MeshLoader() {

    }

    virtual bool loadFile(const std::string& path,
                          std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& normals,
                          std::vector<glm::uvec3>& faces) const {

        std::ifstream fileStream(path);

        if(!fileStream)
            return false;
        
        std::string fileContents = std::string(std::istreambuf_iterator<char>(fileStream),
                                               std::istreambuf_iterator<char>());
        return loadString(fileContents, vertices, normals, faces);
    }
    
    virtual bool loadString(const std::string& contents,
                            std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& normals,
                            std::vector<glm::uvec3>& faces) const = 0;

    ~MeshLoader() {

    }
};


#endif //BOIDS_MESHLOADER_HPP
