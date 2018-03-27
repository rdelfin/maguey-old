//
// Created by rdelfin on 7/2/17.
//

#define GLM_FORCE_RADIANS

#include <maguey/camera.hpp>
#include <maguey/obj_loader.hpp>
#include <maguey/program.hpp>
#include <maguey/triangle_mesh.hpp>
#include <maguey/util.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace maguey {

ObjLoader::ObjLoader() {

}

std::vector< std::vector<uint> > ObjLoader::parse_line_elems(const std::vector<std::string>& line_elems) const {
    std::vector< std::vector<uint> > result;
    for(const std::string& vert : line_elems) {
        std::vector<std::string> vert_vals_str = split(vert, '/');
        std::vector<uint> vert_vals_uint;
        vert_vals_uint.resize(vert_vals_str.size());
        std::transform(vert_vals_str.begin(), vert_vals_str.end(), vert_vals_uint.begin(),
                       [](std::string s) -> uint { return s == "" ? 0 : std::stoul(s); });
        
        result.push_back(vert_vals_uint);
    }

    return result;
}

bool ObjLoader::process_face(const std::string& line, maguey::internal::index_data& mesh_index_data, int line_num) const {
    std::string header;

    std::vector<std::string> lineElems = split(line, ' ');
    lineElems = std::vector<std::string>(lineElems.begin() + 1, lineElems.end());

    if(lineElems.size() != 3 && lineElems.size() != 4) {
        std::cerr << "Invalid face line: \"" << line << "\". Should have 3 or 4 elements instead of " << lineElems.size() << std::endl;
        return false;
    }

    std::vector< std::vector<uint> > vals = parse_line_elems(lineElems);

    // Check that all values have same number of elements
    for(size_t i = 0; i < vals.size() - 1; i++) {
        if(vals[i].size() != vals[i+1].size()) {
            std::cerr
                << "Invalid face line. The coordinates do not have the same number of elements (either 1 or 3): \""
                << line << "\"" << std::endl;
            return false;
        }
    }


    // Regular triangle
    if(vals.size() == 3) {
        if (vals[0].size() == 1) {
            mesh_index_data.faceIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[2][0] - 1));
            mesh_index_data.normalIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[2][0] - 1));
        } else if (vals[0].size() == 3) {
            mesh_index_data.faceIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[2][0] - 1));
            mesh_index_data.normalIdx.push_back(glm::uvec3(vals[0][2] - 1, vals[1][2] - 1, vals[2][2] - 1));
        } else {
            std::cerr << "Invalid face line. One coordinate has the wrong number of elements (either 1 or 3): \""
                        << line << "\"" << std::endl;
            return false;
        }
    }
    
    // Add quad as two triangles
    else if(vals.size() == 4) {
        if (vals[0].size() == 1) {
            // First triangle (1st, 2nd and 4th vertices)
            mesh_index_data.faceIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[3][0] - 1));
            mesh_index_data.normalIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[3][0] - 1));

            // Second triangle (2nd, 3rd and 4th vertices)
            mesh_index_data.faceIdx.push_back(glm::uvec3(vals[1][0] - 1, vals[2][0] - 1, vals[3][0] - 1));
            mesh_index_data.normalIdx.push_back(glm::uvec3(vals[1][0] - 1, vals[2][0] - 1, vals[3][0] - 1));
        } else if (vals[0].size() == 3) {
            // First triangle (1st, 2nd and 4th vertices)
            mesh_index_data.faceIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[3][0] - 1));
            mesh_index_data.normalIdx.push_back(glm::uvec3(vals[0][2] - 1, vals[1][2] - 1, vals[3][2] - 1));

            // Second triangle (2nd, 3rd and 4th vertices)
            mesh_index_data.faceIdx.push_back(glm::uvec3(vals[1][0] - 1, vals[2][0] - 1, vals[3][0] - 1));
            mesh_index_data.normalIdx.push_back(glm::uvec3(vals[1][2] - 1, vals[2][2] - 1, vals[3][2] - 1));
        } else {
            std::cerr << "Invalid face line. One coordinate has the wrong number of elements (either 1 or 3): \""
                        << line << "\"" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Invalid face line. There can only be 3 or 4 elements in the face line and there are "
                  << vals.size() << ": \"" << line << "\"" << std::endl; 
    }

    return true;
}

