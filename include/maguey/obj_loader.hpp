//
// Created by rdelfin on 7/2/17.
//

#ifndef BOIDS_OBJLOADER_HPP
#define BOIDS_OBJLOADER_HPP


#include <maguey/mesh_loader.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <maguey/triangle_mesh.hpp>
#include <maguey/std_shaders.hpp>
#include <maguey/program.hpp>

#include <vector>
#include <unordered_map>
#include <string>

namespace maguey {

    namespace internal {
        typedef struct {
            std::vector<glm::uvec3> faceIdx;
            std::vector<glm::uvec3> normalIdx;
        } index_data;
    }

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
     * Uses the contents string to create a set of triangle meshes generated
     * from the Wavefront .obj file (each file might contain multiple objects).
     * The names for each triangle mesh is based on 
     * 
     * @param contents The contents of the file format being loaded in.
     * @param error          Is set to true if there was an error (notably if
     *                       an invalid line is found). Else is set to false.
     * @param camera         Camera object used to create the TriangleMesh
     *                       objects.
     * @param vertexShader   Default vertex shader if none is specified by the
     *                       model.
     * @param geometryShader Default geometry shader if none is specified by
     *                       the model.
     * @param fragmentShader Default fragment shader if none is specified by
     *                       the model.
     */
    virtual std::unordered_map<std::string, TriangleMesh*>  loadString(const std::string& contents, bool& error,
                                                                       Camera& camera,
                                                                       const Shader& vertexShader = Shader(MESH_SHADER_VERT, false),
                                                                       const Shader& geometryShader = Shader(MESH_SHADER_GEOM, false),
                                                                       const Shader& fragmentShader = Shader(MESH_SHADER_VERT, false)) const;
    ~ObjLoader();

private:
    bool process_face(const std::string& line, internal::index_data& mesh_index_data, int line_num) const;
    
    std::vector<std::vector<uint>> parse_line_elems(const std::vector<std::string>& line_elems) const;
    
    bool load_data_into_meshes(const std::unordered_map<std::string, internal::index_data>& index_map,
                               const std::vector<glm::vec4>& vertexRaw, const std::vector<glm::vec4>& normalRaw,
                               std::unordered_map<std::string, TriangleMesh*>& meshes,
                               Camera& camera,
                               const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader) const;
};

} // namespace maguey

#endif //BOIDS_OBJLOADER_HPP
