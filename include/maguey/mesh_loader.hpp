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
#include <unordered_map>

#include <maguey/triangle_mesh.hpp>
#include <maguey/program.hpp>
#include <maguey/std_shaders.hpp>

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
     * Loads in the contents of the file, and returns a list of triangle meshes
     * generated from the file (each file might contain multiple objects). The
     * names can either be generated from the files or by the MeshLoader
     * implementation by its own policy.
     * 
     * @param path           The path of the file to load. Relative to the
     *                       working directory.
     * @param error          Is set to true if there was an error. Else set to
     *                       false.
     * @param camera         Camera object used to create the TriangleMesh
     *                       objects.
     * @param vertexShader   Default vertex shader if none is specified by the
     *                       model.
     * @param geometryShader Default geometry shader if none is specified by
     *                       the model.
     * @param fragmentShader Default fragment shader if none is specified by
     *                       the model.
     */
    virtual std::unordered_map<std::string, TriangleMesh> loadFile(const std::string& path, bool& error,
                                                                   Camera& camera,
                                                                   const Shader& vertexShader = Shader(MESH_SHADER_VERT, false),
                                                                   const Shader& geometryShader = Shader(MESH_SHADER_GEOM, false),
                                                                   const Shader& fragmentShader = Shader(MESH_SHADER_VERT, false)) const {
        error = false;

        std::ifstream fileStream(path);

        if(!fileStream) {
            error = true;
            return {};
        }

        std::string fileContents = std::string(std::istreambuf_iterator<char>(fileStream),
                                               std::istreambuf_iterator<char>());
        return loadString(fileContents, error, camera, vertexShader, geometryShader, fragmentShader);
    }
    
    /**
     * Uses the contents string to create a set of triangle meshes generated
     * from the contents (each content string might contain multiple objects).
     * The names can either be generated from the files or by the MeshLoader
     * implementation by its own policy.
     * 
     * @param contents       The contents of the file format being loaded in.
     * @param error          Is set to true if there was an error. Else set to
     *                       false.
     * @param camera         Camera object used to create the TriangleMesh
     *                       objects.
     * @param vertexShader   Default vertex shader if none is specified by the
     *                       model.
     * @param geometryShader Default geometry shader if none is specified by
     *                       the model.
     * @param fragmentShader Default fragment shader if none is specified by
     *                       the model.
     */
    virtual std::unordered_map<std::string, TriangleMesh>  loadString(const std::string& contents, bool& error,
                                                                      Camera& camera,
                                                                      const Shader& vertexShader = Shader(MESH_SHADER_VERT, false),
                                                                      const Shader& geometryShader = Shader(MESH_SHADER_GEOM, false),
                                                                      const Shader& fragmentShader = Shader(MESH_SHADER_VERT, false)) const = 0;

    ~MeshLoader() {

    }
};

} // namespace maguey

#endif //BOIDS_MESHLOADER_HPP
