#ifndef tools__H
#define tools__H

#include "definitions.h"

sfSprite* LoadSprite(const char* _filename, int _isCentered);
sfSound* LoadSound(const char* _filename, float _volume, sfBool _loop);
sfText* LoadText(sfFont* _font, int _size);

void BlitSprite(sfSprite* _sprite, sfVector2f _pos, float _angle, sfRenderWindow* _window);
void BlitRectangle(sfRectangleShape* _rect, sfVector2f _pos, sfVector2f _size, sfRenderWindow* _window);
void BlitText(sfText* _text, sfVector2f _pos, const char* _words, sfRenderWindow* _window);

sfVector2i ScreenToWorld(sfVector2i _mouse, sfVector2f _cam);
sfVector2f PosWithCamera(sfVector2f _pos, sfVector2f _cam);

float GridGetX(int _column);
float GridGetY(int _line);
int GridGetLine(float _y);
int GridGetColumn(float _x);
int IsCellAvailable(int _line, int _column);

float Rad2Deg(float _value);
float Deg2Rad(float _value);
float GetSquaredLength(sfVector2f _vector);

int IsCollidingRectRectCentered(sfVector2f _pos1, sfVector2f _size1, sfVector2f _pos2, sfVector2f _size2);

#endif