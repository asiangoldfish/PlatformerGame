#ifndef __TIMER_H__
#define __TIMER_H__

namespace Framework {
    /**
     * Measure time in milliseconds or seconds
     */
	class Timer {
	public:
        /**
         * Construct the Timer class
         */
		Timer();

        /**
         * Destruct the timer class
         */
		virtual ~Timer() = default;

		/**
		 * Run this function to update the Timer's logic.
		 *
		 * Hint: Call this function on every frame to get the most accurate
		 * result.
		 * @param timestep Time to update the total elapsed time by
		 */
		inline void update(const float timestep) { elapsedTime += timestep; }

        /**
         * Set the Timer's elapsed time to 0, effectively resetting it.
         */
		void resetTimer();

		/**
		 * Get time elapsed between the previous and the current frame.
		 * @return Time before previous and current frame.
		 */
		inline float getDeltaTime() const { return deltaTime; }

        /**
         * Get time elapsed since Timer was constructed or reset.
         * @return Time elapsed since constructed or reset.
         */
		inline float getElapsedTimeInSeconds() const { return elapsedTime; }

	private:
        /** Absolute time at last frame */
		float lastFrame;

        /** Time between current and last frame */
        float deltaTime;

        /** Time since constructed or reset */
        float elapsedTime = 0.0f;
	};
}

#endif // __TIMER_H__