//
// Created by rdelfin on 7/12/17.
//

#ifndef BOIDS_GAME_TIME_HPP
#define BOIDS_GAME_TIME_HPP

namespace maguey {

class GameTime
{
public:
    GameTime();
    ~GameTime();

    void update();
    long double getMillisecondsPerFrame();
    long double getElapsedGameTime();

    static long double milliseconds_now();

    void reset();

private:
    long double start;
    long double lastFrame;
    long double currFrame;

};

} // namespace maguey

#endif //BOIDS_GAME_TIME_HPP

