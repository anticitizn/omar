#ifndef OMAR
#define OMAR

#include <string>
#include <SDL.h>

#include <src/TileContainer.h>
#include <src/Tile.h>
#include <src/Point.h>
#include <src/Color.h>

using namespace std;

namespace omar
{

	class Terminal : public TileContainer
	{
	private:
		SDL_Window* screen;
		SDL_Event event;
		
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
		
		unsigned int lastTick = 0;
		void displayFps();
		
		float* vertices;
		
		// user input
		Point clickPos;
		Point mousePos;
		
		bool leftClickHeld;
		bool rightClickHeld;
		
		char keyPress;
		string textInput;
		
		void processInput(SDL_Event event);
		void cleanInputVariables();
		
		void allocateVertices(float** vertices, const int width, const int height);
		void generateVertices(float* vertices, TileContainer& content, const int width, const int height);
		void generateQuad(float* vertices, Tile tile, const int x, const int y, const int tileSize, const int fontSymbolWidth, const int fontSymbolHeight);
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
		
		Point screenToTilePosition(int x, int y);
		Point screenToTilePosition(Point point);
		
		bool showFps = false;
		unsigned int getTicks();
		
		// input methods
		Point getMousePosTile();
		Point getClickPosTile();
		
		Point getMousePosScreen();
		Point getClickPosScreen();
		
		bool isLClickHeld();
		bool isRClickHeld();
		
		char getKeyPress();
		
		void StartTextInput();
		void StopTextInput();
		string getTextInput();
		
	};

}

#endif
