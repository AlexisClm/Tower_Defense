#include "camera.h"

void CenterPlayer(struct GameData* _gd, struct Camera* _cam)
{
	if (_gd->player.pos.x - SCREEN_W / 2 > GRID_OFFSET_X && _gd->player.pos.x - SCREEN_W / 2 < -GRID_OFFSET_X)
	{
		_cam->pos.x = _gd->player.pos.x - SCREEN_W / 2;
	}
	if (_gd->player.pos.y - SCREEN_H / 2 > GRID_OFFSET_Y && _gd->player.pos.y - SCREEN_H / 2 < -GRID_OFFSET_Y)
	{
		_cam->pos.y = _gd->player.pos.y - SCREEN_H / 2;
	}
}

void InitCamera(struct GameData* _gd)
{
	struct Camera* cam = &_gd->cam;

	cam->speed = 2500;//500;
	cam->isFocused = sfTrue;

	CenterPlayer(_gd, cam);
}

void MoveCamera(struct GameData* _gd, struct Camera* _cam)
{
	if (sfKeyboard_isKeyPressed(sfKeyZ) && _cam->pos.y > GRID_OFFSET_Y)
	{
		_cam->pos.y -= _cam->speed * _gd->syst.dt;
	}
	if (sfKeyboard_isKeyPressed(sfKeyS) && _cam->pos.y < -GRID_OFFSET_Y)
	{
		_cam->pos.y += _cam->speed * _gd->syst.dt;
	}
	if (sfKeyboard_isKeyPressed(sfKeyQ) && _cam->pos.x > GRID_OFFSET_X)
	{
		_cam->pos.x -= _cam->speed * _gd->syst.dt;
	}
	if (sfKeyboard_isKeyPressed(sfKeyD) && _cam->pos.x < -GRID_OFFSET_X)
	{
		_cam->pos.x += _cam->speed * _gd->syst.dt;
	}
}

void FocusCamera(struct GameData* _gd)
{
	struct Camera* cam = &_gd->cam;

	if (cam->isFocused)
	{
		cam->isFocused = sfFalse;
	}
	else
	{
		cam->isFocused = sfTrue;
	}
}

void UpdateCamera(struct GameData* _gd)
{
	struct Camera* cam = &_gd->cam;

	if (!cam->isFocused)
	{
		MoveCamera(_gd, cam);
	}
	else
	{
		CenterPlayer(_gd, cam);
	}
}