#include "card.h"

#define index y*width+x

using namespace std;

Card::Card(int cardWidth, int cardHeight, char initFillSymbol, glm::vec4 initFillTileColor, glm::vec4 initFillTextColor)
{
	width = cardWidth;
	height = cardHeight;
	
	fillTileColor = initFillTileColor;
	fillTextColor = initFillTextColor;
	
	fillSymbol = initFillSymbol;
	
	for (int i = 0; i < width * height; i++)
	{
		content.push_back(fillSymbol);
		tileColorVec.push_back(fillTileColor);
		textColorVec.push_back(fillTextColor);
	}
}

// blits another card's content into itself at the specified x and y offsets
void Card::blit(Card derivedCard, int xOffset, int yOffset)
{	
	for (int y = 0; y < derivedCard.height; y++)
	{
		for (int x = 0; x < derivedCard.width; x++)
		{
			if (derivedCard.blitTransparency && derivedCard.getChar(x, y) == ' ')
			{
				
			}
			else 
			{
				this->setChar(x + xOffset, y + yOffset, derivedCard.getChar(x, y));
			}
		}
	}
}

void Card::resize(int newx, int newy) 
{
	width = newx;
	height = newy;
	content.clear();
	tileColorVec.clear();
	textColorVec.clear();
	for (int i = 0; i < width * height; i++)
	{
		content.push_back(fillSymbol);
		tileColorVec.push_back(fillTileColor);
		textColorVec.push_back(fillTextColor);
	}
}

// setters
void Card::setChar(int x, int y, char symbol)
{
	content[index] = symbol;
}

void Card::setTileColor(int x, int y, glm::vec4 color)
{
	tileColorVec[index] = color;
}

void Card::setTextColor(int x, int y, glm::vec4 color)
{
	textColorVec[index] = color;
}

// getters
char Card::getChar(int x, int y)
{
	return content[index];
}

glm::vec4 Card::getTileColor(int x, int y)
{
	return tileColorVec[index];
}

glm::vec4 Card::getTextColor(int x, int y)
{
	return textColorVec[index];
}

void Card::saveCard(string path)
{
	ofstream cardFile;
	cardFile.open(path);
	
	string cardTextContainer;
	
	vector<string> cardText;
	cardText.push_back("omarcard");
	cardText.push_back(to_string(width));
	cardText.push_back(to_string(height));
	cardText.push_back(getContent());
	
	for (int i = 0; i < cardText.size(); i++)
	{
		cardTextContainer.append(cardText[i]);
		cardTextContainer.append("\n");
	}
	cardFile << cardTextContainer;
	cardFile.close();
}

void Card::loadCard(string path)
{
	// loads a text file and parses its content into the card
	fstream cardFile(path);
	stringstream cardBuffer;
	cardBuffer << cardFile.rdbuf();
	cardFile.close();
	
	vector<string> cardText;
	while (cardBuffer.good())
	{
		string substr;
		getline(cardBuffer, substr, '\n');
		cardText.push_back(substr);
	}
	
	if (cardText[0] == "omarcard")
	{
		cout << "x: " << cardText[1] << " y: " << cardText[2] << " content: " << cardText[3] << endl;
	}
	else {
		cout << "text file is not card-format!" << endl;
	}
	width = stoi(cardText[1]);
	height = stoi(cardText[2]);
	
	content = cardText[3];
	
	/* THIS CAUSES A BAD_ALLOC. WHY?
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//cout << "x: " << x << " y: " << y << " width: " << width << endl;
			//cout << cardText[3] << endl;
			//cout << cardText[3][x+y*width] << endl;
			setChar(x, y, cardText[3][x+y*width]);
		}
	}
	*/
}

string Card::getContent()
{
	return content;
}

void Card::setContent(string newContent)
{
	content = newContent;
}
