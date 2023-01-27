#ifndef collision__H
#define collision__H

#include "definitions.h"
#include "tools.h"

void CheckColMissileWithEnemies(struct GameData* _gd, struct Shot* _shot);
void UpdateCollisions(struct GameData* _gd);

#endif