#ifndef enemy__H
#define enemy__H

#include "definitions.h"
#include "tools.h"
#include "shot.h"
#include "diamantum.h"

void InitWave(struct GameData* _gd);
void UpdateEnemies(struct GameData* _gd);
void DisplayEnemies(struct GameData* _gd);

#endif