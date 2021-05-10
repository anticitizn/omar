#ifndef OMAR_TILE
#define OMAR_TILE

#include "Color.h"

namespace omar
{
	struct Tile
	{
		Tile (char Symbol, Color TextColor, Color TileColor)
		{
			symbol = Symbol;
			textColor = TextColor;
			tileColor = TileColor;
		}
		
		char symbol;
		Color textColor;
		Color tileColor;
	};
}

#endif
