#include "hud.h"

void DisplayHudEnemyLife(struct GameData* _gd, struct Enemy* _enemy)
{
	sfVector2f tempPos = PosWithCamera((sfVector2f) { _enemy->pos.x - _enemy->size.x / 2, _enemy->pos.y - 25 }, _gd->cam.pos);
	sfVector2f tempSize = { (_enemy->size.x) * ((float)_enemy->hp / _enemy->hpMax), 5 };

	sfRectangleShape_setFillColor(_enemy->rect, sfRed);
	BlitRectangle(_enemy->rect, tempPos, tempSize, _gd->syst.window);
}

void DisplayHudBuildingLife(struct GameData* _gd, struct Building* _building)
{
	if (_building->hp > 0)
	{
		sfVector2f tempPos = PosWithCamera((sfVector2f) { _building->pos.x - _building->size.x / 2, _building->pos.y - _building->size.y / 2 - CELL_SIZE }, _gd->cam.pos);
		sfVector2f tempSize = { (_building->size.x) * ((float)_building->hp / _building->hpMax), 15 };

		sfRectangleShape_setFillColor(_building->rect, sfGreen);
		BlitRectangle(_building->rect, tempPos, tempSize, _gd->syst.window);
	}
}

void DisplayHudTurretLife(struct GameData* _gd, struct Turret* _turret)
{
	sfVector2f tempPos = PosWithCamera((sfVector2f) { _turret->pos.x - CELL_SIZE / 2 + 1, _turret->pos.y - CELL_SIZE - 2 }, _gd->cam.pos);
	sfVector2f tempSize = { (_turret->size.x - 2) * ((float)_turret->hp / _turret->hpMax), 5 };

	sfRectangleShape_setFillColor(_turret->rect, sfGreen);
	BlitRectangle(_turret->rect, tempPos, tempSize, _gd->syst.window);
}

void DisplayPlayerLife(struct GameData* _gd)
{
	struct Player* player = &_gd->player;

	sfVector2f tempPos = PosWithCamera((sfVector2f) { player->pos.x - player->size.x / 2, player->pos.y - 30 }, _gd->cam.pos);
	sfVector2f tempSize = { (player->size.x) * ((float)player->hp / player->hpMax), 10 };

	sfRectangleShape_setFillColor(player->rect, sfGreen);
	BlitRectangle(player->rect, tempPos, tempSize, _gd->syst.window);
}

void DisplayHudLife(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nb.enemy; i++)
	{
		struct Enemy* enemy = &_gd->enemy[i];
		DisplayHudEnemyLife(_gd, enemy);
	}

	for (int i = 0; i < NB_TYPE_BUILDING; i++)
	{
		struct Building* building = &_gd->building[i];
		DisplayHudBuildingLife(_gd, building);
	}

	for (int i = 0; i < _gd->nb.turret; i++)
	{
		struct Turret* turret = &_gd->turret[i];
		DisplayHudTurretLife(_gd, turret);
	}

	DisplayPlayerLife(_gd);
}

void DisplayHudDiam(struct GameData* _gd)
{
	char bufferDiamantum[50];

	sprintf(bufferDiamantum, "X %d", _gd->player.diamantum);
	BlitSprite(_gd->spr.diamantum, (sfVector2f) { 10, 50 }, 0, _gd->syst.window);
	BlitText(_gd->hud.text, (sfVector2f) { 25, 25 }, bufferDiamantum, _gd->syst.window);
}

void DisplayHudTimerWave(struct GameData* _gd)
{
	char bufferTimer[50];

	sprintf(bufferTimer, "Wave in %d", (int)(_gd->wave.timerMax - _gd->wave.timer));
	BlitText(_gd->hud.text, (sfVector2f) { 10, 75 }, bufferTimer, _gd->syst.window);
}

void DisplayHudScore(struct GameData* _gd)
{
	char bufferScore[50];

	sprintf(bufferScore, "Score %d", _gd->player.score);
	BlitText(_gd->hud.text, (sfVector2f) { 10, 125 }, bufferScore, _gd->syst.window);
}

void DisplayHud(struct GameData* _gd)
{
	DisplayHudLife(_gd);
	DisplayHudDiam(_gd);
	DisplayHudTimerWave(_gd);
	DisplayHudScore(_gd);
}