//
// Created by rdelfin on 1/8/18.
//

#include <maguey/material.hpp>

namespace maguey {

extern const std::string AMBIENT_COLOR_UNIFORM_NAME = "ambient_color";
extern const std::string DIFUSE_COLOR_UNIFORM_NAME = "difuse_color";
extern const std::string SPECULAR_COLOR_UNIFORM_NAME = "specular_color";
extern const std::string SPECULAR_EXPONENT_UNIFORM_NAME = "specular_exponent";
extern const std::string TRANSPARENCY_UNIFORM_NAME = "transparency";


Material::Material(std::string name, glm::vec3 ambient, glm::vec3 difuse,
                   glm::vec3 specular, float specular_exp,
                   float transparency)
    : _name(name), ambient(ambient), difuse(difuse), specular(specular),
      specular_exp(specular_exp), transparancy(transparency) {
    
}

std::function<const void*()> Material::ambient_color_data_source() {
    return [&]() -> const void* {
        return &this->ambient;
    };
}

std::function<const void*()> Material::difuse_color_data_source() {
    return [&]() -> const void* {
        return &this->difuse;
    };
}

std::function<const void*()> Material::specular_color_data_source() {
    return [&]() -> const void* {
        return &this->specular;
    };
}

std::function<const void*()> Material::specular_exponent_data_source() {
    return [&]() -> const void* {
        return &this->specular_exp;
    };
}

std::function<const void*()> Material::transparency_data_source() {
    return [&]() -> const void* {
        return &this->transparancy;
    };
}


const std::string& Material::name() { return this->_name; }

std::vector<ShaderUniform> Material::get_uniforms() {
    return { this->ambient_color_uniform(), this->difuse_color_uniform(),
             this->specular_color_uniform(), this->specular_exponent_uniform(),
             this->transparency_uniform() };
}

ShaderUniform Material::ambient_color_uniform() {
    return ShaderUniform(AMBIENT_COLOR_UNIFORM_NAME, BINDER_VECTOR3_F,
                         this->ambient_color_data_source());
}

ShaderUniform Material::difuse_color_uniform() {
    return ShaderUniform(DIFUSE_COLOR_UNIFORM_NAME, BINDER_VECTOR3_F,
                         this->difuse_color_data_source());
}

ShaderUniform Material::specular_color_uniform() {
    return ShaderUniform(SPECULAR_COLOR_UNIFORM_NAME, BINDER_VECTOR3_F,
                         this->specular_color_data_source());
}

ShaderUniform Material::specular_exponent_uniform() {
    return ShaderUniform(SPECULAR_EXPONENT_UNIFORM_NAME, BINDER_FLOAT,
                         this->specular_exponent_data_source());
}

ShaderUniform Material::transparency_uniform() {
    return ShaderUniform(TRANSPARENCY_UNIFORM_NAME, BINDER_FLOAT,
                         this->transparency_data_source());
}


bool Material::operator==(const Material& m) const {
    return this->_name             == m._name        &&
           this->ambient           == m.ambient      &&
           this->difuse            == m.difuse       &&
           this->specular          == m.specular     &&
           this->specular_exp      == m.specular_exp &&
           this->transparancy      == m.transparancy &&
           this->illumination_mode == m.illumination_mode;
}


Material::~Material() { }


} // namespace maguey