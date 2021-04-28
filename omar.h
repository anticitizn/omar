#ifndef OMAR
#define OMAR

#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "include/stb_image.h"

#include "include/shaders/shader.h"
#include <algorithm>

using namespace std;

class Terminal
{
public:

	Terminal(float TileSize, char FillSymbol, string ResourcesPath)
	{
		tileSize = TileSize;
		fillSymbol = FillSymbol;
		resourcesPath = ResourcesPath;
	}
	// how many tiles there are in x and y axis
	int width = 0;
	int height = 0;

	void initialize()
	{
		string shadersPath = resourcesPath + "shaders/";

		// SDL window and OpenGL context initialization
		cout << "Initializing SDL" << endl;

		if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO) == -1)
		{
			cout << "SDL initialization failed " << SDL_GetError();
			exit(-1);
		}

		atexit(SDL_Quit);
		screen = SDL_CreateWindow("Omar Window", 0, 0, SCR_W, SCR_H, SDL_WINDOW_OPENGL);
		if (screen == NULL)
		{
			cout << "SDL couldn't set video mode" << endl << SDL_GetError() << endl;
			exit(-1);
		}

		SDL_GLContext context = SDL_GL_CreateContext(screen);
		SDL_Event userEvent;
		cout << "SDL initialized" << endl;

		SDL_SetRelativeMouseMode(SDL_FALSE);
		
		// Initializing GLAD, make sure it's after the OpenGL context initialization
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			cout << "Failed to initialize GLAD" << endl;
			exit(-1);
		}
		SDL_DisplayMode dm;

		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			exit(-1);
		}

		// int w, h;
		// w = dm.w;
		// h = dm.h;
		SCR_W = dm.w;
		SCR_H = dm.h;
		cout << "Setting viewport to: " << SCR_W << "/" << SCR_H << endl;
		SDL_SetWindowSize(screen, SCR_W, SCR_H);
		
		//	tile vertex vector initialization
		generateVertexVector();
		
		// opengl logic
		glViewport(0, 0, dm.w, dm.h);

		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		
		// This might have to be rerun when regenerating vertex vector
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertVec.size() * sizeof(float), vertVec.data(), GL_STATIC_DRAW);
		// position coordinates
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coordinates
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// color attributes
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(4 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
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

		glm::mat4 projection = glm::ortho(0.0f, (float)SCR_W, (float)SCR_H, 0.0f);
		glm::mat4 fontProjection = glm::ortho(0.0f, 256.0f, 224.0f, 0.0f);

		Shader sceneShader(shadersPath + "shader.vert", shadersPath + "shader.frag");
		sceneShader.use();

		glm::vec4 tileColor = glm::vec4(0.3f, 0.8f, 0.5f, 1.0f);
		sceneShader.setUniform("fontTexture", 1);
		sceneShader.setUniform("projection", projection);
		sceneShader.setUniform("fontProjection", fontProjection);
	}
	
	// generate a sufficiently big vertex array to fill the entire window
	// running this twice creates some issue
	void generateVertexVector()
	{
		vertVec.clear();
		
		width = floor(SCR_W / tileSize);
		height = floor(SCR_H / tileSize);

		float tileIndex = (float)fillSymbol - 32.0f;
		float tileY = floor(tileIndex / fontMaxTileW);
		float tileX = tileIndex - tileY * fontMaxTileW;

		float x1 = 8.0f * tileX;	   // left
		float y1 = 8.0f * tileY;	   // top
		float x2 = 8.0f * (tileX + 1); // right
		float y2 = 8.0f * (tileY + 1); // bottom

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				// right top
				vertVec.push_back(tileSize * j + tileSize);
				vertVec.push_back(tileSize * i + tileSize);
				vertVec.push_back(x2);
				vertVec.push_back(y2);

				// text color
				vertVec.push_back(textColor.x);
				vertVec.push_back(textColor.y);
				vertVec.push_back(textColor.z);
				vertVec.push_back(textColor.w);

				// tile color
				vertVec.push_back(tileColor.x);
				vertVec.push_back(tileColor.y);
				vertVec.push_back(tileColor.z);
				vertVec.push_back(tileColor.w);

				// right bottom
				vertVec.push_back(tileSize * j + tileSize);
				vertVec.push_back(tileSize * i);
				vertVec.push_back(x2);
				vertVec.push_back(y1);

				// text color
				vertVec.push_back(textColor.x);
				vertVec.push_back(textColor.y);
				vertVec.push_back(textColor.z);
				vertVec.push_back(textColor.w);

				// tile color
				vertVec.push_back(tileColor.x);
				vertVec.push_back(tileColor.y);
				vertVec.push_back(tileColor.z);
				vertVec.push_back(tileColor.w);

				// left bottom
				vertVec.push_back(tileSize * j);
				vertVec.push_back(tileSize * i);
				vertVec.push_back(x1);
				vertVec.push_back(y1);

				// text color
				vertVec.push_back(textColor.x);
				vertVec.push_back(textColor.y);
				vertVec.push_back(textColor.z);
				vertVec.push_back(textColor.w);

				// tile color
				vertVec.push_back(tileColor.x);
				vertVec.push_back(tileColor.y);
				vertVec.push_back(tileColor.z);
				vertVec.push_back(tileColor.w);

				// left bottom
				vertVec.push_back(tileSize * j);
				vertVec.push_back(tileSize * i);
				vertVec.push_back(x1);
				vertVec.push_back(y1);

				// text color
				vertVec.push_back(textColor.x);
				vertVec.push_back(textColor.y);
				vertVec.push_back(textColor.z);
				vertVec.push_back(textColor.w);

				// tile color
				vertVec.push_back(tileColor.x);
				vertVec.push_back(tileColor.y);
				vertVec.push_back(tileColor.z);
				vertVec.push_back(tileColor.w);

				// left top
				vertVec.push_back(tileSize * j);
				vertVec.push_back(tileSize * i + tileSize);
				vertVec.push_back(x1);
				vertVec.push_back(y2);

				// text color
				vertVec.push_back(textColor.x);
				vertVec.push_back(textColor.y);
				vertVec.push_back(textColor.z);
				vertVec.push_back(textColor.w);

				// tile color
				vertVec.push_back(tileColor.x);
				vertVec.push_back(tileColor.y);
				vertVec.push_back(tileColor.z);
				vertVec.push_back(tileColor.w);

				// right top
				vertVec.push_back(tileSize * j + tileSize);
				vertVec.push_back(tileSize * i + tileSize);
				vertVec.push_back(x2);
				vertVec.push_back(y2);

				// text color
				vertVec.push_back(textColor.x);
				vertVec.push_back(textColor.y);
				vertVec.push_back(textColor.z);
				vertVec.push_back(textColor.w);

				// tile color
				vertVec.push_back(tileColor.x);
				vertVec.push_back(tileColor.y);
				vertVec.push_back(tileColor.z);
				vertVec.push_back(tileColor.w);
			}
		}
	}

	void draw()
	{
		glBufferData(GL_ARRAY_BUFFER, vertVec.size() * sizeof(float), vertVec.data(), GL_DYNAMIC_DRAW);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, vertVec.size());

		SDL_GL_SwapWindow(screen);
	}
	
	void clear()
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				setChar(x, y, ' ');
				//~ setColor(x, y, textColor);
				//setTileColor(x, y, tileColor);
			}
		}
	}

	// getter methods
	char getChar(int x, int y) 
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			// the beginning of the tile's vertex nformation
			int n = (y * width + x) * 72;

			float tileX = vertVec[n + 26];
			float tileY = vertVec[n + 27];
			float tileIndex = (tileY / 8.0f) * fontMaxTileW + tileX / 8.0f;
			return (char)(tileIndex + 32.0f);
		}
		return (char)-1;
	}

	glm::vec4 getTextColor(int x, int y)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			// the beginning of the tile's vertex nformation
			int n = (y * width + x) * 72;

			float textColorX = vertVec[n + 4];
			float textColorY = vertVec[n + 5];
			float textColorZ = vertVec[n + 6];
			float textColorW = vertVec[n + 7];
			return glm::vec4(textColorX, textColorY, textColorZ, textColorW);
		}
		return glm::vec4(-1.0f);
	}

	glm::vec4 getTileColor(int x, int y)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			// the beginning of the tile's vertex nformation
			int n = (y * width + x) * 72;

			float tileColorX = vertVec[n + 8];
			float tileColorY = vertVec[n + 9];
			float tileColorZ = vertVec[n + 10];
			float tileColorW = vertVec[n + 11];
			return glm::vec4(tileColorX, tileColorY, tileColorZ, tileColorW);
		}
		return glm::vec4(-1.0f);
	}

	// setter methods
	void setChar(int x, int y, char symbol)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			// bitmap font texture data
			float tileMaxW = 16.0f; // number of tiles in a row
			float tileMaxH = 14.0f; // number of tiles in a column

			float tileIndex = (float)symbol - 32.0f;
			float tileY = floor(tileIndex / tileMaxW);
			float tileX = tileIndex - tileY * tileMaxW;

			float x1 = 8.0f * tileX;	 // left
			float y1 = 8.0f * tileY;	 // top
			float x2 = 8.0f * (++tileX); // right
			float y2 = 8.0f * (++tileY); // bottom
			
			// beware, untested change: '+ x' replaced by clamp(), might cause unforeseen issues
			// what the fuck did i mean by this comment
			int loc = 72 * (y * (int)width + clamp(x, 0, width)); 
			
			if (loc + 72 < vertVec.size())
			{
				vertVec[loc + 2] = x2;
				vertVec[loc + 3] = y2;

				vertVec[loc + 14] = x2;
				vertVec[loc + 15] = y1;

				vertVec[loc + 26] = x1;
				vertVec[loc + 27] = y1;

				vertVec[loc + 38] = x1;
				vertVec[loc + 39] = y1;

				vertVec[loc + 50] = x1;
				vertVec[loc + 51] = y2;

				vertVec[loc + 62] = x2;
				vertVec[loc + 63] = y2;
			}
		}
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
			setColor(x + i, y, r, g, b);
		}
	}

	void setColor(int x, int y, float r, float g, float b)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			for (int i = 0; i < 6; i++)
			{
				int loc = 72 * (y * (int)width + x);

				if (loc + 72 < vertVec.size())
				{
					vertVec[loc + i * 12 + 4] = r / 255;
					vertVec[loc + i * 12 + 5] = g / 255;
					vertVec[loc + i * 12 + 6] = b / 255;
					vertVec[loc + i * 12 + 7] = 1.0f;
				}
			}
		}
	}

	void setColor(int x, int y, glm::vec3 color)
	{
		setColor(x, y, color.x, color.y, color.z);
	}

	void setTileColor(int x, int y, float r, float g, float b)
	{
		if (x < width && y < height && x >= 0 && y >= 0)
		{
			for (int i = 0; i < 6; i++)
			{
				int loc = 72 * (y * (int)width + x);

				vertVec[loc + i * 12 + 8]  = r / 255;
				vertVec[loc + i * 12 + 9]  = g / 255;
				vertVec[loc + i * 12 + 10] = b / 255;
				vertVec[loc + i * 12 + 11] = 1.0f;
			}
		}
	}

	void setTileColor(int x, int y, glm::vec3 color)
	{
		setTileColor(x, y, color.x, color.y, color.z);
	}
	
	glm::vec2 screenToTilePosition(int x, int y)
	{
		glm::vec2 temp;
		temp.x = floor(x/tileSize);
		temp.y = floor(y/tileSize);
		return temp;
	}
	
	glm::vec2 screenToTilePosition(glm::vec2 position)
	{
		return screenToTilePosition(position.x, position.y);
	}
	
	Uint32 getTicks()
	{
		return SDL_GetTicks();
	}
	
	private:
		SDL_Window* screen;
		
		char fillSymbol = ' ';
		float tileSize = 16.0f;

		int SCR_W = 600;
		int SCR_H = 800;

		float fontMaxTileW = 16.0f;
		float fontMaxTileY = 14.0f;

		glm::vec4 textColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 tileColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		vector<float> vertVec;
		string resourcesPath;
};

#endif
