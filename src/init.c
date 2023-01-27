#include "init.h"

void InitSettings(struct GameData* _gd)
{
	sfVideoMode mode = { SCREEN_W, SCREEN_H, 32 };

	_gd->syst.window = sfRenderWindow_create(mode, "Créapocalypse", sfResize | sfClose, NULL);
	_gd->syst.dtClock = sfClock_create();
	_gd->syst.gameState = GAME;

	srand((unsigned int)time(NULL));
}

void InitSprites(struct GameData* _gd)
{
	struct Sprites* spr = &_gd->spr;

	spr->grid[ROCK] = LoadSprite("../Assets/Images/Grid/Rock.png", 0);
	spr->grid[SAND] = LoadSprite("../Assets/Images/Grid/Sand.png", 0);
	spr->grid[SAND2] = LoadSprite("../Assets/Images/Grid/Sand2.png", 0);
	spr->grid[SAND3] = LoadSprite("../Assets/Images/Grid/Sand3.png", 0);
	spr->grid[RUIN] = LoadSprite("../Assets/Images/Grid/Ruin.png", 0);

	spr->building[HOUSE] = LoadSprite("../Assets/Images/Building/House.png", 1);
	spr->building[BARN] = LoadSprite("../Assets/Images/Building/Barn.png", 1);
	spr->building[PIGSTY] = LoadSprite("../Assets/Images/Building/Pigsty.png", 1);
	spr->building[HANGAR] = LoadSprite("../Assets/Images/Building/Hangar.png", 1);
	spr->building[GARAGE] = LoadSprite("../Assets/Images/Building/Garage.png", 1);

	spr->player[UP] = LoadSprite("../Assets/Images/Player/Up.png", 1);
	spr->player[DOWN] = LoadSprite("../Assets/Images/Player/Down.png", 1);
	spr->player[LEFT] = LoadSprite("../Assets/Images/Player/Left.png", 1);
	spr->player[RIGHT] = LoadSprite("../Assets/Images/Player/Right.png", 1);

	spr->enemy[SLOW] = LoadSprite("../Assets/Images/Enemy/Slow.png", 1);
	spr->enemy[FAST] = LoadSprite("../Assets/Images/Enemy/Fast.png", 1);

	spr->turret[LONG_RANGE - 1] = LoadSprite("../Assets/Images/Turret/Long.png", 1);
	spr->turret[SHORT_RANGE - 1] = LoadSprite("../Assets/Images/Turret/Short.png", 1);
	spr->turret[MISSILE - 1] = LoadSprite("../Assets/Images/Turret/Missile.png", 1);
	spr->turret[WALL - 1] = LoadSprite("../Assets/Images/Turret/Wall.png", 1);

	spr->shot[SHOT_LONG_RANGE] = LoadSprite("../Assets/Images/Shot/LongRange.png", 1);
	spr->shot[SHOT_SHORT_RANGE] = LoadSprite("../Assets/Images/Shot/ShortRange.png", 1);
	spr->shot[SHOT_MISSILE] = LoadSprite("../Assets/Images/Shot/Missile.png", 1);
	spr->shot[SHOT_ENEMY_SLOW] = LoadSprite("../Assets/Images/Shot/EnemySlow.png", 1);
	spr->shot[SHOT_ENEMY_FAST] = LoadSprite("../Assets/Images/Shot/EnemyFast.png", 1);

	spr->diamantum = LoadSprite("../Assets/Images/Diamantum.png", 1);
}

void InitTexts(struct GameData* _gd)
{
	struct Font* font = &_gd->font;

	font->outline = sfFont_createFromFile("../Assets/Fonts/Outline.ttf");
	font->normal = sfFont_createFromFile("../Assets/Fonts/Normal.ttf");

	_gd->hud.text = LoadText(font->normal, 35);
}

void InitGame(struct GameData* _gd)
{
	InitGrid(_gd);
	InitBuildings(_gd);
	InitPlayer(_gd);
	InitWave(_gd);
	InitCamera(_gd);
	ReadHighScores(_gd);
}

void Init(struct GameData* _gd)
{
	InitSettings(_gd);
	InitSprites(_gd);
	InitTexts(_gd);
	InitGame(_gd);
}
