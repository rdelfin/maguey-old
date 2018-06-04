//
// Created by rdelfin on 11/26/16.
//

#pragma once

#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <GL/glew.h>

namespace maguey {

typedef enum {
    BINDER_MATRIX4_F,
    BINDER_MATRIX3_F,
    BINDER_VECTOR4_F,
    BINDER_VECTOR3_F,
    BINDER_FLOAT
} BinderType;

/*
 * ShaderUniform: description of a uniform in a shader program.
 *      name: name
 *      binder: function to bind the uniform
 *      data_source: function to get the data for the uniform
 *
 * Lifted from CS 354 project 3
 */
class ShaderUniform {
public:
    ShaderUniform();
    ShaderUniform(std::string name, std::function<void(int, const void*)> binder, std::function<const void*()> data_source);
    ShaderUniform(std::string name, BinderType binderType, std::function<const void*()> data_source);

    ~ShaderUniform();

    std::string name;
    /*
     * binder
     *      argument 0: the location of the uniform
     *      argument 1: the data pointer returned by data_source function.
     */
    std::function<void(int, const void*)> binder;
    /*
     * data_source:
     *      return: the pointer to the uniform data
     *
     * Hint: DON'T DO THIS
     *       data_source = []() -> void* { float value = 1.0f; return &f; };
     *       the value variable will become invalid after returning from
     *       the lambda function
     */
    std::function<const void*()> data_source;
};

/**
 * Description of a OpenGL/GLSL shader program.
 */
struct Shader {
public:
    /**
     * Create an empty shader
     */
    Shader() { }

    /**
     * Creates a non-empty shader object
     * @param program String representing either the program or the path to the shader file
     * @param isFile True if program represents a file path, and false if it contains the program
     */
    Shader(const std::string& program, bool isFile = true) : program(program) {
        if(isFile) {
            // If file is true, `s` is a file name. Read in from file
            std::ifstream reader(program, std::ios_base::in);
            this->program = std::string(std::istreambuf_iterator<char>(reader),
                                        std::istreambuf_iterator<char>());
            reader.close();
        }
    }

    /**
     * Compiles the shader on the GPU.
     * 
     * @param type One of GL_VERTEX_SHADER, GL_GEOMETRY_SHADER or
     *             GL_FRAGMENT_SHADER.
     */
    void compile(int type);

    ~Shader() { }

    std::string program;
    int shader_id;
};


/**
 * Represents a complete GLSL program, with all 3 corresponding shaders, and
 * any appropriate shader uniforms. The program is generally associated with
 * one or more meshes.
 * 
 * Example:
 * ```
 * // During mesh initialization
 * std::vector<ShaderUniform> uniforms = ...something...
 * CHECK_GL_ERROR(glGenVertexArrays(1, &vao));
 * CHECK_GL_ERROR(glBindVertexArray(vao));
 * 
 * CHECK_GL_ERROR(glGenBuffers(3, vbo));
 * program = Program(uniforms, vertexShader, geometryShader, fragmentShader);
 * GLint programId = program.getProgramId();
 *
 * // Bind vertices, normals, attribures, the fragDataLocation, etc.
 * // CODE HERE
 *
 * program.link();
 *
 * // Add the index (aka the faces)
 * // CODE HERE
 *
 * // Add uniform locations
 * program.addUniformLocations();
 * 
 * 
 * 
 * // During render time:
 * CHECK_GL_ERROR(glBindVertexArray(vao));
 * program.use();
 * program.bindUniforms();
 * 
 * // Draw vertices
 * CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0));
 * ```
 * 
 * For the complete example, take a look at src/triangle_mesh.cpp
 */
class Program {
public:
    /**
     * Initializes an empty program
     */
    Program();

    /**
     * Main constructor for shader. Creates the shader with a program and a set
     * of uniforms. It will also compile the shaders.
     * @param uniforms A vector of uniforms to be passed into the shader
     * @param s        The string representing either a file name or the shader
     *                 program itself
     * @param file     If true, `s` represents a file name. Otherwise, `s`
     *                 represents a string with the program
     */
    Program(std::vector<ShaderUniform> uniforms,
            Shader vertexShader, Shader geometryShader, Shader fragmentShader);

    /**
     * Returns the ID for the shader program. This is generated when the
     * Program() object is created, at compilation..
     */
    GLint getProgramId() { return shaderProgram; }

    /**
     * Compiles the shader on the GPU. Run as soon after initialization as
     * possible. Lifted mostly from CS 354 project 3
     */
    void compile();

    /**
     * Adds the indeces for the uniforms to a list of locations by using
     * `glGetUniformLocation()`.
     */
    void addUniformLocations();

    /**
     * Binds the uniforms to their corresponding data sources (basically using
     * the `glUniform*()` functions.
     */
    void bindUniforms();

    /**
     * Runs the linker on the shader program.
     */
    void link();

    /**
     * Enables the shader program using `glUseProgram()`. This should run
     * before rendering the associated mesh.
     */
    void use();

    ~Program();
private:
    Shader vertexShader, geometryShader, fragmentShader;
    std::vector<ShaderUniform> uniforms;
    std::vector<GLint> uniformIds;
    GLint shaderProgram;
};

}  // namespace maguey
