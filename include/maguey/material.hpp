//
// Created by rdelfin on 1/8/18.
//

#pragma once

#include <functional>
#include <string>

#include <glm/glm.hpp>

#include <maguey/program.hpp>

namespace maguey {

extern const std::string AMBIENT_COLOR_UNIFORM_NAME;
extern const std::string DIFUSE_COLOR_UNIFORM_NAME;
extern const std::string SPECULAR_COLOR_UNIFORM_NAME;
extern const std::string SPECULAR_EXPONENT_UNIFORM_NAME;
extern const std::string TRANSPARENCY_UNIFORM_NAME;

/**
 * Enum describing the different illumination modes available. These are
 * replicated from the Wavefront Material Template Library (MTL or .mtl) files.
 * For more information, please read:
 * https://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library
 */
enum IlluminationMode {
    ILLUMINATION_COLOR_ON_AMBIENT_OFF = 0,
    ILLUMINATION_COLOR_ON_AMBIENT_ON,
    ILLUMINATION_HIGHLIGHT_ON,
    ILLUMINATION_REFLECTION_ON_RAYTRACE_ON,
    ILLUMINATION_GLASS_ON_RAYTRACE_ON,
    ILLUMINATION_FRESNELL_ON_RAYTRACE_ON,
    ILLUMINATION_REFRACTION_ON_FRESNELL_OFF_RAYTRACE_ON,
    ILLUMINATION_REFRACTION_ON_FRESNELL_ON_RAYTRACE_ON,
    ILLUMINATION_REFLECTION_ON_RAYTRACE_OFF,
    ILLUMINATION_GLASS_ON_RAYTRACE_OFF,
    ILLUMINATION_CAST_SHADOWS
};

/**
 * This class describes an OpenGL material. These are used by the default
 * shaders for the Phong reflection model. For other reflection models, you
 * will need to write your own material classes or contribute to this project.
 * Since it contains data used by shaders and ShaderUniforms, copies of this
 * class will invalidate any ShaderUniforms created. Please refrain from doing
 * so.
 */
class Material {
public:
    /**
     * Main constructor for the material, defining the basic properties fo the
     * material in question.
     *
     * @param name         A string name representing the material. These
     *                     SHOULD (but are not required) to be unique.
     * @param ambient      RGB color of the ambient light on the material, as
     *                     defined by the Phong reflection model.
     * @param difuse       RGB difusion color of the material, as defined by
     *                     the Phong reflection model.
     * @param specular     RGB specular reflection color of the material, as
     *                     defined by the Phong reflection model.
     * @param specular_exp Exponent of the specular reflection, indicating
     *                     concentration of the specular reflection, as defined
     *                     by the Phong reflection model.
     * @param transparency Transparency of the material, with 0.0 being
     *                     completely opaque, and 1.0 being completely
     *                     transparent.
     */
    Material(std::string name, glm::vec3 ambient, glm::vec3 difuse,
             glm::vec3 specular, float specular_exp, float transparency);

    /**
     * Remove the copy constructor to avoid issues with undefined data sources.
     */
    Material(const Material&) = delete;

    /**
     * Disable the copy assignment operator to avoid issues with undefined data
     * sources.
     */
    Material& operator=(Material const&) = delete;

    /**
     * Returns the name of the material, as provided by the constructor.
     */
    const std::string& name();

    /**
     * Returns a list of all the uniforms defined by this material. This is
     * equivalent to putting the results of `ambient_color_uniform()`,
     * `difuse_color_uniform()`, `specular_color_uniform()`,
     * `specular_exponent_uniform()` and `transparency_uniform()`, and putting
     * them in a vector. This ordering is not guaranteed.
     */
    std::vector<ShaderUniform> get_uniforms();

    /**
     * Generates a new ShaderUniform object for the ambient color.
     */
    ShaderUniform ambient_color_uniform();

    /**
     * Generates a new ShaderUniform object for the difuse color.
     */
    ShaderUniform difuse_color_uniform();

    /**
     * Generates a new ShaderUniform object for the specular reflection color.
     */
    ShaderUniform specular_color_uniform();

    /**
     * Generates a new ShaderUniform object for the specular reflection
     * exponent.
     */
    ShaderUniform specular_exponent_uniform();
    
    /**
     * Generates a new ShaderUniform object for the transparency.
     */
    ShaderUniform transparency_uniform();

    ~Material();
private:
    std::function<const void*()> ambient_color_data_source();
    std::function<const void*()> difuse_color_data_source();
    std::function<const void*()> specular_color_data_source();
    std::function<const void*()> specular_exponent_data_source();
    std::function<const void*()> transparency_data_source();

    std::string _name;
    glm::vec3 ambient;
    glm::vec3 difuse;
    glm::vec3 specular;
    float specular_exp;
    float transparancy;
    IlluminationMode illumination_mode;
};

} // namespace maguey