bool ObjLoader::load_data_into_meshes(const std::unordered_map<std::string, internal::index_data>& index_map,
                                      const std::vector<glm::vec4>& vertexRaw, const std::vector<glm::vec4>& normalRaw,
                                      std::unordered_map<std::string, TriangleMesh*>& meshes,
                                      Camera& camera,
                                      const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader) const {

    for(const std::pair<std::string, internal::index_data>& data_pair : index_map) {
        const internal::index_data& mesh_index_data = data_pair.second;
        std::vector<glm::vec4> vertices, normals;
        std::vector<glm::uvec3> faces;


        for(size_t i = 0; i < mesh_index_data.faceIdx.size(); i++) {
            size_t start_vertices = vertices.size();
            glm::uvec3 face = mesh_index_data.faceIdx[i];
            glm::uvec3 normal = mesh_index_data.normalIdx[i];

            // Push faces and normals.
            vertices.push_back(vertexRaw[face[0]]);
            vertices.push_back(vertexRaw[face[1]]);
            vertices.push_back(vertexRaw[face[2]]);

            normals.push_back(normalRaw[normal[0]]);
            normals.push_back(normalRaw[normal[1]]);
            normals.push_back(normalRaw[normal[2]]);

            // Create face vector and push
            faces.push_back(glm::uvec3(start_vertices, start_vertices+1, start_vertices+2));
        }

        meshes.insert({data_pair.first, new TriangleMesh});
        meshes[data_pair.first]->load(vertices, normals, faces, camera, vertexShader, geometryShader, fragmentShader);
    }
}

std::unordered_map<std::string, TriangleMesh*> ObjLoader::loadString(const std::string& contents, bool& error,
                                                                     Camera& camera,
                                                                     const Shader& vertexShader,
                                                                     const Shader& geometryShader,
                                                                     const Shader& fragmentShader) const {
    // Result map returned at the end
    std::unordered_map<std::string, TriangleMesh*> meshes;
    // Set error flag to false
    error = false;

    std::vector<glm::vec4> vertexRaw;
    std::vector<glm::vec4> normalRaw;

    std::unordered_map<std::string, internal::index_data> index_map;

    std::stringstream ss(contents);
    std::string line;

    // Used for debugging purposes
    int line_num = 0;

    // Vertex state
    std::string group = "";

    while(std::getline(ss, line)) {
        line_num++;
        std::stringstream line_stream(line);
        std::string header;

        line_stream >> header;

        // Group definition: Either "g group_name" or "g obj_name group_name".
        // More than one argument is ignored
        if(header == "g") {

            std::string header, gname;

            if(!(line_stream >> gname)) {
                std::cerr << "Invalid group on line " << line_num << ": \"" << line << "\"" << std::endl;
                error = true;
                return std::unordered_map<std::string, TriangleMesh*>();
            }

            group = gname;
        }

        if(header == "v") {
            double x, y, z;

            if(!(line_stream >> x >> y >> z)) {
                std::cerr << "Invalid vertex on line " << line_num << ": \"" << line << "\"" << std::endl;
                error = true;
                return std::unordered_map<std::string, TriangleMesh*>();
            }

            vertexRaw.push_back(glm::vec4(x, y, z, 1.0f));
        }

        else if(header == "vn") {
            double x, y, z;

            if(!(line_stream >> x >> y >> z)) {
                std::cerr << "Invalid vertex normal line " << line_num << ": \"" << line << "\"" << std::endl;
                error = true;
                return std::unordered_map<std::string, TriangleMesh*>();
            }

            normalRaw.push_back(glm::vec4(x, y, z, 0.0f));
        }

        else if(header == "f") {
            if(index_map.count(group) == 0)
                index_map.insert({group, internal::index_data()});
            if(!process_face(line, index_map[group], line_num)) {
                error = true;
                return std::unordered_map<std::string, TriangleMesh*>();
            }
        }
    }


    if(!load_data_into_meshes(index_map, vertexRaw, normalRaw, meshes, camera, vertexShader, geometryShader, fragmentShader)) {
        std::cerr << "There was an error loading indices into meshes. Consult errors above." << std::endl;
        error = true;
        return std::unordered_map<std::string, TriangleMesh*>();
    }

    return meshes;
}

