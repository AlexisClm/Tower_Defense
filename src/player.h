#ifndef player__H
#define player__H

#include "definitions.h"
#include "tools.h"
#include "highscore.h"

void SelectTurret(struct GameData* _gd);
void InputPlayer(struct GameData* _gd);
void InitPlayer(struct GameData* _gd);
void UpdatePlayer(struct GameData* _gd);
void DisplayPlayer(struct GameData* _gd);

#endif