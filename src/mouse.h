#ifndef mouse__H
#define mouse__H

#include "definitions.h"
#include "player.h"
#include "turret.h"
#include "camera.h"

void InputKeyboard(struct GameData* _gd);
void InputMouse(struct GameData* _gd);
void UpdateMouse(struct GameData* _gd);

#endif