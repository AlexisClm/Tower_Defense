#include "mouse.h"

void InputMouse(struct GameData* _gd)
{
	struct Mouse* mouse = &_gd->mouse;
	sfEvent event = _gd->syst.event;

	if (event.type == sfEvtMouseButtonPressed)
	{
		if (event.mouseButton.button == sfMouseLeft)
		{
			mouse->line = GridGetLine((float)mouse->worldPos.y);
			mouse->column = GridGetColumn((float)mouse->worldPos.x);

			InputTurret(_gd);
		}
		else if (event.mouseButton.button == sfMouseRight)
		{
			InputPlayer(_gd);
		}
	}
	else if (event.type == sfEvtClosed)
	{
		exit(0);
	}
}

void InputKeyboard(struct GameData* _gd)
{
	sfEvent event = _gd->syst.event;

	if (event.type == sfEvtKeyPressed)
	{
		SelectTurret(_gd);

		if (event.key.code == sfKeySpace)
		{
			FocusCamera(_gd);
		}
		else if (event.key.code == sfKeyEscape)
		{
			exit(0);
		}
	}
}

void UpdateMouse(struct GameData* _gd)
{
	struct Mouse* mouse = &_gd->mouse;

	mouse->pos = sfMouse_getPositionRenderWindow(_gd->syst.window);
	mouse->worldPos = ScreenToWorld(mouse->pos, _gd->cam.pos);
}