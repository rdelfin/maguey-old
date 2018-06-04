//
// Created by rdelfin on 12/29/17.
//

#include <algorithm>
#include <unordered_set>

#include <glm/glm.hpp>
#include <gtest/gtest.h>

#include <maguey/obj_loader.hpp>
#include <maguey/camera.hpp>

std::string oneTrigObjFile =
R"(
v 1.0 0.0 0.0
v 0.0 1.0 0.0
v 0.0 0.0 1.0

vn 1.732050808 1.732050808 1.732050808
vn 1.732050808 1.732050808 1.732050808
vn 1.732050808 1.732050808 1.732050808

g g1
f 1 2 3
)";

std::string squareObjFile = 
R"(
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 0.0 1.0
v 1.0 0.0 1.0

vn 0.0 1.0 0.0
vn 0.0 1.0 0.0
vn 0.0 1.0 0.0
vn 0.0 1.0 0.0

g g1
f 1 2 3
f 2 4 3
)";

std::string oneTrigDashedObjFile = 
R"(
v 1.0 0.0 0.0
v 0.0 1.0 0.0
v 0.0 0.0 1.0

vn 1.732050808 1.732050808 1.732050808

g g1
f 1//1 2//1 3//1
)";

std::string squareDashedObjFile = 
R"(
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 0.0 1.0
v 1.0 0.0 1.0

vn 0.0 1.0 0.0

g g1
f 1//1 2//1 3//1
f 2//1 4//1 3//1
)";

std::string quadSimpleObjFile = 
R"(
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 1.0 0.0 1.0
v 0.0 0.0 1.0

vn 0.0 1.0 0.0
vn 0.0 1.0 0.0
vn 0.0 1.0 0.0
vn 0.0 1.0 0.0

g g1
f 1 2 3 4
)";

std::string quadDashedObjFile = 
R"(
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 1.0 0.0 1.0
v 0.0 0.0 1.0

vn 0.0 1.0 0.0

g g1
f 1//1 2//1 3//1 4//1
)";

void comparison_test(const std::string& file,
                     std::unordered_map<std::string, std::vector<glm::vec4>> true_verts,
                     std::unordered_map<std::string, std::vector<glm::vec4>> true_normals,
                     std::unordered_map<std::string, std::vector<glm::uvec3>> true_faces) {
    maguey::Camera camera;
    maguey::ObjLoader loader;
    bool error;

    std::unordered_map<std::string, maguey::TriangleMesh*> mesh_map = loader.loadString(file, error);
    ASSERT_FALSE(error);

    std::vector<std::string> mesh_keys(mesh_map.size()), true_vert_keys(true_verts.size()),
                             true_normal_keys(true_normals.size()), true_face_keys(true_faces.size());

    // Load group names into vectors
    std::transform(mesh_map.cbegin(), mesh_map.cend(), mesh_keys.begin(), [](const std::pair<std::string, maguey::TriangleMesh*>& pair) -> std::string { return pair.first; });
    std::transform(true_verts.cbegin(), true_verts.cend(), true_vert_keys.begin(), [](const std::pair<std::string, std::vector<glm::vec4>>& pair) -> std::string { return pair.first; });
    std::transform(true_normals.cbegin(), true_normals.cend(), true_normal_keys.begin(), [](const std::pair<std::string, std::vector<glm::vec4>>& pair) -> std::string { return pair.first; });
    std::transform(true_faces.cbegin(), true_faces.cend(), true_face_keys.begin(), [](const std::pair<std::string, std::vector<glm::uvec3>>& pair) -> std::string { return pair.first; });

    // Move vectors into sets
    std::unordered_set<std::string> mesh_key_set(mesh_keys.begin(), mesh_keys.end()),
                                    true_vert_keys_set(true_vert_keys.begin(), true_vert_keys.end()),
                                    true_normal_keys_set(true_normal_keys.begin(), true_normal_keys.end()),
                                    true_faces_keys_set(true_face_keys.begin(), true_face_keys.end());

    // Ensure the same groups are present in all sets
    ASSERT_EQ(mesh_key_set, true_vert_keys_set);
    ASSERT_EQ(mesh_key_set, true_normal_keys_set);
    ASSERT_EQ(mesh_key_set, true_faces_keys_set);

    // Compare each group's vertices, normals and faces
    for(std::pair<std::string, maguey::TriangleMesh*> p : mesh_map) {
        std::string group = p.first;

        ASSERT_EQ(p.second->getVertices(), true_verts[group]);
        ASSERT_EQ(p.second->getFaces(), true_faces[group]);
        ASSERT_EQ(p.second->getNormals(), true_normals[group]);
    }
}

