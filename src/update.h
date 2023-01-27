#ifndef update__H
#define update__H

#include "definitions.h"
#include "mouse.h"
#include "camera.h"
#include "player.h"
#include "building.h"
#include "turret.h"
#include "shot.h"
#include "enemy.h"
#include "collision.h"

void EvtUpdate(struct GameData* _gd);
void Update(struct GameData* _gd);

#endif