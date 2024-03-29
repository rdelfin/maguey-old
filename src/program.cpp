//
// Created by rdelfin on 11/26/16.
//

#define GLM_FORCE_RADIANS

#include <functional>
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <maguey/debuggl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <maguey/program.hpp>


namespace maguey {

/*============================================================================================================
 *============================================ SHADER UNIFORM ================================================
 *============================================================================================================
 */
ShaderUniform::ShaderUniform() {

}

ShaderUniform::ShaderUniform(std::string name, std::function<void(int, const void*)> binder, std::function<const void*()> data_source)
    : name(name), binder(binder), data_source(data_source) {

}


ShaderUniform::ShaderUniform(std::string name, BinderType binderType, std::function<const void*()> data_source)
    : name(name), data_source(data_source) {
    // This switch sets up a couple of binders for uniforms: ie float, vector3, vector4, matrix3 and matrix4
    switch (binderType) {
        case BINDER_FLOAT:
            binder = [](int loc, const void* data) {
                glUniform1fv(loc, 1, (const GLfloat*) data);
            };
            break;
        case BINDER_VECTOR3_F:
            binder = [](int loc, const void* data) {
                glUniform3fv(loc, 1, (const GLfloat*) data);
            };
            break;
        case BINDER_VECTOR4_F:
            binder = [](int loc, const void* data) {
                glUniform4fv(loc, 1, (const GLfloat*) data);
            };
            break;
        case BINDER_MATRIX3_F:
            binder = [](int loc, const void* data) {
                glUniformMatrix3fv(loc, 1, GL_FALSE, (const GLfloat*) data);
            };
            break;
        case BINDER_MATRIX4_F:
            binder = [](int loc, const void* data) {
                glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*) data);
            };
            break;
    }
}

ShaderUniform::~ShaderUniform() { }

/*============================================================================================================
 *================================================ SHADER ====================================================
 *============================================================================================================
 */

void Shader::compile(int type) {
    GLuint shader_id = 0;
    CHECK_GL_ERROR(shader_id = glCreateShader(type));
#if 0
    std::cerr << __func__ << " shader id " << shader_id << " type " << type << "\tsource:\n" << program.c_str() << std::endl;
#endif
    const char* programPointer = program.c_str();
    CHECK_GL_ERROR(glShaderSource(shader_id, 1, &programPointer, nullptr));
    glCompileShader(shader_id);
    CHECK_GL_SHADER_ERROR(shader_id);
    this->shader_id = shader_id;
}


/*============================================================================================================
 *================================================ PROGRAM ===================================================
 *============================================================================================================
 */

Program::Program() {

}

Program::Program(std::vector<ShaderUniform> uniforms,
                 Shader vertexShader, Shader geometryShader,
                 Shader fragmentShader)
    : uniforms(uniforms), vertexShader(vertexShader),
      geometryShader(geometryShader), fragmentShader(fragmentShader) { }

void Program::addUniformLocations() {
    uniformIds.resize(uniforms.size());
    for(size_t i = 0; i < uniforms.size(); i++) {
        CHECK_GL_ERROR(uniformIds[i] = glGetUniformLocation(shaderProgram, uniforms[i].name.c_str()));
    }
}

void Program::link() {
    glLinkProgram(shaderProgram);
    CHECK_GL_PROGRAM_ERROR(shaderProgram);
}

void Program::use() {
    CHECK_GL_ERROR(glUseProgram(shaderProgram));
}

void Program::bindUniforms() {
    for (size_t i = 0; i < uniforms.size(); i++) {
        const auto& uni = uniforms[i];
        CHECK_GL_ERROR(uni.binder(uniformIds[i], uni.data_source()));
    }
}


void Program::compile() {
    vertexShader.compile(GL_VERTEX_SHADER);
    geometryShader.compile(GL_GEOMETRY_SHADER);
    fragmentShader.compile(GL_FRAGMENT_SHADER);
    CHECK_GL_ERROR(shaderProgram = glCreateProgram());
    glAttachShader(shaderProgram, vertexShader.shader_id);
    glAttachShader(shaderProgram, fragmentShader.shader_id);
    glAttachShader(shaderProgram, geometryShader.shader_id);
}

Program::~Program() {

}

} // namespace maguey
