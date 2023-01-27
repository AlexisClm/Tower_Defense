#include "enemy.h"

void InitWave(struct GameData* _gd)
{
	struct Wave* wave = &_gd->wave;

	wave->current = 0;
	wave->nbEnemy = 15;
	wave->timerMax = 10;
	wave->timer = 0;
}

void InitRandomPos(struct GameData* _gd, struct Enemy* _enemy)
{
	int random = rand() % NB_DIR;

	switch (random)
	{
	case UP:
		_enemy->pos.x = GridGetX(RAND_MIN_MAX(0, NB_COLUMNS)) + CELL_SIZE / 2;
		_enemy->pos.y = GridGetY(0) - CELL_SIZE / 2;
		break;

	case DOWN:
		_enemy->pos.x = GridGetX(RAND_MIN_MAX(0, NB_COLUMNS)) + CELL_SIZE / 2;
		_enemy->pos.y = GridGetY(NB_LINES) + CELL_SIZE / 2;
		break;

	case LEFT:
		_enemy->pos.x = GridGetX(0) - CELL_SIZE / 2;
		_enemy->pos.y = GridGetY(RAND_MIN_MAX(0, NB_LINES));
		break;

	case RIGHT:
		_enemy->pos.x = GridGetX(NB_COLUMNS) + CELL_SIZE / 2;
		_enemy->pos.y = GridGetY(RAND_MIN_MAX(0, NB_LINES));
		break;

	default:
		break;
	}
}

void Move(struct GameData* _gd, struct Enemy* _enemy)
{
	sfVector2f nextPos = { _enemy->posToGo.x - _enemy->pos.x, _enemy->posToGo.y - _enemy->pos.y };
	float squaredDistNextPos = GetSquaredLength(nextPos);

	_enemy->angle = Rad2Deg(atan2f(nextPos.y, nextPos.x));

	if (squaredDistNextPos > DIST_MIN)
	{
		_enemy->pos.x += _enemy->speed * cosf(Deg2Rad(_enemy->angle)) * _gd->syst.dt;
		_enemy->pos.y += _enemy->speed * sinf(Deg2Rad(_enemy->angle)) * _gd->syst.dt;
	}
	else
	{
		_enemy->posToGo.x = GridGetX(RAND_MIN_MAX(BUILDING_COLUMN_MIN, BUILDING_COLUMN_MAX));
		_enemy->posToGo.y = GridGetY(RAND_MIN_MAX(BUILDING_LINE_MIN, BUILDING_LINE_MAX));
	}
}

void Shoot(struct GameData* _gd, struct Enemy* _enemy)
{
	_enemy->timer += _gd->syst.dt;
	if (_enemy->timer > _enemy->timerMax)
	{
		CreateShot(_gd, _enemy->pos, _enemy->range, _enemy->angle, _enemy->type + 3);
		_enemy->timer = 0;
	}
}

void EarnDiamantum(struct GameData* _gd, struct Enemy* _enemy)
{
	switch (_enemy->type)
	{
	case SLOW:
		_gd->player.diamantum += DIAMANTUM_VALUE_ENEMY_SLOW;
		_gd->player.score += DIAMANTUM_VALUE_ENEMY_SLOW * 10;
			break;

	case FAST:
		_gd->player.diamantum += DIAMANTUM_VALUE_ENEMY_FAST;
		_gd->player.score += DIAMANTUM_VALUE_ENEMY_FAST * 10;
			break;
	default:
		break;
	}
}

void Drop(struct GameData* _gd, struct Enemy* _enemy)
{
	int random = rand() % 3;

	if (!random)
	{
		CreateDiamantum(_gd, _enemy->pos);
	}

	EarnDiamantum(_gd, _enemy);

	*_enemy = _gd->enemy[_gd->nb.enemy - 1];
	_gd->nb.enemy--;
}

