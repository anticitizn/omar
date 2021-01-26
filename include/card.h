#ifndef CARD
#define CARD

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

class Card {
public:
	Card(int width, int height, char fillSymbol, glm::vec4 fillTileColor, glm::vec4 fillTextColor);

	int width;
	int height;
	bool blitTransparency = true;
	
	// copies another card's content into itself starting at the specified x and y offsets
	void blit(Card derivedCard, int xOffset, int yOffset);
	
	void resize(int newx, int newy);
	
	void setChar(int x, int y, char symbol);
	void setTileColor(int x, int y, glm::vec4 color);
	void setTextColor(int x, int y, glm::vec4 color);
	
	char getChar(int x, int y);
	glm::vec4 getTileColor(int x, int y);
	glm::vec4 getTextColor(int x, int y);
	
	void saveCard(string path);
	void loadCard(string path);
	
	string getContent();
	void setContent(string newContent);
	
	string content;
private:
	vector<glm::vec4> tileColorVec;
	vector<glm::vec4> textColorVec;
	
	glm::vec4 fillTileColor;
	glm::vec4 fillTextColor;
	char fillSymbol;
};

#endif
