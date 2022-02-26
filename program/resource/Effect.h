#ifndef EFFECT_H_
#define EFFECT_H_

#include"main.h"

void EffectINIT(void);
void EffectUPDATE(void);
void EffectDRAW(void);
void EffectUNINIT(void);

//�����G�t�F�N�g
void SetExplosion(Float2 pos);

//�ԉ΃G�t�F�N�g
void SetHeart(Float2 pos);

//�Q�[���I�[�o�[���ɏォ����K�[�����~��Ă���G�t�F�N�g
void SetGameOver(void);

//�N���A�̕���
void SetClear(void);

bool GetIsClear(void);

bool GetIsGameover(void);

void SetFireWorks(void);

void SetSyabonBreak(Float2 pos);

void SetPrincess(Float2 pos);

void SetFire(Float2 pos);


#endif