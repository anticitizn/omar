#pragma once

#include <vector>
#include <string>

#include <src/TileContainer.h>
#include <src/Point.h>

using namespace omar;

class InterfaceElement : public TileContainer
{
private:
	string name;
	InterfaceElement* parent;
	vector<InterfaceElement> children;
	Point position;
public:
	InterfaceElement(int Width, int Height, string Name, char FillSymbol, InterfaceElement* Parent, Point Position); // parameters have default values, check .cpp file
	void blitInto(InterfaceElement& otherContainer);
	void setPosition(Point newPosition);
	Point getPosition();
};
