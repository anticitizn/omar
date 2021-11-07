#ifndef OMAR_TILECONTAINER
#define OMAR_TILECONTAINER

#include <src/Point.h>
#include <src/Color.h>
#include <src/Tile.h>

#include <string>

using namespace std;

namespace omar
{

	class TileContainer
	{
	private:
		// the symbol applied to all tiles when initializing or clear() is called
		char fillSymbol;
		
		int width;
		int height;
		
		Color textColor;
		Color tileColor;
		
		Tile* tiles;
		
		void generateTiles(Tile** tiles, const int width, const int height, const char symbol);
		
	public:
		TileContainer(int Width = 0, int Height = 0, char FillSymbol = ' ', Color TextColor = Color(255, 255, 255), Color TileColor = Color(0, 0, 0));
		void reinitialize(int Width = 0, int Height = 0, char FillSymbol = ' ', Color TextColor = Color(255, 255, 255), Color TileColor = Color(0, 0, 0));
		
		// these should maybe be protected instead of public
		// blits another TileContainer into itself starting at the given coordinates
		void blit(TileContainer& otherCard, int xOffset, int yOffset);
		// blits itself into another TileContainer starting at the given coordinates
		
		// getter methods
		char getChar(int x, int y);
		
		Color getTextColor(int x, int y);
		Color getTileColor(int x, int y);
		
		Tile getTile(int x, int y);
		
		Point getDimensions();
		
		// setter methods
		void setChar(int x, int y, char symbol);
		
		void setTextColor(int x, int y, int r, int g, int b);
		void setTextColor(int x, int y, Color color);
		
		void setTileColor(int x, int y, int r, int g, int b);
		void setTileColor(int x, int y, Color color);
		
		void setString(int x, int y, string str);
		void setString(int x, int y, string str, Color textColor);
		void setString(int x, int y, string str, Color textColor, Color tileColor);
		
		bool blitTransparency = true;
	};
	
}

#endif
