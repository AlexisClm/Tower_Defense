#include "grid.h"

void InitGrid(struct GameData* _gd)
{
	for (int l = 0; l < NB_LINES; l++)
	{
		for (int c = 0; c < NB_COLUMNS; c++)
		{
			struct Grid* cell = &_gd->grid[l][c];

			cell->rect = sfRectangleShape_create();
			cell->size.x = CELL_SIZE;
			cell->size.y = CELL_SIZE;
			cell->pos.x = GridGetX(c);
			cell->pos.y = GridGetY(l);
			cell->isColliding = sfFalse;
			cell->type = rand() % (NB_TYPE_CELL - 1);
		}
	}
}

void DisplayGrid(struct GameData* _gd)
{
	struct Player* player = &_gd->player;

	for (int l = 0; l < NB_LINES; l++)
	{
		for (int c = 0; c < NB_COLUMNS; c++)
		{
			struct Grid* cell = &_gd->grid[l][c];

			if (_gd->cam.pos.x < cell->pos.x + cell->size.x && _gd->cam.pos.x + SCREEN_W > cell->pos.x &&
				_gd->cam.pos.y < cell->pos.y + cell->size.x && _gd->cam.pos.y + SCREEN_H > cell->pos.y)
			{
				sfVector2f tempPos = PosWithCamera(cell->pos, _gd->cam.pos);
				BlitSprite(_gd->spr.grid[cell->type], tempPos, 0, _gd->syst.window);

				if (cell->isConstructible)
				{
					sfRectangleShape_setFillColor(cell->rect, (sfColor) { 0, 255, 0, 100 });
					BlitRectangle(cell->rect, tempPos, cell->size, _gd->syst.window);
				}
			}
		}
	}
}