#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class Timer
{
    public:
		Timer();

		void start();
		void stop();
		void pause();
		void unpause();

		Uint32 getTicks();

		bool isStarted();
		bool isPaused();

    private:
		Uint32 startTicks;
		Uint32 pausedTicks;

		bool paused;
		bool started;
};

#endif // TIMER_H