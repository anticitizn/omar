#pragma once

#include <vector>

#include <src/TileContainer.h>
#include <src/Point.h>

using namespace omar;

class InterfaceElement : public TileContainer
{
private:
	InterfaceElement* parent;
	vector<InterfaceElement> children;
	Point position;
public:
	InterfaceElement(int Width, int Height, char FillSymbol, InterfaceElement* Parent); // parameters have default values, check .cpp file
	void blit(TileContainer& otherContainer, int xOffset, int yOffset);
	void setPosition(Point newPosition);
	Point getPosition();
};
