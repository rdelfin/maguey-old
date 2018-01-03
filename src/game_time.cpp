#define GLM_FORCE_RADIANS

#include <maguey/game_time.hpp>
#include <chrono>

namespace maguey {

GameTime::GameTime() {
    reset();
}

GameTime::~GameTime() {
}

long double GameTime::getElapsedGameTime() { return start - currFrame; }
long double GameTime::getMillisecondsPerFrame() { return currFrame - lastFrame; }

void GameTime::update() {
    lastFrame = currFrame;
    currFrame = milliseconds_now();
}

long double GameTime::milliseconds_now() {
    
    std::chrono::milliseconds ms =
            std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::high_resolution_clock::now().time_since_epoch());
    return ms.count();
}

void GameTime::reset() {
    start = milliseconds_now();
    lastFrame = start;
    currFrame = start;
}

}  // namespace maguey