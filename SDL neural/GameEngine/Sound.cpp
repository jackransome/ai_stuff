#include "Sound.h"
#include <iostream>
namespace GameEngine {
	Sound::Sound() {}

	Sound::~Sound() {}

	//loads a sound and returns the index of the sound for future access
	int Sound::loadSound(char *path) {
		if (!(soundsArray[soundsArrayLength] = Mix_LoadMUS(path))) {
			std::cout << path << "not successfully loaded\n" << std::endl;
		}
		soundsArrayLength++;
		return soundsArrayLength - 1;
	}
	//play a sound previously loaded
	int Sound::playSound(int index) {
		if (soundsArray[index] == nullptr) {
			std::cout << "Invalid sound index\n" << std::endl;
			return 0;
		}
		return Mix_PlayMusic(soundsArray[index], 1);
	}
	//continuously loop a sound previously loaded
	int Sound::loopSound(int index) {
		if (soundsArray[index] == nullptr) {
			std::cout << "Invalid sound index\n" << std::endl;
			return 0;
		}
		return Mix_PlayMusic(soundsArray[index], -1);
	}
	//unload a sound previously loads
	void Sound::unloadSound(int index) {
		if (soundsArray[index] == nullptr) {
			std::cout << "Invalid sound index\n" << std::endl;
			return;
		}
		Mix_FreeMusic(soundsArray[index]);
		soundsArray[index] = nullptr;
	}
	void Sound::unloadAll() {
		for (int i = 0; i < soundsArrayLength; i++) {
			if (soundsArray[i]) {
				Mix_FreeMusic(soundsArray[i]);
			}
		}
	}
}