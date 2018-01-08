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

namespace maguey {

/**
 * Abstract class used to load in a triangle mesh from a file who's contents
 * can be loaded into an std::string, or alternatively from the string
 * directly.
 */
class MeshLoader {
public:
    MeshLoader() {

    }

    /**
     * Loads in the contents of the file, and fills up the three vectors with
     * the vertex data stored in the file. Returns whether the operation
     * succeded or not.
     * 
     * @param path     The path of the file to load. Relative to the working
     *                 directory.
     * @param vertices A reference to the vertice vector that will be edited to
     *                 contain the vertices specified in the file.
     * @param normals  A reference to the normal vector that will be edited to
     *                 contain the normals specified in the file.
     * @param faces    A reference to the face vector that will be edited to
     *                 contain the indices of the vertices and normals that
     *                 make up the faces specified in the file. Specifically,
     *                 each index in each face corresponds to both the vertex
     *                 and normal.
     */
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
    
    /**
     * Uses the contents string to fill up the three vectors with the vertex
     * data stored in the file. Returns whether the operation succeded or not.
     * 
     * @param contents The contents of the file format being loaded in.
     * @param vertices A reference to the vertice vector that will be edited to
     *                 contain the vertices specified in the contents.
     * @param normals  A reference to the normal vector that will be edited to
     *                 contain the normals specified in the contents.
     * @param faces    A reference to the face vector that will be edited to
     *                 contain the indices of the vertices and normals that
     *                 make up the faces specified in the contents.
     *                 Specifically, each index in each face corresponds to
     *                 both the vertex and normal.
     */
    virtual bool loadString(const std::string& contents,
                            std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& normals,
                            std::vector<glm::uvec3>& faces) const = 0;

    ~MeshLoader() {

    }
};

} // namespace maguey

#endif //BOIDS_MESHLOADER_HPP
