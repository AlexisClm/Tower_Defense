#include "building.h"

#define HANGAR_LINE_2 6
#define HANGAR_COLUMN_2 6
#define GARAGE_COLUMN_2 5
#define GARAGE_COLUMN_3 8

void GenericSetBuildingCells(struct GameData* _gd, sfVector2i _min, sfVector2i _max, sfBool _isBuilding)
{
	for (int l = _min.y; l < _max.y; l++)
	{
		for (int c = _min.x; c < _max.x; c++)
		{
			_gd->grid[l][c].isColliding = _isBuilding;
			_gd->grid[l][c].isBuilding = _isBuilding;

			if (!_isBuilding)
			{
				_gd->grid[l][c].type = RUIN;
			}
		}
	}
}

void SetBuildingCells(struct GameData* _gd, struct Building* _building, sfBool _isBuilding)
{
	sfVector2i min = { _building->column, _building->line };
	sfVector2i max = { _building->column + _building->nbColumns, _building->line + _building->nbLines };

	switch (_building->type)
	{
	case HOUSE:
	case BARN:
	case PIGSTY:
		GenericSetBuildingCells(_gd, min, max, _isBuilding);
		break;

	case HANGAR:
		max.y = _building->line + HANGAR_LINE_2;
		GenericSetBuildingCells(_gd, min, max, _isBuilding);

		min.x = _building->column + HANGAR_COLUMN_2;
		max.y = _building->line + _building->nbLines;
		GenericSetBuildingCells(_gd, min, max, _isBuilding);
		break;

	case GARAGE:
		max.x = _building->column + GARAGE_COLUMN_2;
		GenericSetBuildingCells(_gd, min, max, _isBuilding);

		min.x = _building->column + GARAGE_COLUMN_3;
		max.x = _building->column + _building->nbColumns;
		GenericSetBuildingCells(_gd, min, max, _isBuilding);
		break;
	default:
		break;
	}
}

void InitBuildings(struct GameData* _gd)
{
	for (int i = 0; i < NB_TYPE_BUILDING; i++)
	{
		struct Building* building = &_gd->building[i];

		building->rect = sfRectangleShape_create();
		building->size.x = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.building[i])).x;
		building->size.y = (float)sfTexture_getSize(sfSprite_getTexture(_gd->spr.building[i])).y;
		building->nbLines = (int)building->size.y / CELL_SIZE;
		building->nbColumns = (int)building->size.x / CELL_SIZE;
		building->line = RAND_MIN_MAX(BUILDING_LINE_MIN, BUILDING_LINE_MAX - building->nbLines);
		building->column = RAND_MIN_MAX(BUILDING_COLUMN_MIN, BUILDING_COLUMN_MAX - building->nbColumns);
		building->pos.x = GridGetX(building->column) + building->size.x / 2;
		building->pos.y = GridGetY(building->line) + building->size.y / 2;
		building->type = i;

		switch (i)
		{
		case HOUSE:
			building->hpMax = 1000;
			break;
		case BARN:
			building->hpMax = 3000;
			break;
		case PIGSTY:
			building->hpMax = 3000;
			break;
		case HANGAR:
			building->hpMax = 2000;
			break;
		case GARAGE:
			building->hpMax = 4000;
			break;
		default:
			break;
		}

		building->hp = building->hpMax;
		SetBuildingCells(_gd, building, sfTrue);
	}
}

void UpdateBuildings(struct GameData* _gd)
{
	for (int i = 0; i < NB_TYPE_BUILDING; i++)
	{
		struct Building* building = &_gd->building[i];

		if (building->hp <= 0 && !building->isKo)
		{
			SetBuildingCells(_gd, building, sfFalse);
			building->isKo = sfTrue;
		}
	}

	if (_gd->building[HOUSE].hp <= 0 && _gd->building[BARN].hp <= 0 && _gd->building[PIGSTY].hp <= 0 && _gd->building[HANGAR].hp <= 0 && _gd->building[GARAGE].hp <= 0)
	{
		_gd->syst.gameState = HIGHSCORES;
		SaveHighScore(_gd);
	}
}

void DisplayBuildings(struct GameData* _gd)
{
	for (int i = 0; i < NB_TYPE_BUILDING; i++)
	{
		struct Building* building = &_gd->building[i];
		sfVector2f tempPos = PosWithCamera(building->pos, _gd->cam.pos);

		if (!building->isKo)
		{
			BlitSprite(_gd->spr.building[i], tempPos, 0, _gd->syst.window);
		}
	}
}
