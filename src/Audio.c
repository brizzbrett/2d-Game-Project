#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio.h"
#include "simple_logger.h"
#include <SDL.h>

int numSounds = 0;
int soundMax = 0;
int numMusics = 0;
int musicMax = 0;
Sound *soundList = NULL;
Music *musicList = NULL;

void Sound_Free(Sound **sound)
{
	if(!sound)return;
	else if(!*sound)return;

	(*sound)->used--;

	if((*sound)->sound != NULL)
	{
		Mix_FreeChunk((*sound)->sound);
	}
	numSounds--;
	*sound = NULL;
}

void Music_Free(Music **music)
{
	if(!music)return;
	else if(!*music)return;

	(*music)->used--;

	if((*music)->music != NULL)
	{
		Mix_FreeMusic((*music)->music);
	}
	numMusics--;

	*music = NULL;
}

void Sound_Close()
{
	int i;
	Sound *sound;

	if(!soundList)return;

	for(i = 0; i < soundMax; i++)
	{
		sound = &soundList[i];
		Sound_Free(&sound);
	}
	soundMax = 0;
	free(soundList); 
	soundList = NULL;
}

void Music_Close()
{
	int i;
	Music *music;

	if(!musicList)return;

	for(i = 0; i < musicMax; i++)
	{
		music = &musicList[i];
		Music_Free(&music);
	}
	musicMax = 0;
	free(musicList); 
	musicList = NULL;
}

void Audio_CloseAll()
{	
	Sound_Close();
	Music_Close();
	Mix_CloseAudio();
}

void Audio_InitSystem(int sMax, int mMax)
{
	int flags=MIX_INIT_OGG;
	if(sMax <= 0 || mMax <= 0)return;

	if(Mix_Init(flags) != flags)return;

	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0)return;

	if(Mix_AllocateChannels(5) != 5)return;

	musicList = (Music *) malloc (sizeof (Music) * mMax);
	soundList = (Sound *) malloc (sizeof (Sound) * sMax);

	memset(musicList, 0, sizeof (Music) * mMax);
	memset(soundList, 0, sizeof (Sound) * sMax);

	musicMax = mMax;
	soundMax = sMax;
	
	atexit(Audio_CloseAll);
}

Sound *Sound_New(char *filename, int loop, int channel)
{
	int i;

	for(i = 0; i < soundMax; i++)
	{
		if(soundList[i].used)
		{
			continue;
		}
		memset(&soundList[i],0,sizeof(Sound));		
		soundList[i].used = 1;
		numSounds++;
		
		if(numSounds > soundMax)
		{
			slog("Maximum sounds reached");
			exit(1);
		}

		strcpy(soundList[i].filename, filename);
		soundList[i].channel = channel;
		soundList[i].loop = loop;
		soundList[i].sound = Mix_LoadWAV(filename);

		return &soundList[i];
	}
	return NULL;
}

Music *Music_New(char *filename, int loop)
{
	int i;

	for(i = 0; i < musicMax; i++)
	{
		if(musicList[i].used)
		{
			continue;
		}
		memset(&musicList[i],0,sizeof(Music));
		musicList[i].used = 1;
		numMusics++;

		if(numMusics > musicMax)
		{
			slog("Maximum musics reached");
			exit(1);
		}

		strcpy(musicList[i].filename, filename);
		musicList[i].music = Mix_LoadMUS(filename);
		musicList[i].loop = loop;
		return &musicList[i];
	}
	return NULL;
}

void Sound_Player(Sound *sound)
{
	Mix_PlayChannel(sound->channel, sound->sound, sound->loop);
}

void Music_Player(Music *music)
{
	Mix_PlayMusic(music->music, music->loop);
}