void CreateEnemy(struct GameData* _gd)
{
	if (_gd->nb.enemy < NB_ENEMY_MAX)
	{
		struct Enemy* enemy = &_gd->enemy[_gd->nb.enemy];

		enemy->rect = sfRectangleShape_create();
		enemy->type = rand() % NB_TYPE_ENEMY;
		enemy->action = Move;
		enemy->size.x = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.enemy[enemy->type])).x;
		enemy->size.y = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.enemy[enemy->type])).y;
		enemy->posToGo.x = GridGetX(NB_COLUMNS / 2);
		enemy->posToGo.y = GridGetY(NB_LINES / 2);
		enemy->timerMax = 2;
		enemy->timer = enemy->timerMax;

		switch (enemy->type)
		{
		case SLOW:
			enemy->hpMax = 125;
			enemy->speed = 75;
			break;

		case FAST:
			enemy->hpMax = 50;
			enemy->speed = 150;
			break;
		default:
			break;
		}

		enemy->hp = enemy->hpMax;
		enemy->range = 10;
		enemy->distMin = (float)(enemy->range * CELL_SIZE) * (enemy->range * CELL_SIZE);
		enemy->nearestDist = enemy->distMin + 1;

		InitRandomPos(_gd, enemy);

		_gd->nb.enemy++;
	}
}

void UpdateWave(struct GameData* _gd)
{
	struct Wave* wave = &_gd->wave;

	wave->timer += _gd->syst.dt;

	if (wave->timer > wave->timerMax)
	{
		for (int i = 0; i < wave->nbEnemy; i++)
		{
			CreateEnemy(_gd);
		}

		wave->current++;
		wave->nbEnemy += wave->current * 2;
		wave->timer = 0;
	}
}

void UpdateEnemiesAction(struct Enemy* _enemy)
{
	if (_enemy->nearestDist < _enemy->distMin)
	{
		_enemy->action = Shoot;
	}
	else
	{
		_enemy->action = Move;
	}
	if (_enemy->hp <= 0)
	{
		_enemy->action = Drop;
	}
}

void CheckNearestEntity(struct GameData* _gd, struct Enemy* _enemy)
{
	_enemy->nearestDist = _enemy->distMin + 1;

	//Check nearest dist building/turret
	for (int l = 0; l < NB_LINES; l++)
	{
		for (int c = 0; c < NB_COLUMNS; c++)
		{
			struct Grid* cell = &_gd->grid[l][c];

			if (cell->isColliding)
			{
				sfVector2f dist = { cell->pos.x + CELL_SIZE / 2 - _enemy->pos.x, cell->pos.y + CELL_SIZE / 2 - _enemy->pos.y };
				float squaredDist = GetSquaredLength(dist);

				if (squaredDist < _enemy->nearestDist)
				{
					_enemy->posToShot = dist;
					_enemy->nearestDist = squaredDist;
					_enemy->angle = Rad2Deg(atan2f(dist.y, dist.x));
				}
			}
		}
	}

	//Check nearest dist player
	struct Player* player = &_gd->player;
	sfVector2f dist = { player->pos.x - _enemy->pos.x, player->pos.y - _enemy->pos.y };
	float squaredDist = GetSquaredLength(dist);

	if (_enemy->nearestDist > squaredDist)
	{
		_enemy->posToShot = dist;
		_enemy->nearestDist = squaredDist;
		_enemy->angle = Rad2Deg(atan2f(dist.y, dist.x));
	}
}

void UpdateEnemies(struct GameData* _gd)
{
	UpdateWave(_gd);

	for (int i = 0; i < _gd->nb.enemy; i++)
	{
		struct Enemy* enemy = &_gd->enemy[i];

		UpdateEnemiesAction(enemy);
		CheckNearestEntity(_gd, enemy);
		enemy->action(_gd, enemy);
	}
}

void DisplayEnemies(struct GameData* _gd)
{
	for (int i = 0; i < _gd->nb.enemy; i++)
	{
		struct Enemy* enemy = &_gd->enemy[i];

		sfVector2f tempPos = PosWithCamera(enemy->pos, _gd->cam.pos);
		BlitSprite(_gd->spr.enemy[enemy->type], tempPos, 0, _gd->syst.window);
	}
}
