#include "turret.h"

#define OFFSET_X CELL_SIZE / 2

#define NB_SHOT_SHORT_RANGE 10
#define SHORT_RANGE_ANGLE 30

void SortTurrets(struct GameData* _gd)
{
	for (int k = 0; k < _gd->nb.turret; k++)
	{
		for (int i = 0; i < _gd->nb.turret - 1; i++)
		{
			if (_gd->turret[i].pos.y > _gd->turret[i + 1].pos.y)
			{
				struct Turret turret = _gd->turret[i];
				_gd->turret[i] = _gd->turret[i + 1];
				_gd->turret[i + 1] = turret;
				_gd->grid[_gd->turret[i].line][_gd->turret[i].column].isColliding = sfTrue;
			}
		}
	}
}

void CreateTurret(struct GameData* _gd, int _line, int _column, enum TypeTurret _type)
{
	if (_gd->nb.turret < NB_TURRET_MAX)
	{
		struct Turret* turret = &_gd->turret[_gd->nb.turret];

		turret->rect = sfRectangleShape_create();
		turret->type = _type;
		turret->size.x = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.turret[turret->type])).x;
		turret->size.y = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.turret[turret->type])).y;
		turret->line = _line;
		turret->column = _column;
		turret->pos.x = GridGetX(turret->column) + CELL_SIZE / 2;
		turret->pos.y = GridGetY(turret->line) + 1.5f;

		switch (turret->type)
		{
		case LONG_RANGE - 1:
			turret->range = 18;
			turret->hpMax = 500;
			turret->timerMax = 0.5;
			break;

		case SHORT_RANGE - 1:
			turret->range = 10;
			turret->hpMax = 500;
			turret->timerMax = 2;
			break;

		case MISSILE - 1:
			turret->range = 15;
			turret->hpMax = 350;
			turret->timerMax = 5;
			break;

		case WALL - 1:
			turret->hpMax = 600;
			break;
		default:
			break;
		}

		turret->hp = turret->hpMax;
		turret->timer = turret->timerMax;
		turret->distMax = (float)(turret->range * CELL_SIZE) * (turret->range * CELL_SIZE);
		turret->nearestDist = turret->distMax + 1;
		_gd->nb.turret++;

		SortTurrets(_gd);
		_gd->grid[turret->line][turret->column].isColliding = sfTrue;
	}
}

void GenericBuyTurret(struct Player* _player, int _value)
{
	if (_player->diamantum >= _value)
	{
		_player->diamantum -= _value;
		_player->canBuild = sfTrue;
	}
}

void BuyTurret(struct GameData* _gd)
{
	struct Player* player = &_gd->player;

	switch (player->selectedTurret)
	{
	case LONG_RANGE:
		GenericBuyTurret(player, DIAMANTUM_VALUE_TURRET_LONG_RANGE);
		break;

	case SHORT_RANGE:
		GenericBuyTurret(player, DIAMANTUM_VALUE_TURRET_SHORT_RANGE);
		break;

	case MISSILE:
		GenericBuyTurret(player, DIAMANTUM_VALUE_TURRET_MISSILE);
		break;

	case WALL:
		GenericBuyTurret(player, DIAMANTUM_VALUE_TURRET_WALL);
		break;
	default:
		break;
	}
}

void InputTurret(struct GameData* _gd)
{
	if (_gd->grid[_gd->mouse.line][_gd->mouse.column].isConstructible && _gd->player.selectedTurret >= 0)
	{
		BuyTurret(_gd);
		if (_gd->player.canBuild)
		{
			CreateTurret(_gd, _gd->mouse.line, _gd->mouse.column, _gd->player.selectedTurret - 1);
			_gd->player.canBuild = sfFalse;
		}
	}
}

void CheckNearestEnemy(struct GameData* _gd, struct Turret* _turret)
{
	_turret->nearestDist = _turret->distMax + 1;

	for (int i = 0; i < _gd->nb.enemy; i++)
	{
		struct Enemy* enemy = &_gd->enemy[i];
		sfVector2f dist = { enemy->pos.x - _turret->pos.x, enemy->pos.y - _turret->pos.y + _turret->size.y / 4 };
		float squaredDist = GetSquaredLength(dist);

		if (_turret->nearestDist > squaredDist)
		{
			_turret->posToShot = dist;
			_turret->nearestDist = squaredDist;
		}
	}
}

void UpdateFireRateTurrets(struct GameData* _gd, struct Turret* _turret)
{
	if (_turret->nearestDist < _turret->distMax)
	{
		_turret->timer += _gd->syst.dt;
		if (_turret->timer > _turret->timerMax)
		{
			switch (_turret->type)
			{
			case LONG_RANGE - 1:
			case MISSILE - 1:
				CreateShot(_gd, _turret->pos, _turret->range, Rad2Deg(atan2f(_turret->posToShot.y, _turret->posToShot.x)), _turret->type);
				break;

			case SHORT_RANGE - 1:
				for (int i = -5; i < NB_SHOT_SHORT_RANGE - 5; i++)
				{
					float angle = Rad2Deg(atan2f(_turret->posToShot.y, _turret->posToShot.x)) + i * SHORT_RANGE_ANGLE / NB_SHOT_SHORT_RANGE;
					CreateShot(_gd, _turret->pos, _turret->range, angle, _turret->type);
				}
				break;

			default:
				break;
			}
			_turret->timer = 0;
		}
	}
}

void DeleteTurrets(struct GameData* _gd)
{
	for (int i = _gd->nb.turret - 1; i >= 0; i--)
	{
		if (_gd->turret[i].hp <= 0)
		{
			_gd->grid[_gd->turret[i].line][_gd->turret[i].column].isColliding = sfFalse;
			_gd->turret[i] = _gd->turret[_gd->nb.turret - 1];
			_gd->nb.turret--;
		}
	}
}

void UpdateTurrets(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nb.turret; i++)
	{
		struct Turret* turret = &_gd->turret[i];

		CheckNearestEnemy(_gd, turret);
		UpdateFireRateTurrets(_gd, turret);
	}
	DeleteTurrets(_gd);
}

void DisplayTurrets(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nb.turret; i++)
	{
		struct Turret* turret = &_gd->turret[i];

		sfVector2f tempPos = PosWithCamera(turret->pos, _gd->cam.pos);
		BlitSprite(_gd->spr.turret[turret->type], tempPos, 0, _gd->syst.window);
	}
}