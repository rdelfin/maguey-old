//
// Created by rdelfin on 7/2/17.
//

#define GLM_FORCE_RADIANS

#include <maguey/obj_loader.h>
#include <maguey/util.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>


ObjLoader::ObjLoader() {

}

std::vector<std::vector<uint>> ObjLoader::parse_line_elems(const std::vector<std::string>& line_elems) const {
    std::vector<std::vector<uint>> result;
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

bool ObjLoader::process_face(const std::string& line, std::vector<glm::uvec3>& facesRaw, std::vector<glm::uvec3>& normalIdx) const {
    std::string header;

    std::vector<std::string> lineElems = split(line, ' ');
    lineElems = std::vector<std::string>(lineElems.begin() + 1, lineElems.end());

    if(lineElems.size() != 3 && lineElems.size() != 4) {
        std::cerr << "Invalid face line: \"" << line << "\". Should have 3 or 4 elements instead of " << lineElems.size() << std::endl;
        return false;
    }

    std::vector<std::vector<uint>> vals = parse_line_elems(lineElems);

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
            facesRaw.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[2][0] - 1));
            normalIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[2][0] - 1));
        } else if (vals[0].size() == 3) {
            facesRaw.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[2][0] - 1));
            normalIdx.push_back(glm::uvec3(vals[0][2] - 1, vals[1][2] - 1, vals[2][2] - 1));
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
            facesRaw.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[3][0] - 1));
            normalIdx.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[3][0] - 1));

            // Second triangle (2nd, 3rd and 4th vertices)
            facesRaw.push_back(glm::uvec3(vals[1][0] - 1, vals[2][0] - 1, vals[3][0] - 1));
            normalIdx.push_back(glm::uvec3(vals[1][0] - 1, vals[2][0] - 1, vals[3][0] - 1));
        } else if (vals[0].size() == 3) {
            // First triangle (1st, 2nd and 4th vertices)
            facesRaw.push_back(glm::uvec3(vals[0][0] - 1, vals[1][0] - 1, vals[3][0] - 1));
            normalIdx.push_back(glm::uvec3(vals[0][2] - 1, vals[1][2] - 1, vals[3][2] - 1));

            // Second triangle (2nd, 3rd and 4th vertices)
            facesRaw.push_back(glm::uvec3(vals[1][0] - 1, vals[2][0] - 1, vals[3][0] - 1));
            normalIdx.push_back(glm::uvec3(vals[1][2] - 1, vals[2][2] - 1, vals[3][2] - 1));
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

bool ObjLoader::loadString(const std::string &contents,
                           std::vector<glm::vec4> &vertices, std::vector<glm::vec4> &normals,
                           std::vector<glm::uvec3> &faces) const {
    // Clear out all vectors to keep at same length
    vertices.clear();
    normals.clear();
    faces.clear();

    std::vector<glm::uvec3> facesRaw;
    std::vector<glm::vec4> vertexRaw;
    std::vector<glm::vec4> normalRaw;
    std::vector<glm::uvec3> normalIdx;

    std::stringstream ss(contents);
    std::string line;

    while(std::getline(ss, line)) {
        if(line[0] == 'v' && line[1] == ' ') {
            std::stringstream lineStream(line);

            std::string header;
            double x, y, z;

            if(!(lineStream >> header >> x >> y >> z)) {
                std::cerr << "Invalid vertex line: \"" << line << "\"" << std::endl;
                return false;
            }

            vertexRaw.push_back(glm::vec4(x, y, z, 1.0f));
        }

        else if(line[0] == 'v' && line[1] == 'n') {
            std::stringstream lineStream(line);

            std::string header;
            double x, y, z;

            if(!(lineStream >> header >> x >> y >> z)) {
                std::cerr << "Invalid vertex normal line: \"" << line << "\"" << std::endl;
                return false;
            }

            normalRaw.push_back(glm::vec4(x, y, z, 0.0f));
        }

        else if(line[0] == 'f') {
            if(!process_face(line, facesRaw, normalIdx))
                return false;
        }
    }

    for(size_t i = 0; i < facesRaw.size(); i++) {
        size_t start_vertices = vertices.size();
        glm::uvec3 face = facesRaw[i];
        glm::uvec3 normal = normalIdx[i];

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

    return true;
}

ObjLoader::~ObjLoader() {

}
