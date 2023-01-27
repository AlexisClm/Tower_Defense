#include "definitions.h"
#include "init.h"
#include "update.h"
#include "display.h"

struct GameData gd = { 0 };

void main()
{
	Init(&gd);

	while (sfRenderWindow_isOpen(gd.syst.window))
	{
		while (sfRenderWindow_pollEvent(gd.syst.window, &gd.syst.event))
		{
			EvtUpdate(&gd);
		}
		Update(&gd);
		Display(&gd);
	}
}