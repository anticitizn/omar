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
	InterfaceElement(int Width, int Height, string Name, Point Position, char FillSymbol, InterfaceElement* Parent); // some parameters have default values, check .cpp file
	
	void addChild(InterfaceElement& child);
	void removeChild(Point pos);
	void removeChild(string childName);
	
	void blitInto(TileContainer& otherContainer);
	
	string getName();
	void setPosition(Point newPosition);
	Point getPosition();
};
