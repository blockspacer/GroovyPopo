#ifndef SOUND_H_
#define SOUND_H_

#include <nn/atk.h>
#include "SoundData.fsid"

typedef struct
{
	int soundID;

};

void InitSound();
void UninitSound();
void UpdateSound();


void PlayBGM(nn::atk::SoundArchive::ItemId soundId);
void StopBGM();
void SetVolumeBGM(float volume, int delayFrame=0);

void PlaySE(nn::atk::SoundArchive::ItemId soundId);

#endif