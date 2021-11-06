#include <src/TileContainer.h>

#define xyPos y*width + x

using namespace omar;

TileContainer::TileContainer(int Width /*= 0*/, int Height /*= 0*/, char FillSymbol /*= ' '*/, Color TextColor /*= Color(255, 255, 255)*/, Color TileColor /*= Color(0, 0, 0)*/)
{
	width = Width;
	height = Height;
	fillSymbol = FillSymbol;
	textColor = TextColor;
	tileColor = TileColor;
	
	generateTiles(&tiles, width, height, fillSymbol);
}

void TileContainer::reinitialize(int Width, int Height, char FillSymbol, Color TextColor, Color TileColor)
{
	width = Width;
	height = Height;
	fillSymbol = FillSymbol;
	textColor = TextColor;
	tileColor = TileColor;
	free(tiles);
	
	generateTiles(&tiles, width, height, fillSymbol);
}

void TileContainer::blit(TileContainer& otherCard, int xOffset, int yOffset)
{
	for (int x = 0; x < otherCard.width; x++)
	{
		for (int y = 0; y < otherCard.height; y++)
		{
			if (otherCard.blitTransparency && otherCard.getChar(x, y) == ' ')
			{
				continue;
			}
			else
			{
				setChar(x + xOffset, y + yOffset, otherCard.getChar(x, y));
				setTextColor(xOffset + x, y + yOffset, otherCard.getTextColor(x, y));
				setTileColor(xOffset + x, y + yOffset, otherCard.getTileColor(x, y));
			}
		}
	}
}

void TileContainer::generateTiles(Tile** tiles, const int width, const int height, const char symbol)
{
	*tiles = (Tile*) malloc(width * height * sizeof(Tile));
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			setChar(x, y, symbol);
			setTextColor(x, y, textColor);
			setTileColor(x, y, tileColor);
		}
	}
}

// getter methods
char TileContainer::getChar(int x, int y) 
{
	if (x < width && y < height && x >= 0 && y >= 0)
		return tiles[xyPos].symbol;
	else
		return (char)0;
}

Color TileContainer::getTextColor(int x, int y)
{
	if (x < width && y < height && x >= 0 && y >= 0)
		return tiles[xyPos].textColor;
	else
		return Color(0, 0, 0);
}

Color TileContainer::getTileColor(int x, int y)
{
	if (x < width && y < height && x >= 0 && y >= 0)
		return tiles[xyPos].tileColor;
	else
		return Color(0, 0, 0);
}

Tile TileContainer::getTile(int x, int y)
{
	if (x < width && y < height && x >= 0 && y >= 0)
		return tiles[xyPos];
	else
		return Tile(' ', Color(0, 0, 0), Color(200, 200, 0));
}

Point TileContainer::getDimensions()
{
	return Point(width, height);
}

// setter methods
void TileContainer::setChar(int x, int y, char symbol)
{
	if (x < width && y < height && x >= 0 && y >= 0)
	{
		 tiles[xyPos].symbol = symbol;
	}
}

void TileContainer::setTextColor(int x, int y, int r, int g, int b)
{
	if (x < width && y < height && x >= 0 && y >= 0)
	{
		 tiles[xyPos].textColor = Color(r, g, b);
	}
}

void TileContainer::setTextColor(int x, int y, Color color)
{
	setTextColor(x, y, color.r, color.g, color.b);
}

void TileContainer::setTileColor(int x, int y, int r, int g, int b)
{
	if (x < width && y < height && x >= 0 && y >= 0)
	{
		 tiles[xyPos].tileColor = Color(r, g, b);
	}
}

void TileContainer::setTileColor(int x, int y, Color color)
{
	setTileColor(x, y, color.r, color.g, color.b);
}

void TileContainer::setString(int x, int y, string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		setChar(x + i, y, str[i]);
	}
}

void TileContainer::setString(int x, int y, string str, Color textColor)
{
	for (int i = 0; i < str.length(); i++)
	{
		setChar(x + i, y, str[i]);
		setTextColor(x + i, y, textColor);
	}
}

void TileContainer::setString(int x, int y, string str, Color textColor, Color tileColor)
{
	for (int i = 0; i < str.length(); i++)
	{
		setChar(x + i, y, str[i]);
		setTextColor(x + i, y, textColor);
		setTileColor(x + i, y, tileColor);
	}
}
