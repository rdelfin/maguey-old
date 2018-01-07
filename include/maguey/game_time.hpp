//
// Created by rdelfin on 7/12/17.
//

#ifndef BOIDS_GAME_TIME_HPP
#define BOIDS_GAME_TIME_HPP

namespace maguey {

/**
 * Standard class used to keep track of time at the start of each frame. This
 * is meant to be updated at the start of every frame cycle, and used
 * throughout the frame. Please note that the `maguey::Game` class already
 * calls update() on GameTime, and you should *NOT* call it yourself.
 *
 * Example:
 * class MyGame {
 *     ...
 *     GameTime gameTime
 *     ...
 * }
 *
 * // Called every frame...
 * void MyGame::update() {
 *     gameTime.update();
 *     ...
 *     // To calculate linear motion...
 *     new_position = old_position + speed_in_pixels_per_second * gameTime.getMillisecondsPerFrame() / 1000.0f;
 *
 *     std::cout << (long)(gameTime.getElapsedGameTime() / 1000.0f) << "s have passed." << std::endl;
 * }
 */
class GameTime
{
public:
    GameTime();
    ~GameTime();

    /**
     * Update the counters to mark this as the start of a new frame. Will
     * update the values of `getMillisecondsPerFrame()` and
     * `getElapsedGameTime()`.
     */
    void update();

    /**
     * Gets the number of milliseconds between the last and current frame.
     */
    long double getMillisecondsPerFrame();

    /**
     * Gets the number of milliseconds since the start of the game and the
     * start of this frame.
     */
    long double getElapsedGameTime();

    /**
     * Gets the number of milliseconds since January 1st, 1970. Makes use of
     * std::chrono::system_clock::now(), which in most Unix systems is set at
     * UTC time.
     */
    static long double milliseconds_now();

    /**
     * Resets the start of the game, as well as the time for last and current
     * frame. Note that this will cause `getMillisecondsPerFrame()` to perform
     * a division by zero, so you must call update() before using said method.
     */
    void reset();

private:
    long double start;
    long double lastFrame;
    long double currFrame;

};

} // namespace maguey

#endif //BOIDS_GAME_TIME_HPP

