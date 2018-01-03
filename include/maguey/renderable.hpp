//
// Created by rdelfin on 11/26/16.
//

#pragma once

#include <maguey/program.hpp>

namespace maguey {

class Renderable {
public:
    Renderable()  {

    }

    virtual void draw() = 0;

    virtual ~Renderable() {

    }
};

} // namespace maguey
