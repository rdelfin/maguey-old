//
// Created by rdelfin on 7/23/17.
//

#ifndef BOIDS_CUBEMAP_HPP
#define BOIDS_CUBEMAP_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include <maguey/camera.hpp>
#include <maguey/program.hpp>
#include <maguey/renderable.hpp>

namespace maguey {

/**
 * A class representing a box surrounding the world where background textures
 * can be placed.
 */
class Skybox : public Renderable {
public:
    /**
     * Main constructor, creates an empty skybox. Load needs to be called next.
     */
    Skybox();

    /**
     * Loads in the textures for each of the faces of the skybox.
     * 
     * @param right  The file path to the image of the positive-x side of the
     *               cube
     * @param left   The file path to the image of the negative-x side of the
     *               cube
     * @param top    The file path to the image of the positive-y side of the
     *               cube
     * @param bottom The file path to the image of the negative-y side of the
     *               cube
     * @param back   The file path to the image of the positive-z side of the
     *               cube
     * @param front  The file path to the image of the negative-z side of the
     *               cube
     * @param c      The camera object associated with this view.
     */
    void load(const std::string& right, const std::string& left,
            const std::string& top, const std::string& bottom,
            const std::string& back, const std::string& front,
            Camera& c);

    /**
     * Renders the skybox on the screen.
     */
    virtual void draw() override;

    ~Skybox();
private:
    GLuint textureId;
    Program program;

    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;

    std::vector<unsigned> vbo;
    unsigned vao;


    void generateCube();

    static const std::string VERTEX_VBO_NAME;
    static const std::string FRAGMENT_COLOR_NAME;
    static const std::string NORMAL_VBO_NAME;
};

} // namespace maguey

#endif //BOIDS_CUBEMAP_HPP
