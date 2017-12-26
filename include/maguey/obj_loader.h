//
// Created by rdelfin on 7/2/17.
//

#ifndef BOIDS_OBJLOADER_HPP
#define BOIDS_OBJLOADER_HPP


#include <maguey/mesh_loader.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

class ObjLoader : public MeshLoader {
public:
    ObjLoader();
    virtual bool loadFile(const std::string& path,
                          std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& normals,
                          std::vector<glm::uvec3>& faces) const;
    ~ObjLoader();
};


#endif //BOIDS_OBJLOADER_HPP
