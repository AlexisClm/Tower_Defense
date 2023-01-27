#include "diamantum.h"

void CreateDiamantum(struct GameData* _gd, sfVector2f _pos)
{
	if (_gd->nb.diamantum < NB_DIAMANTUM_MAX)
	{
		struct Diamantum* diam = &_gd->diamantum[_gd->nb.diamantum];

		diam->size.x = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.diamantum)).x;
		diam->size.y = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.diamantum)).y;
		diam->pos = _pos;
		diam->speed = 300;
		diam->distMax = 100 * 100;

		_gd->nb.diamantum++;
	}
}

void MoveDiamantum(struct GameData* _gd, struct Diamantum* _diam)
{
	_diam->pos.x += _diam->speed * cosf(Deg2Rad(_diam->angle)) * _gd->syst.dt;
	_diam->pos.y += _diam->speed * sinf(Deg2Rad(_diam->angle)) * _gd->syst.dt;
}

void DeleteDiamantum(struct GameData* _gd, struct Diamantum* _diam)
{
	*_diam = _gd->diamantum[_gd->nb.diamantum - 1];
	_gd->nb.diamantum--;
}

void CheckDistDiamPlayer(struct GameData* _gd, struct Diamantum* _diam)
{
	sfVector2f dist = { _gd->player.pos.x - _diam->pos.x, _gd->player.pos.y - _diam->pos.y };
	float squaredDist = GetSquaredLength(dist);

	if (squaredDist < _diam->distMax)
	{
		_diam->angle = Rad2Deg(atan2f(dist.y, dist.x));
		MoveDiamantum(_gd, _diam);

		if (squaredDist < DIST_MIN)
		{
			_gd->player.diamantum += DIAMANTUM_VALUE_BONUS;
			DeleteDiamantum(_gd, _diam);
		}
	}
}

void UpdateDiamantum(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nb.diamantum; i++)
	{
		struct Diamantum* diam = &_gd->diamantum[i];

		CheckDistDiamPlayer(_gd, diam);
	}
}

void DisplayDiamantum(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nb.diamantum; i++)
	{
		struct Diamantum* diam = &_gd->diamantum[i];

		sfVector2f tempPos = PosWithCamera(diam->pos, _gd->cam.pos);
		BlitSprite(_gd->spr.diamantum, tempPos, 0, _gd->syst.window);
	}
}