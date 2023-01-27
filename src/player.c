#include "player.h"

void UpdatePlayerDir(struct Player* _player, float _limAngle1, float _limAngle2, enum Direction _dir)
{
	if (_dir == LEFT)
	{
		if (_player->angle > _limAngle1 || _player->angle <= _limAngle2)
		{
			_player->dir = _dir;
		}
	}
	else
	{
		if (_player->angle > _limAngle1 && _player->angle <= _limAngle2)
		{
			_player->dir = _dir;
		}
	}
}

void InputPlayer(struct GameData* _gd)
{
	struct Player* player = &_gd->player;
	sfVector2f tempPos = { _gd->mouse.worldPos.x - player->pos.x, _gd->mouse.worldPos.y - player->pos.y };

	player->angle = Rad2Deg(atan2f(tempPos.y, tempPos.x));
	player->posToGo = (sfVector2f){ (float)_gd->mouse.worldPos.x, (float)_gd->mouse.worldPos.y };
	player->isMoving = sfTrue;

	UpdatePlayerDir(player, -135.f, -45.f, UP);
	UpdatePlayerDir(player, -45.f, 45.f, RIGHT);
	UpdatePlayerDir(player, 45.f, 135.f, DOWN);
	UpdatePlayerDir(player, 135.f, -135.f, LEFT);
}

void InitPlayer(struct GameData* _gd)
{
	struct Player* player = &_gd->player;

	player->rect = sfRectangleShape_create();
	player->size.x = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.player[DOWN])).x;
	player->size.y = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.player[DOWN])).y;
	player->line = RAND_MIN_MAX(BUILDING_LINE_MIN, BUILDING_LINE_MAX - 1);
	player->column = RAND_MIN_MAX(BUILDING_COLUMN_MIN, BUILDING_COLUMN_MAX - 1);
	player->hpMax = 100;
	player->hp = player->hpMax;
	player->diamantum = 300;
	player->score = 0;

	while (_gd->grid[player->line][player->column].isColliding)
	{
		player->line = RAND_MIN_MAX(BUILDING_LINE_MIN, BUILDING_LINE_MAX - 1);
		player->column = RAND_MIN_MAX(BUILDING_COLUMN_MIN, BUILDING_COLUMN_MAX - 1);
	}

	player->pos.x = GridGetX(player->column) + player->size.x / 2;
	player->pos.y = GridGetY(player->line) + player->size.y / 2;
	player->speed = 200;
	player->isMoving = sfFalse;
	player->dir = DOWN;
	player->selectedTurret = 0;
}

void SelectTurret(struct GameData* _gd)
{
	struct Player* player = &_gd->player;

	switch (_gd->syst.event.key.code)
	{
	case sfKeyNum1:
		player->selectedTurret = LONG_RANGE;
		break;
	case sfKeyNum2:
		player->selectedTurret = SHORT_RANGE;
		break;
	case sfKeyNum3:
		player->selectedTurret = MISSILE;
		break;
	case sfKeyNum4:
		player->selectedTurret = WALL;
		break;
	default:
		break;
	}
}

void MovePlayer(struct GameData* _gd, struct Player* _player)
{
	_player->line = GridGetLine(_player->pos.y);
	_player->column = GridGetColumn(_player->pos.x);

	if (_player->isMoving)
	{
		sfVector2f nextPos = { _player->posToGo.x - _player->pos.x, _player->posToGo.y - _player->pos.y };
		float squaredDistNextPos = GetSquaredLength(nextPos);

		if (squaredDistNextPos > DIST_MIN)
		{
			_player->pos.x += _player->speed * cosf(Deg2Rad(_player->angle)) * _gd->syst.dt;
			_player->pos.y += _player->speed * sinf(Deg2Rad(_player->angle)) * _gd->syst.dt;
		}
		else
		{
			_player->isMoving = sfFalse;
		}
	}
}

void CheckColCells(struct GameData* _gd, struct Player* _player)
{
	if (_gd->grid[_player->line][_player->column].isColliding)
	{
		_player->isMoving = sfFalse;
		_player->pos.x -= _player->speed * cosf(Deg2Rad(_player->angle)) * _gd->syst.dt;
		_player->pos.y -= _player->speed * sinf(Deg2Rad(_player->angle)) * _gd->syst.dt;
	}
}

void DeleteClosestConstructibleCells(struct GameData* _gd, struct Player* _player)
{
	for (int l = _player->line - 3; l < _player->line + 3; l++)
	{
		for (int c = _player->column - 3; c < _player->column + 3; c++)
		{
			_gd->grid[l][c].isConstructible = sfFalse;
		}
	}
}

void GenericCheckConstructibleCells(struct GameData* _gd, int _line, int _column)
{
	if (!_gd->grid[_line][_column].isColliding && IsCellAvailable(_line, _column))
	{
		_gd->grid[_line][_column].isConstructible = sfTrue;
	}
}

void CheckConstructibleCells(struct GameData* _gd, struct Player* _player)
{
	DeleteClosestConstructibleCells(_gd, _player);
	GenericCheckConstructibleCells(_gd, _player->line - 1, _player->column);
	GenericCheckConstructibleCells(_gd, _player->line + 1, _player->column);
	GenericCheckConstructibleCells(_gd, _player->line, _player->column - 1);
	GenericCheckConstructibleCells(_gd, _player->line, _player->column + 1);
}

void CheckLose(struct GameData* _gd, struct Player* _player)
{
	if (_player->hp <= 0)
	{
		_gd->syst.gameState = HIGHSCORES;
		SaveHighScore(_gd);
	}
}

void UpdatePlayer(struct GameData* _gd)
{
	struct Player* player = &_gd->player;

	MovePlayer(_gd, player);
	CheckConstructibleCells(_gd, player);
	CheckColCells(_gd, player);
	CheckLose(_gd, player);
}

void DisplayPlayer(struct GameData* _gd)
{
	struct Player* player = &_gd->player;
	sfVector2f tempPos = PosWithCamera(player->pos, _gd->cam.pos);

	BlitSprite(_gd->spr.player[player->dir], tempPos, 0, _gd->syst.window);
}