std::unordered_map<std::string, Material> ObjLoader::loadMaterialFile(const std::string& file, bool& error) {
    error = false;

    std::ifstream fileStream(file);

    if(!fileStream) {
        error = true;
        return {};
    }

    std::string fileContents = std::string(std::istreambuf_iterator<char>(fileStream),
                                            std::istreambuf_iterator<char>());
    return loadMaterialString(fileContents, error);
}

std::unordered_map<std::string, Material> ObjLoader::loadMaterialString(const std::string& contents, bool& error) {
    std::unordered_map<std::string, Material> materials;

    error = false;

    std::stringstream ss(contents);
    std::string line;
    
    bool created_material = false;
    std::string material_name;
    glm::vec3 ambient;
    glm::vec3 difuse;
    glm::vec3 specular;
    float specular_exp;
    float transparency;
    bool ambient_set = false, difuse_set = false, specular_set = false, specular_exp_set = false, transparency_set = false;

    while(std::getline(ss, line)) {
        std::stringstream line_stream(line);
        std::string header;
        line_stream >> header;

        // Check for new material line
        if(header == "newmtl") {
            std::stringstream line_stream(line);
            std::string header, name;

            // Save previous material
            if(created_material) {
                if(ambient_set && difuse_set && specular_set && specular_exp_set && transparency_set)
                    materials.insert({material_name, Material(material_name, ambient, difuse, specular, specular_exp, transparency)});
                else
                    std::cerr << "There was an error loading material " << material_name << ". Some components were missing." << std::endl;
            }

            material_name = name;
            ambient_set = difuse_set = specular_set = specular_exp_set = transparency_set = false;
            created_material = true;
        }

        // Different material properties below set here

        else if(header == "Ka") {
            float r, g, b;

            if(!(line_stream >> r >> g >> b)) {
                std::cerr << "There was an error reading the Ka property (ambient color) for material \"" << material_name << "\""  << std::endl;
                error = true;
                return std::unordered_map<std::string, Material>();
            }

            ambient = glm::vec3(r, g, b);
            ambient_set = true;
        }

        else if(header == "Kd") {
            float r, g, b;
            if(!(line_stream >> r >> g >> b)) {
                std::cerr << "There was an error reading the Kd property (Difuse color) for material \"" << material_name << "\""  << std::endl;
                error = true;
                return std::unordered_map<std::string, Material>();
            }

            difuse = glm::vec3(r, g, b);
            difuse_set = true;
        }

        else if(header == "Ks") {
            float r, g, b;
            if(!(line_stream >> r >> g >> b)) {
                std::cerr << "There was an error reading the Ks property (specular color) for material \"" << material_name << "\""  << std::endl;
                error = true;
                return std::unordered_map<std::string, Material>();
            }

            specular = glm::vec3(r, g, b);
            difuse_set = true;
        }

        else if(header == "Ns") {
            if(!(line_stream >> specular_exp)) {
                std::cerr << "There was an error reading the Ns property (specular exponent) for material \"" << material_name << "\""  << std::endl;
                error = true;
                return std::unordered_map<std::string, Material>();
            }

            specular_exp_set = true;
        }

        else if(header == "d") {
            if(!(line_stream >> transparency)) {
                std::cerr << "There was an error reading the d property (opacity) for material \"" << material_name << "\""  << std::endl;
                error = true;
                return std::unordered_map<std::string, Material>();
            }

            transparency = 1.0 - transparency;
            transparency_set = true; 
        }

        else if(header == "Tr") {
            if(!(line_stream >> transparency)) {
                std::cerr << "There was an error reading the Tr property (transparency) for material \"" << material_name << "\""  << std::endl;
                error = true;
                return std::unordered_map<std::string, Material>();
            }

            transparency_set = true;
        }
        else
            std::cerr << "Material file contains unsupported header " << header << std::endl;
    }

    // Save previous material
    if(created_material) {
        if(ambient_set && difuse_set && specular_set && specular_exp_set && transparency_set)
            materials.insert({material_name, Material(material_name, ambient, difuse, specular, specular_exp, transparency)});
        else
            std::cerr << "There was an error loading material " << material_name << ". Some components were missing." << std::endl;
    }

    return materials;
}

ObjLoader::~ObjLoader() {

}

} // namespace maguey
