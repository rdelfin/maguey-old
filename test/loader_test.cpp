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

template<typename T>
class vec_less_than {
public:
    bool operator()(const T& a, const T& b) const {
        for(size_t i = 0; i < a.size(); i++) {
            if(a[i] < b[i])
                return true;
            else if(a[i] > b[i])
                return false;
        }

        return false;
    }
};

// Basic object file test with one triangle, with one element faces
TEST(OneTriangleSimpleTest, ObjectLoaderTest) {
    ObjLoader loader;
    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec3> faces;

    std::set<glm::vec4, vec_less_than<glm::vec4> > true_verts = {glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1)};
    std::set<glm::vec4, vec_less_than<glm::vec4> > true_normals = {glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0),
                                           glm::vec4(1.732050808, 1.732050808, 1.732050808, 0)};
    std::set<glm::uvec3, vec_less_than<glm::uvec3>> true_faces = {glm::uvec3(0, 1, 2)};

    ASSERT_TRUE(loader.loadString(oneTrigObjFile, vertices, normals, faces));

    std::set<glm::vec4, vec_less_than<glm::vec4>> verts_set(vertices.begin(), vertices.end());
    std::set<glm::vec4, vec_less_than<glm::vec4>> normals_set(normals.begin(), normals.end());
    std::set<glm::uvec3, vec_less_than<glm::uvec3>> faces_set(faces.begin(), faces.end());

    ASSERT_EQ(verts_set, true_verts);
    ASSERT_EQ(normals_set, true_normals);
    ASSERT_EQ(faces_set, true_faces);
}
