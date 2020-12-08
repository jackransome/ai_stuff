#pragma once
#include <SDL_mixer.h>
#include <vector>
namespace GameEngine {
	//basic sound class, can load up to 100 sounds at once
	class Sound {
	public:
		Sound();
		~Sound();
		int loadSound(char *path);
		int playSound(int index);
		int loopSound(int index);
		void unloadSound(int index);
		void unloadAll();
	private:
		int soundsArrayLength;
		Mix_Music* soundsArray[100];
	};
}