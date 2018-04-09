#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <gtest/gtest.h>

#include <maguey/material.hpp>
#include <maguey/obj_loader.hpp>


std::string mat_1 =
R"(
newmtl mat_1
Ka 1.000 0.000 0.500
Kd 0.500 1.000 0.000
Ks 0.000 0.500 1.000
Ns 5.0
d  0.9
illum 2
)";

std::string mat_2 =
R"(
newmtl mat_2
Ka 1.000 0.000 0.500
Kd 0.500 1.000 0.000
Ks 0.000 0.500 1.000
Ns 5.0
Tr  0.1
illum 2
)";

std::string multiple_mats =
R"(
newmtl mat_1
Ka 1.000 0.000 0.500
Kd 0.500 1.000 0.000
Ks 0.000 0.500 1.000
Ns 5.0
Tr  0.1
illum 2

newmtl mat_2
Ka 0.392 0.584 0.929
Kd 1.000 0.388 0.278
Ks 0.373 0.620 0.627
Ns 2.0
Tr  0.6
illum 1
)";

template<typename key_type, typename val_type>
std::unordered_set<key_type> get_keys(const std::unordered_map<key_type, val_type>& map) {
    std::unordered_set<key_type> result;
    for(const std::pair<key_type, val_type>& p : map)
        result.insert(p.first);

    return result;
}

void comparison_test(const std::string& file, std::unordered_map<std::string, maguey::Material*> true_material_map) {
    maguey::ObjLoader obj_loader;
    bool error;

    std::unordered_map<std::string, maguey::Material*> material_map = obj_loader.loadMaterialString(file, error);

    std::unordered_set<std::string> material_keys = get_keys(material_map), true_material_keys = get_keys(true_material_map);

    ASSERT_EQ(material_keys, true_material_keys);

    for(std::pair<std::string, maguey::Material*> p : material_map)
        ASSERT_EQ(*p.second, *true_material_map[p.first]);
}

TEST(materialOneTest, MaterialFileTest) {
    std::unordered_map<std::string, maguey::Material*> mats = {
            {
                "mat_1",
                new maguey::Material("mat_1", glm::vec3(1.000, 0.000, 0.500), glm::vec3(0.500, 1.000, 0.000), glm::vec3(0.000, 0.500, 1.000), 5.0, 0.1)
            }
        };
    comparison_test(mat_1, mats);
}

TEST(materialTwoTest, MaterialFileTest) {
    std::unordered_map<std::string, maguey::Material*> mats = {
            {
                "mat_2",
                new maguey::Material("mat_2", glm::vec3(1.000, 0.000, 0.500), glm::vec3(0.500, 1.000, 0.000), glm::vec3(0.000, 0.500, 1.000), 5.0, 0.1)
            }
        };
    comparison_test(mat_2, mats);
}

TEST(materialMultipleTest, MaterialFileTest) {
    std::unordered_map<std::string, maguey::Material*> mats = {
            {
                "mat_1",
                new maguey::Material("mat_1", glm::vec3(1.000, 0.000, 0.500), glm::vec3(0.500, 1.000, 0.000), glm::vec3(0.000, 0.500, 1.000), 5.0, 0.1)
            },
            {
                "mat_2",
                new maguey::Material("mat_2", glm::vec3(0.392, 0.584, 0.929), glm::vec3(1.000, 0.388, 0.278), glm::vec3(0.373, 0.620, 0.627), 2.0, 0.6)
            }
        };
    comparison_test(multiple_mats, mats);
}