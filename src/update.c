#include "update.h"

void EvtUpdate(struct GameData* _gd)
{
	InputMouse(_gd);
	InputKeyboard(_gd);
}

void Update(struct GameData* _gd)
{
	_gd->syst.dt = sfTime_asSeconds(sfClock_restart(_gd->syst.dtClock));

	if (_gd->syst.gameState == GAME)
	{
		UpdateMouse(_gd);
		UpdatePlayer(_gd);
		UpdateEnemies(_gd);
		UpdateCamera(_gd);
		UpdateBuildings(_gd);
		UpdateTurrets(_gd);
		UpdateShots(_gd);
		UpdateDiamantum(_gd);
		UpdateCollisions(_gd);
	}
}
