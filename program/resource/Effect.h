#ifndef EFFECT_H_
#define EFFECT_H_

#include"main.h"

void EffectINIT(void);
void EffectUPDATE(void);
void EffectDRAW(void);
void EffectUNINIT(void);

//爆発エフェクト
void SetExplosion(Float2 pos);

//花火エフェクト
void SetHeart(Float2 pos);

//ゲームオーバー時に上から青いガーンが降りてくるエフェクト
void SetGameOver(void);

//クリアの文字
void SetClear(void);

bool GetIsClear(void);

bool GetIsGameover(void);

void SetFireWorks(void);

void SetSyabonBreak(Float2 pos);

void SetPrincess(Float2 pos);

void SetFire(Float2 pos);


#endif