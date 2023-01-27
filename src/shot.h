#ifndef shot__H
#define shot__H

#include "definitions.h"
#include "tools.h"
#include "collision.h"

void CreateShot(struct GameData* _gd, sfVector2f _pos, int _range, float _angle, enum TypeTurret _type);
void UpdateShots(struct GameData* _gd);
void DisplayShots(struct GameData* _gd);

#endif