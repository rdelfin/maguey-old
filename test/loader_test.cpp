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

std::string oneTrigDashedObjFile = 
R"(
v 1.0 0.0 0.0
v 0.0 1.0 0.0
v 0.0 0.0 1.0
vn 1.732050808 1.732050808 1.732050808
f 1//1 2//1 3//1
)";

std::string squareDashedObjFile = 
R"(
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 0.0 1.0
v 1.0 0.0 1.0

vn 0.0 1.0 0.0

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

f 1 2 3 4
)";

std::string quadDashedObjFile = 
R"(
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 1.0 0.0 1.0
v 0.0 0.0 1.0

vn 0.0 1.0 0.0

f 1//1 2//1 3//1 4//1
)";

void comparison_test(const std::string& file,
                     std::vector<glm::vec4> true_verts, std::vector<glm::vec4> true_normals,
                     std::vector<glm::uvec3> true_faces) {
    
    ObjLoader loader;
    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec3> faces;

    ASSERT_TRUE(loader.loadString(file, vertices, normals, faces));
    ASSERT_EQ(vertices, true_verts);
    ASSERT_EQ(normals, true_normals);
    ASSERT_EQ(faces, true_faces);
}

// Basic object file test with one triangle, with one element faces
TEST(OneTriangleSimpleTest, ObjectLoaderTest) {
    std::vector<glm::vec4> true_verts = {glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1)};
    std::vector<glm::vec4> true_normals = {glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0)};
    std::vector<glm::uvec3> true_faces = {glm::uvec3(0, 1, 2)};

    comparison_test(oneTrigObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a two-triangle square, with one element faces
TEST(SquareSimpleTest, ObjectLoaderTest) {
    std::vector<glm::vec4> true_verts = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    std::vector<glm::vec4> true_normals = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    std::vector<glm::uvec3> true_faces = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(squareObjFile, true_verts, true_normals, true_faces);
}

// Basic object file test with one triangle, a single normal vector, and three element faces
TEST(TriangleOneNormalTest, ObjectLoaderTest) {
    std::vector<glm::vec4> true_verts = {glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1)};
    std::vector<glm::vec4> true_normals = {glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0)};
    std::vector<glm::uvec3> true_faces = {glm::uvec3(0, 1, 2)};

    comparison_test(oneTrigDashedObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a two-triangle square, a single normal vector, and three element faces
TEST(SquareOneNormalTest, ObjectLoaderTest) {
    std::vector<glm::vec4> true_verts = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    std::vector<glm::vec4> true_normals = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    std::vector<glm::uvec3> true_faces = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(squareDashedObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a single quad, and one element faces
TEST(SingleQuadTest, ObjectLoaderTest) {
    std::vector<glm::vec4> true_verts = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    std::vector<glm::vec4> true_normals = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    std::vector<glm::uvec3> true_faces = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(quadSimpleObjFile, true_verts, true_normals, true_faces);
}

// Object file test with a single quad, a single normal vector, and three element faces
TEST(SingleQuadOneNormalTest, ObjectLoaderTest) {
    std::vector<glm::vec4> true_verts = { glm::vec4(0, 0, 0, 1),   glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 1) };
    std::vector<glm::vec4> true_normals = { glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0) };
    std::vector<glm::uvec3> true_faces = { glm::uvec3(0, 1, 2), glm::uvec3(3, 4, 5) };

    comparison_test(quadDashedObjFile, true_verts, true_normals, true_faces);
}