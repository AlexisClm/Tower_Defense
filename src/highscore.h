#ifndef highscore__H
#define highscore__H

#include "definitions.h"
#include "tools.h"

void ReadHighScores(struct GameData* _gd);
void SaveHighScore(struct GameData* _gd);
void DisplayHighScores(struct GameData* _gd);

#endif