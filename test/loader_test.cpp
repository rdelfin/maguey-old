//
// Created by rdelfin on 12/29/17.
//

#include <glm/glm.hpp>
#include <gtest/gtest.h>

#include <maguey/obj_loader.h>

std::string oneTrigObjFile =
R"(
v  1 0 0
v  0 1 0
v  0 0 1
vn 1.732050808 1.732050808 1.732050808
vn 1.732050808 1.732050808 1.732050808
vn 1.732050808 1.732050808 1.732050808
f  0 1 2
)";


TEST(OneTriangleTest, ObjectLoaderTest) {
    std::vector<glm::vec4> true_verts = {glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1)};
    std::vector<glm::vec4> true_normals = {glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0)};
    std::vector<glm::uvec3> true_faces = {glm::uvec3(0, 1, 2)};

    ObjLoader loader;
    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec3> faces;

    ASSERT_TRUE(loader.loadString(oneTrigObjFile, vertices, normals, faces));
    ASSERT_EQ(vertices, true_verts);
    ASSERT_EQ(normals, true_normals);
    ASSERT_EQ(faces, true_faces);
}