#ifndef OMAR
#define OMAR

#define xyPos y*width + x

#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include "SDL.h"

#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glad/glad.h"
#include "include/stb_image.h"

#include "include/Tile.h"
#include "include/Point.h"
#include "include/Color.h"
#include "include/shaders/shader.h"

using namespace std;

namespace omar
{

class Terminal
{
private:
	SDL_Window* screen;
	
	// the path to the font image and the shaders
	string resourcesPath;
	
	char fillSymbol = ' ';
	
	// dimensions of one tile, overwritten by constructor
	float tileSize = 16.0f;
	
	// dimensions in pixels
	int contextWidth = 0;
	int contextHeight = 0;
	
	// dimensions in tiles
	int width = 0;
	int height = 0;

	// width and height of a singular character on the font
	float fontSymbolWidth = 16.0f;
	float fontSymbolHeight = 14.0f;

	Color textColor = Color(255, 255, 255);
	Color tileColor = Color(0, 0, 0);

	Tile* tiles;
	float* vertices;
	
	void generateTiles(Tile** tiles, const int width, const int height)
	{
		*tiles = (Tile*) malloc(width * height * sizeof(Tile));
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				setChar(x, y, ' ');
				setTextColor(x, y, textColor);
				setTileColor(x, y, tileColor);
			}
		}
	}
	
	void allocateVertices(float** vertices, const int width, const int height)
	{
		int nFloats = 60; // amount of floats for every quad
		*vertices = (float*) malloc(width * height * nFloats * sizeof(float));
	}
	
	void generateVertices(float* vertices, Tile* tiles, const int width, const int height)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				generateQuad(vertices + (xyPos) * 60, tiles[xyPos], x, y, tileSize, fontSymbolWidth, fontSymbolHeight);
			}
		}
	}
	
	void generateQuad(float* vertices, const Tile& tile, const int x, const int y, const int tileSize, const int fontSymbolWidth, const int fontSymbolHeight)
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

		// text color
		addColorInfo(&vertices[4], tile.textColor);

		// tile color
		addColorInfo(&vertices[7], tile.tileColor);

		// right bottom
		vertices[10] = tileSize * x + tileSize;
		vertices[11] = tileSize * y;
		vertices[12] = x2;
		vertices[13] = y1;

		// text color
		addColorInfo(&vertices[14], tile.textColor);

		// tile color
		addColorInfo(&vertices[17], tile.tileColor);

		// left bottom
		vertices[20] = tileSize * x;
		vertices[21] = tileSize * y;
		vertices[22] = x1;
		vertices[23] = y1;

		// text color
		addColorInfo(&vertices[24], tile.textColor);

		// tile color
		addColorInfo(&vertices[27], tile.tileColor);

		// left bottom
		vertices[30] = tileSize * x;
		vertices[31] = tileSize * y;
		vertices[32] = x1;
		vertices[33] = y1;

		// text color
		addColorInfo(&vertices[34], tile.textColor);

		// tile color
		addColorInfo(&vertices[37], tile.tileColor);

		// left top
		vertices[40] = tileSize * x;
		vertices[41] = tileSize * y + tileSize;
		vertices[42] = x1;
		vertices[43] = y2;

		// text color
		addColorInfo(&vertices[44], tile.textColor);

		// tile color
		addColorInfo(&vertices[47], tile.tileColor);

		// right top
		vertices[50] = tileSize * x + tileSize;
		vertices[51] = tileSize * y + tileSize;
		vertices[52] = x2;
		vertices[53] = y2;

		// text color
		addColorInfo(&vertices[54], tile.textColor);

		// tile color
		addColorInfo(&vertices[57], tile.tileColor);
	}
	
	void addColorInfo(float* vertices, const Color& color)
	{
		vertices[0] = (float)color.r / 255.0f;
		vertices[1] = (float)color.g / 255.0f;
		vertices[2] = (float)color.b / 255.0f;
	}
	
public:
	Terminal(float TileSize, char FillSymbol, string ResourcesPath)
	{
		tileSize = TileSize;
		fillSymbol = FillSymbol;
		resourcesPath = ResourcesPath;
	}

	void initialize()
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
		screen = SDL_CreateWindow("Omar Window", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);
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

		contextWidth = dm.w;
		contextHeight = dm.h;
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
		
		generateTiles(&tiles, width, height);
		allocateVertices(&vertices, width, height);
		generateVertices(vertices, tiles, width, height);
		
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

	void draw()
	{
		generateVertices(vertices, tiles, width, height);
		glBufferData(GL_ARRAY_BUFFER, width * height * 60 * sizeof(float), vertices, GL_STATIC_DRAW);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, width * height * 60);

		SDL_GL_SwapWindow(screen);
	}
	
	void clear()
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
	char getChar(int x, int y) 
	{
		if (x < width && y < height && x >= 0 && y >= 0)
			return tiles[xyPos].symbol;
		else
			return (char)0;
	}

	Color getTextColor(int x, int y)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
			return tiles[xyPos].textColor;
		else
			return Color(0, 0, 0);
	}

	Color getTileColor(int x, int y)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
			return tiles[xyPos].tileColor;
		else
			return Color(0, 0, 0);
	}
	
	// setter methods
	void setChar(int x, int y, char symbol)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			 tiles[xyPos].symbol = symbol;
		}
	}
	
	void setTextColor(int x, int y, int r, int g, int b)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			 tiles[xyPos].textColor = Color(r, g, b);
		}
	}
	
	void setTextColor(int x, int y, Color color)
	{
		setTextColor(x, y, color.r, color.g, color.b);
	}
	
	void setTileColor(int x, int y, int r, int g, int b)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			 tiles[xyPos].tileColor = Color(r, g, b);
		}
	}
	
	void setTileColor(int x, int y, Color color)
	{
		setTileColor(x, y, color.r, color.g, color.b);
	}

	void setString(int x, int y, string str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			setChar(x + i, y, str[i]);
		}
	}
	
	void setString(int x, int y, string str, int r, int g, int b)
	{
		for (int i = 0; i < str.length(); i++)
		{
			setChar(x + i, y, str[i]);
			setTextColor(x + i, y, r, g, b);
		}
	}
	
	Point screenToTilePosition(int x, int y)
	{
		return Point(floor(x / tileSize), floor(y / tileSize));
	}
	
	Point screenToTilePosition(Point point)
	{
		return screenToTilePosition(point.x, point.y);
	}
	
	unsigned int getTicks()
	{
		return SDL_GetTicks();
	}
};

}

#endif
