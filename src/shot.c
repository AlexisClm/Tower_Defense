#include "shot.h"

void CreateShot(struct GameData* _gd, sfVector2f _pos, int _range, float _angle, enum TypeTurret _type)
{
	struct Shot* shot = (struct Shot*)calloc(sizeof(struct Shot), 1);

	shot->type = _type;
	shot->pos.x = _pos.x;
	shot->pos.y = _pos.y - _gd->turret[0].size.y / 4;
	shot->size.x = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.shot[_type])).x;
	shot->size.y = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.shot[_type])).y;
	shot->range = _range;
	shot->angle = _angle;
	shot->distMax = (float)(_range * CELL_SIZE) * (_range * CELL_SIZE);
	shot->posFrom.x = shot->pos.x;
	shot->posFrom.y = shot->pos.y;

	switch (_type)
	{
	case SHOT_LONG_RANGE:
		shot->dmg = RAND_MIN_MAX(10, 15);
		shot->speed = 300.f;
		break;

	case SHOT_SHORT_RANGE:
		shot->dmg = RAND_MIN_MAX(5, 10);
		shot->speed = 200.f;
		break;

	case SHOT_MISSILE:
		shot->dmg = 50;
		shot->speed = 100.f;
		break;

	case SHOT_ENEMY_SLOW:
	case SHOT_ENEMY_FAST:
		shot->dmg = RAND_MIN_MAX(1, 5);
		shot->speed = 300.f;
		break;

	default:
		break;
	}

	shot->next = _gd->shot;
	_gd->shot = shot;
}

void MoveShots(struct GameData* _gd, struct Shot* _shot)
{
	sfVector2f vector = { _shot->posFrom.x - _shot->pos.x , _shot->posFrom.y - _shot->pos.y };
	float actualDist = GetSquaredLength(vector);

	if (actualDist < _shot->distMax)
	{
		_shot->pos.x += _shot->speed * cosf(Deg2Rad(_shot->angle)) * _gd->syst.dt;
		_shot->pos.y += _shot->speed * sinf(Deg2Rad(_shot->angle)) * _gd->syst.dt;
	}
	else
	{
		if (_shot->type == SHOT_MISSILE)
		{
			CheckColMissileWithEnemies(_gd, _shot);
		}
		_shot->isKo = sfTrue;
	}
}

void DeleteShots(struct GameData* _gd)
{
	//Delete the first element of the list
	while (_gd->shot != NULL && _gd->shot->isKo)
	{
		struct Shot* nextShot = _gd->shot->next;
		free(_gd->shot);
		_gd->shot = nextShot;
	}

	//Delete in the middle of the list
	struct Shot* tempShot = _gd->shot;
	while (tempShot != NULL)
	{
		while (tempShot->next != NULL && tempShot->next->isKo) //Si élément suivant à supprimer
		{
			struct Shot* toDeleteShot = tempShot->next;
			tempShot->next = toDeleteShot->next;
			free(toDeleteShot);
		}

		tempShot = tempShot->next;
	}
}

void UpdateShots(struct GameData* _gd)
{
	struct Shot* shot = _gd->shot;

	while (shot != NULL)
	{
		if (!shot->isKo)
		{
			MoveShots(_gd, shot);
		}
		shot = shot->next;
	}
	DeleteShots(_gd);
}

void DisplayShots(struct GameData* _gd)
{
	struct Shot* shot = _gd->shot;

	while (shot != NULL)
	{
		if (!shot->isKo)
		{
			sfVector2f tempPos = PosWithCamera(shot->pos, _gd->cam.pos);
			BlitSprite(_gd->spr.shot[shot->type], tempPos, shot->angle, _gd->syst.window);
		}
		shot = shot->next;
	}
}