//
// Created by rdelfin on 7/2/17.
//

#define GLM_FORCE_RADIANS

#include <maguey/obj_loader.h>
#include <maguey/util.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

ObjLoader::ObjLoader() {

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
                break;
            }

            vertexRaw.push_back(glm::vec4(x, y, z, 1.0f));
        }

        else if(line[0] == 'v' && line[1] == 'n') {
            std::stringstream lineStream(line);

            std::string header;
            double x, y, z;

            if(!(lineStream >> header >> x >> y >> z)) {
                std::cerr << "Invalid vertex normal line: \"" << line << "\"" << std::endl;
                break;
            }

            normalRaw.push_back(glm::vec4(x, y, z, 0.0f));
        }

        else if(line[0] == 'f') {
            std::string header;

            std::vector<std::string> lineElems = split(line, ' ');

            if(lineElems.size() != 4) {
                std::cerr << "Invalid face line: \"" << line << "\"" << std::endl;
                break;
            }

            try {
                std::vector<std::string> xElems, yElems, zElems;
                xElems = split(lineElems[1], '/');
                yElems = split(lineElems[2], '/');
                zElems = split(lineElems[3], '/');
                if (xElems.size() == yElems.size() && yElems.size() == zElems.size()) {
                    if (xElems.size() == 1) {
                        unsigned long x, y, z;
                        x = std::stoul(xElems[0]);
                        y = std::stoul(yElems[0]);
                        z = std::stoul(zElems[0]);

                        facesRaw.push_back(glm::uvec3(z - 1, y - 1, x - 1));
                        normalIdx.push_back(glm::uvec3(z - 1, y - 1, x - 1));
                    } else if (xElems.size() == 3) {
                        unsigned long xFace, yFace, zFace, xNormal, yNormal, zNormal;
                        xFace = std::stoul(xElems[0]);
                        yFace = std::stoul(yElems[0]);
                        zFace = std::stoul(zElems[0]);
                        xNormal = std::stoul(xElems[2]);
                        yNormal = std::stoul(yElems[2]);
                        zNormal = std::stoul(zElems[2]);

                        facesRaw.push_back(glm::uvec3(xFace - 1, yFace - 1, zFace - 1));
                        normalIdx.push_back(glm::uvec3(xNormal - 1, yNormal - 1, zNormal - 1));
                    } else {
                        std::cerr << "Invalid face line. One coordinate has the wrong number of elements (either 1 or 3): \""
                                  << line << "\"" << std::endl;
                    }
                } else {
                    std::cerr
                            << "Invalid face line. The coordinates do not have the same number of elements (either 1 or 3): \""
                            << line << "\"" << std::endl;
                }
            } catch(std::invalid_argument e) {
                std::cerr << "There was an error reading an integer on a line. Error: \"" << e.what() << "\"" << std::endl
                          << "\t Line: \"" << line << "\"" << std::endl;
            } catch(std::out_of_range e) {
                std::cerr << "There was an error reading an integer on a line. Error: \"" << e.what() << "\""
                          << std::endl
                          << "\t Line: \"" << line << "\"" << std::endl;
            }
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
