#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION

#define xyPos y*width + x

#include "omar.h"

#include <iostream>
#include <string>
#include <SDL.h>

#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glad/glad.h"
#include "include/stb_image.h"

#include "include/Tile.h"
#include "include/Point.h"
#include "include/Color.h"
#include "include/shaders/shader.h"

using namespace std;
using namespace omar;

void Terminal::allocateVertices(float** vertices, const int width, const int height)
{
	int nFloats = 60; // amount of floats for every quad
	*vertices = (float*) malloc(width * height * nFloats * sizeof(float));
}

void Terminal::generateVertices(float* vertices, TileContainer& content, const int width, const int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			generateQuad(vertices + (xyPos) * 60, content.getTile(x, y), x, y, tileSize, fontSymbolWidth, fontSymbolHeight);
		}
	}
}

void Terminal::generateQuad(float* vertices, Tile tile, const int x, const int y, const int tileSize, const int fontSymbolWidth, const int fontSymbolHeight)
{
	int textureTileIndex = tile.symbol - 32;
	float textureY = floor(textureTileIndex / fontSymbolWidth);
	float textureX = textureTileIndex - textureY * fontSymbolWidth;
	
	float x1 = 8.0f * textureX;	   	  // left
	float y1 = 8.0f * textureY;		  // top
	float x2 = 8.0f * (textureX + 1); // right
	float y2 = 8.0f * (textureY + 1); // bottom
	
	// right top
	vertices[0] = tileSize * x + tileSize;
	vertices[1] = tileSize * y + tileSize;
	vertices[2] = x2;
	vertices[3] = y2;

	addColorInfo(&vertices[4], tile.textColor);

	addColorInfo(&vertices[7], tile.tileColor);

	// right bottom
	vertices[10] = tileSize * x + tileSize;
	vertices[11] = tileSize * y;
	vertices[12] = x2;
	vertices[13] = y1;

	addColorInfo(&vertices[14], tile.textColor);

	addColorInfo(&vertices[17], tile.tileColor);

	// left bottom
	vertices[20] = tileSize * x;
	vertices[21] = tileSize * y;
	vertices[22] = x1;
	vertices[23] = y1;

	addColorInfo(&vertices[24], tile.textColor);

	addColorInfo(&vertices[27], tile.tileColor);

	// left bottom
	vertices[30] = tileSize * x;
	vertices[31] = tileSize * y;
	vertices[32] = x1;
	vertices[33] = y1;

	addColorInfo(&vertices[34], tile.textColor);

	addColorInfo(&vertices[37], tile.tileColor);

	// left top
	vertices[40] = tileSize * x;
	vertices[41] = tileSize * y + tileSize;
	vertices[42] = x1;
	vertices[43] = y2;

	addColorInfo(&vertices[44], tile.textColor);

	addColorInfo(&vertices[47], tile.tileColor);

	// right top
	vertices[50] = tileSize * x + tileSize;
	vertices[51] = tileSize * y + tileSize;
	vertices[52] = x2;
	vertices[53] = y2;

	addColorInfo(&vertices[54], tile.textColor);

	addColorInfo(&vertices[57], tile.tileColor);
}

void Terminal::addColorInfo(float* vertices, const Color& color)
{
	vertices[0] = (float)color.r;
	vertices[1] = (float)color.g;
	vertices[2] = (float)color.b;
}

void Terminal::displayFps()
{
	unsigned int newTick = getTicks();
	int fps = 1000 / (int)(newTick - lastTick);
	lastTick = newTick;
	
	setString(0, 0, "   ");
	setString(0, 0, to_string(fps).substr(0, 3), Color(255, 255, 255), Color(0, 0, 0));
}

Terminal::Terminal(const float TileSize /* = 16.0f */, const char FillSymbol /* = ' ' */, string ResourcesPath /* = "" */)
{
	tileSize = TileSize;
	fillSymbol = FillSymbol;
	resourcesPath = ResourcesPath;
}

void Terminal::initialize()
{
	string shadersPath = resourcesPath + "include/shaders/";

	// SDL window and OpenGL context initialization
	cout << "Initializing SDL" << endl;

	if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO) == -1)
	{
		cout << "SDL initialization failed " << SDL_GetError();
		exit(-1);
	}

	atexit(SDL_Quit);
	screen = SDL_CreateWindow("Omar Window", 0, 0, 900, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_MaximizeWindow(screen);
	
	if (screen == NULL)
	{
		cout << "SDL couldn't set video mode" << endl << SDL_GetError() << endl;
		exit(-1);
	}

	SDL_GLContext context = SDL_GL_CreateContext(screen);
	SDL_Event userEvent;
	cout << "SDL initialized" << endl;

	SDL_SetRelativeMouseMode(SDL_FALSE);
	
	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		exit(-1);
	}
	
	SDL_GetWindowSize(screen, &contextWidth, &contextHeight);

	//~ contextWidth = dm.w;
	//~ contextHeight = dm.h;
	cout << "Resolution is " << contextWidth << "/" << contextHeight << endl;
	
	width = floor(contextWidth / tileSize);
	height = floor(contextHeight / tileSize);
	
	// Initializing GLAD, make sure it's after the OpenGL context initialization
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		exit(-1);
	}
	
	// opengl logic
	glViewport(0, 0, dm.w, dm.h);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	content = TileContainer(width, height, fillSymbol, textColor, tileColor);
	
	allocateVertices(&vertices, width, height);
	generateVertices(vertices, content, width, height);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, width * height * 60 * sizeof(float), vertices, GL_STATIC_DRAW);
	
	// position coordinates
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// text color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	// tile color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(3);
	
	glBindVertexArray(VAO);

	// loading font bitmap
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	// texture wrapping/filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int imgWidth, imgHeight, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load((resourcesPath + "consolas-mono-16x.bmp").c_str(), &imgWidth, &imgHeight, &nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load font texture" << endl;
	}
	
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glm::mat4 projection = glm::ortho(0.0f, (float)contextWidth, (float)contextHeight, 0.0f);
	glm::mat4 fontProjection = glm::ortho(0.0f, 256.0f, 224.0f, 0.0f);

	Shader sceneShader(shadersPath + "shader.vert", shadersPath + "shader.frag");
	sceneShader.use();

	sceneShader.setUniform("fontTexture", 1);
	sceneShader.setUniform("projection", projection);
	sceneShader.setUniform("fontProjection", fontProjection);
}

