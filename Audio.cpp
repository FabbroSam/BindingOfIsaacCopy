// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Audio.h"
#include "SDL.h"
#include "fileUtils.h"
#include <iostream>

using namespace agp;

Audio* Audio::instance()
{
	static Audio uniqueInstance;
	return &uniqueInstance;
}

Audio::Audio()
{
	if (SDL_Init(SDL_INIT_AUDIO))
		throw SDL_GetError();

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512))
		throw Mix_GetError();

	setVolumeSfx(3);
	_volumeSfx = 3;
	setVolumeMusic (2);
	_volumeMusic = 2;

	_stop = false;


	auto soundFiles = getFilesInDirectory("../sounds");
	for (auto& f : soundFiles)
	{
		std::string name = getFileName(f, false);

		Mix_Chunk* chunk = Mix_LoadWAV(("../sounds/" + f).c_str());
		if (!chunk)
			std::cerr << Mix_GetError() << "\n";
		else
			_sounds[name] = chunk;
	}

	auto musicFiles = getFilesInDirectory("../musics");
	for (auto& f : musicFiles)
	{
		std::string name = getFileName(f, false);

		Mix_Music* music = Mix_LoadMUS(("../musics/" + f).c_str());
		if (!music)
			std::cerr << Mix_GetError() << "\n";
		else
			_musics[name] = music;
	}
}

Audio::~Audio()
{
	for (auto& entry : _musics)
		Mix_FreeMusic(entry.second);
	for (auto& entry : _sounds)
		Mix_FreeChunk(entry.second);

	Mix_CloseAudio();
}

void Audio::playSound(const std::string& id, int loops)
{
	if (_sounds.find(id) == _sounds.end())
	{
		std::cerr << "Cannot find sound \"" << id << "\"\n";
		return;
	}

	Mix_PlayChannel(-1, _sounds[id], loops);
}

void Audio::playMusic(const std::string& id, bool enqueue, int loops)
{
	if (_musics.find(id) == _musics.end())
	{
		std::cerr << "Cannot find music \"" << id << "\"\n";
		return;
	}

	if (!enqueue)
	{
		Mix_PlayMusic(_musics[id], loops);
		Mix_HookMusicFinished(onMusicFinished);
	}
	else
		_musicQueue.push_back(id);

}

void Audio::onMusicFinished()
{
	Audio* instance = Audio::instance();
	if (!instance->_musicQueue.empty())
	{
		std::string nextMusic = instance->_musicQueue.front();
		instance->_musicQueue.pop_front();


		Mix_PlayMusic(instance->_musics[nextMusic], -1); //la musica riprodotta in coda avrà un loop infinito, metodo utile solo per una canzone in loop
		Mix_HookMusicFinished(onMusicFinished);			 // Per avere una coda più lunga bisogna creare una nuova funzione "enqueueMusic" che aggiunge alla coda
	}													// e in questa funzione aggiunge un "if coda != 0: riproduci canzone successiva" 
}


void Audio::resumeMusic()
{
	Mix_ResumeMusic();
}

void Audio::pauseMusic()
{
	Mix_PauseMusic();
}

void Audio::haltMusic()
{
	Mix_HaltMusic();
}

void Audio::setVolumeSfx(int volume)
{ 
	Mix_Volume(-1, volume); 
}

void Audio::upVolumeSfx()
{
	if (_volumeSfx < 10)
	{
		_volumeSfx += 1;
		setVolumeSfx(_volumeSfx * 10);
	}
}

void Audio::downVolumeSfx()
{
	if (_volumeSfx > 0)
	{
		_volumeSfx -= 1;
		setVolumeSfx(_volumeSfx * 10);
	}
}

void Audio::setVolumeMusic(int volume)
{
	Mix_VolumeMusic(volume);
}

void Audio::upVolumeMusic()
{
	if (_volumeMusic < 10)
	{
		_volumeMusic += 1;
		setVolumeMusic(_volumeMusic * 10);
	}
}

void Audio::downVolumeMusic()
{
	if (_volumeMusic > 0)
	{
		_volumeMusic -= 1;
		setVolumeMusic(_volumeMusic * 10);
	}
}