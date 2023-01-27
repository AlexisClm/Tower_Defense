#include "tools.h"

//Returns the sprite
sfSprite* LoadSprite(const char* _filename, int _isCentered)
{
	sfSprite* tempSprite;
	sfTexture* tempText;

	tempText = sfTexture_createFromFile(_filename, NULL);
	tempSprite = sfSprite_create();
	sfSprite_setTexture(tempSprite, tempText, sfTrue);

	if (_isCentered == 1)
	{
		sfVector2u tempSize = sfTexture_getSize(tempText);
		sfVector2f tempOrigin = { (float)tempSize.x / 2, (float)tempSize.y / 2 };
		sfSprite_setOrigin(tempSprite, tempOrigin);
	}

	return tempSprite;
}

//Chargement du son
sfSound* LoadSound(const char* _filename, float _volume, sfBool _loop)
{
	sfSound* tempSound;
	sfSoundBuffer* tempSoundBuffer;

	tempSound = sfSound_create();
	tempSoundBuffer = sfSoundBuffer_createFromFile(_filename);
	sfSound_setBuffer(tempSound, tempSoundBuffer);
	sfSound_setVolume(tempSound, _volume);
	sfSound_setLoop(tempSound, _loop);

	return tempSound;
}

//Chargement du texte
sfText* LoadText(sfFont* _font, int _size)
{
	sfText* tempText;

	tempText = sfText_create();
	sfText_setFont(tempText, _font);
	sfText_setCharacterSize(tempText, _size);

	return tempText;
}

//Affichage du sprite
void BlitSprite(sfSprite* _sprite, sfVector2f _pos, float _angle, sfRenderWindow* _window)
{
	sfVector2f tempPos = { _pos.x, _pos.y };
	sfSprite_setPosition(_sprite, tempPos);
	sfSprite_setRotation(_sprite, _angle);
	sfRenderWindow_drawSprite(_window, _sprite, NULL);
}

//Affichage du rectangle
void BlitRectangle(sfRectangleShape* _rect, sfVector2f _pos, sfVector2f _size, sfRenderWindow* _window)
{
	sfVector2f tempPos = { _pos.x, _pos.y };
	sfRectangleShape_setPosition(_rect, tempPos);
	sfVector2f tempSize = { _size.x, _size.y };
	sfRectangleShape_setSize(_rect, tempSize);
	sfRenderWindow_drawRectangleShape(_window, _rect, NULL);
}

//Affichage du texte
void BlitText(sfText* _text, sfVector2f _pos, const char* _words, sfRenderWindow* _window)
{
	sfText_setString(_text, _words);
	sfText_setPosition(_text, _pos);
	sfRenderWindow_drawText(_window, _text, NULL);
}

//Fonction pour le Mouse picking
sfVector2i ScreenToWorld(sfVector2i _mouse, sfVector2f _cam)
{
	return (sfVector2i) { (_mouse.x + (int)_cam.x), (_mouse.y + (int)_cam.y) };
}

//Fonction pour la translation à l'écran
sfVector2f PosWithCamera(sfVector2f _pos, sfVector2f _cam)
{
	return (sfVector2f) { (_pos.x - _cam.x), (_pos.y - _cam.y) };
}

//Vérifie si la case est disponible
int IsCellAvailable(int _line, int _column)
{
	return (_line >= 0) && (_line < NB_LINES) && (_column >= 0) && (_column < NB_COLUMNS);
}

//Returns the X pos on the grid
float GridGetX(int _column)
{
	return (float)GRID_OFFSET_X + _column * CELL_SIZE;
}

//Returns the Y pos on the grid
float GridGetY(int _line)
{
	return (float)GRID_OFFSET_Y + _line * CELL_SIZE;
}

//Returns grid's line
int GridGetLine(float _y)
{
	return (int)floor(((_y - GRID_OFFSET_Y) / CELL_SIZE));
}

//Returns grid's column
int GridGetColumn(float _x)
{
	return (int)floor(((_x - GRID_OFFSET_X) / CELL_SIZE));
}

//Converts from radians to degrees
float Rad2Deg(float _value)
{
	return _value * 180 / PI;
}

//Converts from degrees to radians
float Deg2Rad(float _value)
{
	return _value / 180 * PI;
}

//Returns vector's squared length
float GetSquaredLength(sfVector2f _vector)
{
	return _vector.x * _vector.x + _vector.y * _vector.y;
}

//Returns true if there is a collision between two centered rectangles
int IsCollidingRectRectCentered(sfVector2f _pos1, sfVector2f _size1, sfVector2f _pos2, sfVector2f _size2)
{
	return (_pos1.x - _size1.x / 2 < _pos2.x + _size2.x / 2 &&
			_pos1.x + _size1.x / 2 > _pos2.x - _size2.x / 2 &&
			_pos1.y - _size1.y / 2 < _pos2.y + _size2.y / 2 &&
			_pos1.y + _size1.y / 2 > _pos2.y - _size2.y / 2);
}