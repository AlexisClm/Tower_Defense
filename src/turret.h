#ifndef turret__H
#define turret__H

#include "definitions.h"
#include "tools.h"
#include "shot.h"

void InputTurret(struct GameData* _gd);
void UpdateTurrets(struct GameData* _gd);
void DisplayTurrets(struct GameData* _gd);

#endif