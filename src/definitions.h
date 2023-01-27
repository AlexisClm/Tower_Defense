#ifndef definitions__H
#define definitions__H

#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/OpenGL.h>
#include <SFML/Network.h>
#include <SFML/Audio.h>
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"
#include "math.h"
#include "tchar.h"
#include "string.h"

#define RAND_MIN_MAX(min, max) rand() % (max-min+1) + min

#define SCREEN_W 1600
#define SCREEN_H 900

#define PI 3.14159265359f

#define NB_LINES 250
#define NB_COLUMNS 250
#define CELL_SIZE 32
#define GRID_OFFSET_X (SCREEN_W - NB_COLUMNS*CELL_SIZE)/2
#define GRID_OFFSET_Y (SCREEN_H - NB_LINES*CELL_SIZE)/2

#define BUILDING_LINE_MIN NB_LINES / 3
#define BUILDING_LINE_MAX NB_LINES * 2 / 3
#define BUILDING_COLUMN_MIN NB_COLUMNS / 3
#define BUILDING_COLUMN_MAX NB_COLUMNS * 2 / 3

#define RANGE_EXPLODE_MISSILE 5
#define RANGE_EXPLODE_MAX (RANGE_EXPLODE_MISSILE * CELL_SIZE) * (RANGE_EXPLODE_MISSILE * CELL_SIZE)

#define DIAMANTUM_VALUE_TURRET_LONG_RANGE 20
#define DIAMANTUM_VALUE_TURRET_SHORT_RANGE 15
#define DIAMANTUM_VALUE_TURRET_MISSILE 30
#define DIAMANTUM_VALUE_TURRET_WALL 5
#define DIAMANTUM_VALUE_ENEMY_SLOW 1
#define DIAMANTUM_VALUE_ENEMY_FAST 2
#define DIAMANTUM_VALUE_BONUS 5

#define DIST_MIN 10

#define NB_CHAR_MAX 50
#define NB_TYPE_TURRET 4
#define NB_TURRET_MAX 1000
#define NB_ENEMY_MAX 1000
#define NB_DIAMANTUM_MAX 1000
#define NB_HIGHSCORE_MAX 100

typedef void (*ActionEnemy)(struct GameData* _gd, struct Enemy* _enemy);

enum GameState
{
	GAME,
	HIGHSCORES
};

enum TypeFont
{
	OUTLINE,
	NORMAL,
	NB_TYPE_FONT
};

enum TypeCell
{
	ROCK,
	SAND,
	SAND2,
	SAND3,
	RUIN,
	NB_TYPE_CELL
};

enum TypeBuilding
{
	HOUSE,
	BARN,
	PIGSTY,
	HANGAR,
	GARAGE,
	NB_TYPE_BUILDING
};

enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NB_DIR
};

enum TypeTurret
{
	LONG_RANGE = 1,
	SHORT_RANGE,
	MISSILE,
	WALL,
};

enum TypeShot
{
	SHOT_LONG_RANGE,
	SHOT_SHORT_RANGE,
	SHOT_MISSILE,
	SHOT_ENEMY_SLOW,
	SHOT_ENEMY_FAST,
	NB_TYPE_SHOT
};

enum TypeEnemy
{
	SLOW,
	FAST,
	NB_TYPE_ENEMY
};

struct System
{
	sfRenderWindow* window;
	sfClock* dtClock;
	sfEvent event;
	float dt;
	enum GameState gameState;
};

struct Sprites
{
	sfSprite* grid[NB_TYPE_CELL];
	sfSprite* building[NB_TYPE_BUILDING];
	sfSprite* player[NB_DIR];
	sfSprite* enemy[NB_TYPE_ENEMY];
	sfSprite* turret[NB_TYPE_TURRET];
	sfSprite* shot[NB_TYPE_SHOT];
	sfSprite* diamantum;
};

struct Font
{
	sfFont* normal;
	sfFont* outline;
};

struct Mouse
{
	sfVector2i pos;
	sfVector2i worldPos;
	int line;
	int column;
};

struct Camera
{
	sfVector2f pos;
	float speed;
	char isFocused;
};

struct Grid
{
	sfRectangleShape* rect;
	sfVector2f pos;
	sfVector2f size;
	char isColliding;
	char isConstructible;
	char isBuilding;
	enum TypeCell type;
};

struct Building
{
	sfRectangleShape* rect;
	sfVector2f pos;
	sfVector2f size;
	int line;
	int column;
	int nbLines;
	int nbColumns;
	int hp;
	int hpMax;
	char isKo;
	enum TypeBuilding type;
};

struct Player
{
	sfRectangleShape* rect;
	sfVector2f pos;
	sfVector2f posToGo;
	sfVector2f size;
	int line;
	int column;
	int hp;
	int hpMax;
	int diamantum;
	int score;
	float angle;
	float speed;
	char isMoving;
	char canBuild;
	char name[50];
	enum Direction dir;
	enum TypeTurret selectedTurret;
};

struct Enemy
{
	sfRectangleShape* rect;
	ActionEnemy action;
	sfVector2f pos;
	sfVector2f posToGo;
	sfVector2f posToShot;
	sfVector2f size;
	int range;
	int hp;
	int hpMax;
	float angle;
	float speed;
	float timer;
	float timerMax;
	float distMin;
	float nearestDist;
	enum TypeEnemy type;
};

struct Wave
{
	int current;
	int nbEnemy;
	float timer;
	float timerMax;
};

struct Turret
{
	sfRectangleShape* rect;
	sfVector2f pos;
	sfVector2f posToShot;
	sfVector2f size;
	int line;
	int column;
	int range;
	int hp;
	int hpMax;
	float timer;
	float timerMax;
	float distMax;
	float nearestDist;
	enum TypeTurret type;
};

struct Shot
{
	struct Shot* next;
	sfVector2f pos;
	sfVector2f posFrom;
	sfVector2f size;
	int dmg;
	int range;
	float angle;
	float speed;
	float distMax;
	char isKo;
	enum TypeShot type;
};

struct Diamantum
{
	sfVector2f pos;
	sfVector2f size;
	float angle;
	float speed;
	float distMax;
};

struct OneHighScore
{
	int score;
	char name[50];
	char date[50];
};

struct HighScore
{
	sfText* text[NB_TYPE_FONT];
	int nb;
	struct OneHighScore tab[NB_HIGHSCORE_MAX];
};

struct Hud
{
	sfText* text;
	//sfText* text[NB_TYPE_FONT];
};

struct Counter
{
	int turret;
	int enemy;
	int diamantum;
};

struct GameData
{
	struct System syst;
	struct Sprites spr;
	struct Font font;
	struct Mouse mouse;
	struct Camera cam;
	struct Grid grid[NB_LINES][NB_COLUMNS];
	struct Building building[NB_TYPE_BUILDING];
	struct Player player;
	struct Enemy enemy[NB_ENEMY_MAX];
	struct Wave wave;
	struct Turret turret[NB_TURRET_MAX];
	struct Shot* shot;
	struct Diamantum diamantum[NB_DIAMANTUM_MAX];
	struct HighScore highScore;
	struct Hud hud;
	struct Counter nb;
};

#endif