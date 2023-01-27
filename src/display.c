#include "display.h"

void Display(struct GameData* _gd)
{
	sfRenderWindow_clear(_gd->syst.window, sfBlack);

	switch (_gd->syst.gameState)
	{
	case GAME:
		DisplayGrid(_gd);
		DisplayBuildings(_gd);
		DisplayTurrets(_gd);
		DisplayEnemies(_gd);
		DisplayPlayer(_gd);
		DisplayDiamantum(_gd);
		DisplayShots(_gd);
		DisplayHud(_gd);
		break;

	case HIGHSCORES:
		DisplayHighScores(_gd);
		break;

	default:
		break;
	}

	sfRenderWindow_display(_gd->syst.window);
}