// Basic object file test with one triangle, with one element faces
TEST(OneTriangleSimpleTest, ObjectLoaderTest) {
    std::unordered_map<std::string, std::vector<glm::vec4>> true_verts;
    std::unordered_map<std::string, std::vector<glm::vec4>> true_normals;
    std::unordered_map<std::string, std::vector<glm::uvec3>> true_faces;
    true_verts["g1"]   = {glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1)};
    true_normals["g1"] = {glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                          glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                          glm::vec4(1.732050808, 1.732050808, 1.732050808, 0)};
    true_faces["g1"]   = {glm::uvec3(0, 1, 2)};

    comparison_test(oneTrigObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a two-triangle square, with one element faces
TEST(SquareSimpleTest, ObjectLoaderTest) {
    std::unordered_map<std::string, std::vector<glm::vec4>> true_verts;
    std::unordered_map<std::string, std::vector<glm::vec4>> true_normals;
    std::unordered_map<std::string, std::vector<glm::uvec3>> true_faces;

    true_verts["g1"]   = { glm::vec4(0, 0, 0, 1), glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    true_normals["g1"] = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    true_faces["g1"]   = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(squareObjFile, true_verts, true_normals, true_faces);
}

// Basic object file test with one triangle, a single normal vector, and three element faces
TEST(TriangleOneNormalTest, ObjectLoaderTest) {
    std::unordered_map<std::string, std::vector<glm::vec4>> true_verts;
    std::unordered_map<std::string, std::vector<glm::vec4>> true_normals;
    std::unordered_map<std::string, std::vector<glm::uvec3>> true_faces;

    true_verts["g1"]   = {glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1)};
    true_normals["g1"] = {glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                          glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                          glm::vec4(1.732050808, 1.732050808, 1.732050808, 0)};
    true_faces["g1"]   = {glm::uvec3(0, 1, 2)};

    comparison_test(oneTrigDashedObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a two-triangle square, a single normal vector, and three element faces
TEST(SquareOneNormalTest, ObjectLoaderTest) {
    std::unordered_map<std::string, std::vector<glm::vec4>> true_verts;
    std::unordered_map<std::string, std::vector<glm::vec4>> true_normals;
    std::unordered_map<std::string, std::vector<glm::uvec3>> true_faces;

    true_verts["g1"]   = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    true_normals["g1"] = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    true_faces["g1"]   = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(squareDashedObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a single quad, and one element faces
TEST(SingleQuadTest, ObjectLoaderTest) {
    std::unordered_map<std::string, std::vector<glm::vec4>> true_verts;
    std::unordered_map<std::string, std::vector<glm::vec4>> true_normals;
    std::unordered_map<std::string, std::vector<glm::uvec3>> true_faces;

    true_verts["g1"]   = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    true_normals["g1"] = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    true_faces["g1"]   = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(quadSimpleObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a single quad, a single normal vector, and three element faces
TEST(SingleQuadOneNormalTest, ObjectLoaderTest) {
    std::unordered_map<std::string, std::vector<glm::vec4>> true_verts;
    std::unordered_map<std::string, std::vector<glm::vec4>> true_normals;
    std::unordered_map<std::string, std::vector<glm::uvec3>> true_faces;

    true_verts["g1"]   = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    true_normals["g1"] = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    true_faces["g1"]   = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(quadDashedObjFile, true_verts, true_normals, true_faces);
}