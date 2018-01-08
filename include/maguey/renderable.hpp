//
// Created by rdelfin on 11/26/16.
//

#pragma once

#include <maguey/program.hpp>

namespace maguey {

/**
 * Interface representing a basic renderable object, such as a triangle mesh or
 * a geometric shape.
 */
class Renderable {
public:
    Renderable()  {

    }

    /**
     * Pure virtual function to be called to render the object on the screen.
     */
    virtual void draw() = 0;

    virtual ~Renderable() {

    }
};

} // namespace maguey