void Terminal::draw()
{
	processInput(event);
	if (showFps)
		displayFps();
		
	generateVertices(vertices, content, width, height);
	glBufferData(GL_ARRAY_BUFFER, width * height * 60 * sizeof(float), vertices, GL_STATIC_DRAW);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, width * height * 60);

	SDL_GL_SwapWindow(screen);
}

void Terminal::clear()
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			setChar(x, y, ' ');
			setTextColor(x, y, textColor);
			setTileColor(x, y, tileColor);
		}
	}
}

// getter methods
char Terminal::getChar(int x, int y) 
{
	return content.getChar(x, y);
}

Color Terminal::getTextColor(int x, int y)
{
	return content.getTextColor(x, y);
}

Color Terminal::getTileColor(int x, int y)
{
	return content.getTileColor(x, y);
}

Point Terminal::getPixelDimensions()
{
	return Point(contextWidth, contextHeight);
}

Point Terminal::getTileDimensions()
{
	return Point(width, height);
}

// setter methods
void Terminal::setChar(int x, int y, char symbol)
{
	content.setChar(x, y, symbol);
}

void Terminal::setTextColor(int x, int y, int r, int g, int b)
{
	content.setTextColor(x, y, r, g, b);
}

void Terminal::setTextColor(int x, int y, Color color)
{
	setTextColor(x, y, color.r, color.g, color.b);
}

void Terminal::setTileColor(int x, int y, int r, int g, int b)
{
	content.setTileColor(x, y, r, g, b);
}

void Terminal::setTileColor(int x, int y, Color color)
{
	setTileColor(x, y, color.r, color.g, color.b);
}

void Terminal::setString(int x, int y, string str)
{
	content.setString(x, y, str);
}

void Terminal::setString(int x, int y, string str, Color textColor)
{
	content.setString(x, y, str, textColor);
}

void Terminal::setString(int x, int y, string str, int textR, int textG, int textB)
{
	content.setString(x, y, str, Color(textR, textG, textB));
}

void Terminal::setString(int x, int y, string str, Color textColor, Color tileColor)
{
	content.setString(x, y, str, textColor, tileColor);
}

void Terminal::setString(int x, int y, string str, int textR, int textG, int textB, int tileR, int tileG, int tileB)
{
	content.setString(x, y, str, Color(textR, textG, textB), Color(tileR, tileG, tileB));
}

void Terminal::blit(TileContainer& otherCard, int xOffset, int yOffset)
{
	content.blit(otherCard, xOffset, yOffset);
}

Point Terminal::screenToTilePosition(int x, int y)
{
	return Point(floor(x / tileSize), floor(y / tileSize));
}

Point Terminal::screenToTilePosition(Point point)
{
	return screenToTilePosition(point.x, point.y);
}

unsigned int Terminal::getTicks()
{
	return SDL_GetTicks();
}

Point Terminal::getMousePosTile()
{
	return screenToTilePosition(mousePos);
}

Point Terminal::getClickPosTile()
{
	return screenToTilePosition(clickPos);
}

Point Terminal::getMousePosScreen()
{
	return mousePos;
}

Point Terminal::getClickPosScreen()
{
	return clickPos;
}

bool Terminal::isLClickHeld()
{
	return leftClickHeld;
}

bool Terminal::isRClickHeld()
{
	return rightClickHeld;
}

char Terminal::getKeyPress()
{
	return keyPress;
}

void Terminal::StartTextInput()
{
	SDL_StartTextInput();
}

void Terminal::StopTextInput()
{
	SDL_StopTextInput();
}
string Terminal::getTextInput()
{
	return textInput;
}

void Terminal::processInput(SDL_Event event)
{
	cleanInputVariables();
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				int clickX;
				int clickY;
				SDL_GetMouseState(&clickX, &clickY);
				clickPos.x = clickX;
				clickPos.y = clickY;
			break;
			
			case SDL_KEYDOWN:
				keyPress = char(event.key.keysym.sym);
			break;
			
			case SDL_TEXTINPUT:
				textInput = event.text.text;
			break;
			
			case SDL_MOUSEMOTION:
				int mouseX;
				int mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				mousePos.x = mouseX;
				mousePos.y = mouseY;
			break;
		}
	}
}

void Terminal::cleanInputVariables()
{
	clickPos.x = -1;
	clickPos.y = -1;
	
	keyPress = (char)-1;
	textInput = "";
}
