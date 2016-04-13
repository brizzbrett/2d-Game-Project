#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <SDL_mixer.h>

enum SGROUP	
{
	NULL_FX, 
	PLAYER_FX,
	ENEMY_FX,
	PROJECTILE_FX, 
	HIT_FX, 	
};

typedef struct Sound_S
{
	Mix_Chunk *sound;
	char filename[80];
	int used;
	float volume;
	int loop;
	int channel;
}Sound;

typedef struct Music_S
{
	Mix_Music *music;
	char filename[80];
	int used;
	float volume;
	int loop;
}Music;

void Audio_InitSystem(int sMax, int mMax);

void Audio_CloseAll();
void Music_Close();
void Sound_Close();

void Sound_Free(Sound **sound);
void Music_Free(Music **music);


Sound *Sound_New(char *filename, int loop, int channel);
Music *Music_New(char *filename, int loop);

void Sound_Player(Sound *sound);
void Music_Player(Music *music);

#endif