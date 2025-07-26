#pragma once

#include "pch.h"

namespace FW {
    /**
     * Measure time.
     *
     * @details Track time elapsed from certain place in code to another. The class can be instanced, so multiple timers can be
     * used to track time.
     *
     * @details By default, time is measured in milliseconds. When using <u>update()</u>, make sure to pass in number of
     * millimeters to increment time by.
     *
     * @example Get elapsed time during the game's runtime.
     * @code
     * void run() {
     *     Timer timer;
     *     while (true) {
     *         timer.update(timer.getDeltaTime());
     *         float elapsedTime = timer.getElapsedTimeInSeconds();
     *     }
     * }
     * @endcode
     *
     * @details The <u>Timer</u> supports getting delta time between frames.
     *
     * @example
     * @code
     * void run() {
     *     Timer timer;
     *     while true {
     *         timer.updateDeltaTime();
     *         float delta = timer.getDeltaTime();
     *     }
     * }
     * @endcode
     */
    class Timer
    {
    public:
        /**
         * Construct the Timer class
         */
        Timer() = default;

        /**
         * Destruct the timer class
         */
        virtual ~Timer() = default;

        /**
         * Update the timer's currently elapsed time.
         *
         * @details Call this function on every frame to get the most accurate result.
         *
         * @param milliseconds Milliseconds to increment <u>elapsedTime</u> by.
         */
        inline void update(const float milliseconds) { elapsedTime += milliseconds; }

        /**
         * Update the timer's delta time.
         *
         * @details This function is ideally called at the beginning of every frame.
         */
        void updateDeltaTime();

        /**
         * Get the time from the last frame to the current frame.
         */
        inline float getDeltaTime() { return deltaTime; }

        /**
         * Reset the <u>elapsedTime</u> to 0.
         */
        inline void resetTimer() { elapsedTime = 0.0f ;}

        /**
         * Get time elapsed since it was last reset in seconds.
         */
        inline float getElapsedTime() const { return elapsedTime; }

        /**
         * Get time elapsed since it was last reset in milliseconds.
         */
        inline float getElapsedTimeInMilliseconds() const { return elapsedTime * 1000.0f; }

    private:
        /** Time since constructed or reset */
        float elapsedTime = 0.0f;

        /** Absolute time at last frame */
        float lastFrame = 0.0f;

        /** Time between current and last frame */
        float deltaTime = 0.0f;
    };
} // FW
