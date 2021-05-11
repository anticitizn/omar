#ifndef OMAR
#define OMAR

#include <string>
#include "SDL.h"

#include "include/Tile.h"
#include "include/Point.h"
#include "include/Color.h"

using namespace std;

namespace omar
{

	class Terminal
	{
	private:
		SDL_Window* screen;
		
		// the path to the font image and the shaders folder
		string resourcesPath;
		
		// the symbol applied to all tiles when initializing or clear() is called
		char fillSymbol;
		
		// dimensions of one tile, overwritten by constructor
		float tileSize;
		
		// dimensions in pixels
		int contextWidth = 0;
		int contextHeight = 0;
		
		// dimensions in tiles
		int width;
		int height;
		
		// width and height of a singular character on the font
		float fontSymbolWidth = 16.0f;
		float fontSymbolHeight = 14.0f;
		
		Color textColor = Color(255, 255, 255);
		Color tileColor = Color(0, 0, 0);
		
		Tile* tiles;
		float* vertices;
		
		void generateTiles(Tile** tiles, const int width, const int height);;
		void allocateVertices(float** vertices, const int width, const int height);
		void generateVertices(float* vertices, Tile* tiles, const int width, const int height);
		void generateQuad(float* vertices, const Tile& tile, const int x, const int y, const int tileSize, const int fontSymbolWidth, const int fontSymbolHeight);
		void addColorInfo(float* vertices, const Color& color);
		
	public:
		Terminal(const float TileSize = 16.0f, const char FillSymbol = ' ', string ResourcesPath = "");
		void initialize();
		void draw();
		void clear();
		
		// getter methods
		char getChar(int x, int y);
		
		Color getTextColor(int x, int y);
		Color getTileColor(int x, int y);
		
		Point getPixelDimensions();
		Point getTileDimensions();
		
		// setter methods
		void setChar(int x, int y, char symbol);
		
		void setTextColor(int x, int y, int r, int g, int b);
		void setTextColor(int x, int y, Color color);
		
		void setTileColor(int x, int y, int r, int g, int b);
		void setTileColor(int x, int y, Color color);
		
		void setString(int x, int y, string str);
		void setString(int x, int y, string str, int r, int g, int b);
		
		Point screenToTilePosition(int x, int y);
		
		Point screenToTilePosition(Point point);
		
		unsigned int getTicks();
	};

}

#endif
