#include "collision.h"

void CheckColMissileWithEnemies(struct GameData* _gd, struct Shot* _shot)
{
	for (int i = 0; i < _gd->nb.enemy; i++)
	{
		struct Enemy* enemy = &_gd->enemy[i];

		sfVector2f dist = { enemy->pos.x - _shot->pos.x, enemy->pos.y - _shot->pos.y };
		float squaredDist = GetSquaredLength(dist);

		if (squaredDist < RANGE_EXPLODE_MAX)
		{
			enemy->hp -= _shot->dmg;
		}
	}
}

void CheckColShotWithEnemies(struct GameData* _gd, struct Shot* _shot)
{
	for (int i = 0; i < _gd->nb.enemy; i++)
	{
		struct Enemy* enemy = &_gd->enemy[i];

		if (IsCollidingRectRectCentered(_shot->pos, _shot->size, enemy->pos, enemy->size))
		{
			if (_shot->type == SHOT_MISSILE)
			{
				CheckColMissileWithEnemies(_gd, _shot);
			}
			else
			{
				enemy->hp -= _shot->dmg;
			}
			_shot->isKo = sfTrue;
		}
	}
}

void CheckColShotWithPlayer(struct Shot* _shot, struct Player* _player)
{
	if (IsCollidingRectRectCentered(_shot->pos, _shot->size, _player->pos, _player->size))
	{
		_shot->isKo = sfTrue;
		_player->hp -= _shot->dmg;
	}
}

void CheckColShotWithTurrets(struct GameData* _gd, struct Shot* _shot)
{
	for (int i = 0; i < _gd->nb.turret; i++)
	{
		struct Turret* turret = &_gd->turret[i];

		if (IsCollidingRectRectCentered(_shot->pos, _shot->size, turret->pos, turret->size))
		{
			_shot->isKo = sfTrue;
			turret->hp -= _shot->dmg;
		}
	}
}

void CheckColShotWithBuildings(struct GameData* _gd, struct Shot* _shot)
{
	for (int l = GridGetLine(_shot->pos.y) - 1; l <= GridGetLine(_shot->pos.y) + 1; l++)
	{
		for (int c = GridGetColumn(_shot->pos.x) - 1; c <= GridGetColumn(_shot->pos.x) + 1; c++)
		{
			struct Grid* cell = &_gd->grid[l][c];
			sfVector2f cellPos = { cell->pos.x + CELL_SIZE / 2, cell->pos.y + CELL_SIZE / 2 };

			if (cell->isBuilding && IsCollidingRectRectCentered(_shot->pos, _shot->size, cellPos, cell->size))
			{
				_shot->isKo = sfTrue;
			}
		}
	}

	if (_shot->type == SHOT_ENEMY_SLOW || _shot->type == SHOT_ENEMY_FAST)
	{
		for (int i = 0; i < NB_TYPE_BUILDING; i++)
		{
			struct Building* building = &_gd->building[i];

			if (IsCollidingRectRectCentered(_shot->pos, _shot->size, building->pos, building->size))
			{
				building->hp -= _shot->dmg;
			}
		}
	}
}

void CheckColShots(struct GameData* _gd)
{
	struct Shot* shot = _gd->shot;
	struct Player* player = &_gd->player;

	while (shot != NULL)
	{
		if (!shot->isKo)
		{
			switch (shot->type)
			{
			case SHOT_LONG_RANGE:
			case SHOT_SHORT_RANGE:
				CheckColShotWithEnemies(_gd, shot);
				CheckColShotWithBuildings(_gd, shot);
				break;

			case SHOT_MISSILE:
				CheckColShotWithEnemies(_gd, shot);
				break;

			case SHOT_ENEMY_SLOW:
				CheckColShotWithPlayer(shot, player);
				CheckColShotWithBuildings(_gd, shot);
				CheckColShotWithTurrets(_gd, shot);
				break;

			case SHOT_ENEMY_FAST:
				CheckColShotWithPlayer(shot, player);
				CheckColShotWithBuildings(_gd, shot);
				CheckColShotWithTurrets(_gd, shot);
				break;
			default:
				break;
			}

		}
		shot = shot->next;
	}
}

void UpdateCollisions(struct GameData* _gd)
{
	CheckColShots(_gd);
}