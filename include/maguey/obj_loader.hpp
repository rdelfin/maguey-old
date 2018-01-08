//
// Created by rdelfin on 7/2/17.
//

#ifndef BOIDS_OBJLOADER_HPP
#define BOIDS_OBJLOADER_HPP


#include <maguey/mesh_loader.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

namespace maguey {

/**
 * Class used to load in a triangle mesh from a Wavefront .obj file.
 * 
 * The current version is very barebones and only loads in vertices, normals,
 * and faces. It will ignore all material, section, or any other data. It does
 * however, process quads and turns them to triangles.
 */
class ObjLoader : public MeshLoader {
public:
    ObjLoader();
    
    /**
     * Uses the contents string to fill up the three vectors with the vertex
     * data stored in the Wavefront .obj file. Returns whether the operation
     * succeded or not. Notably, it'll return false if it finds an invalid
     * line. In this case, the contents of vertices, normals, and faces are not
     * guaranteed to be in their initial state.
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
                            std::vector<glm::uvec3>& faces) const;
    ~ObjLoader();

private:
    bool process_face(const std::string& line, std::vector<glm::uvec3>& facesRaw, std::vector<glm::uvec3>& normalIdx) const;
    std::vector<std::vector<uint>> parse_line_elems(const std::vector<std::string>& line_elems) const;
};

} // namespace maguey

#endif //BOIDS_OBJLOADER_HPP
