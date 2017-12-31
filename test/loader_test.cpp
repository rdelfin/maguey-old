//
// Created by rdelfin on 12/29/17.
//

#include <set>

#include <glm/glm.hpp>
#include <gtest/gtest.h>

#include <maguey/obj_loader.h>

std::string oneTrigObjFile =
R"(
v 1.0 0.0 0.0
v 0.0 1.0 0.0
v 0.0 0.0 1.0
vn 1.732050808 1.732050808 1.732050808
vn 1.732050808 1.732050808 1.732050808
vn 1.732050808 1.732050808 1.732050808
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

f 1 2 3
f 2 4 3
)";

// Basic object file test with one triangle, with one element faces
TEST(OneTriangleSimpleTest, ObjectLoaderTest) {
    ObjLoader loader;
    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec3> faces;
    std::vector<glm::vec4> true_verts = {glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1)};
    std::vector<glm::vec4> true_normals = {glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0)};
    std::vector<glm::uvec3> true_faces = {glm::uvec3(0, 1, 2)};

    ASSERT_TRUE(loader.loadString(oneTrigObjFile, vertices, normals, faces));

    ASSERT_EQ(vertices, true_verts);
    ASSERT_EQ(normals, true_normals);
    ASSERT_EQ(faces, true_faces);
}

// Object file test with a two-triangle square, with one element faces
TEST(SquareSimpleTest, ObjectLoaderTest) {
    ObjLoader loader;
    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec3> faces;
    std::vector<glm::vec4> true_verts = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    std::vector<glm::vec4> true_normals = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    std::vector<glm::uvec3> true_faces = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    ASSERT_TRUE(loader.loadString(squareObjFile, vertices, normals, faces));
    
    ASSERT_EQ(vertices, true_verts);
    ASSERT_EQ(normals, true_normals);
    ASSERT_EQ(faces, true_faces);
}
