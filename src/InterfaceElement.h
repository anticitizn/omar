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
	InterfaceElement* parent; // Is this property even necessary? Children seemingly never act on their parents
	vector<InterfaceElement> children;
	Point position;
public:
	InterfaceElement(int Width, int Height, string Name, Point Position, char FillSymbol); // some parameters have default values, check .cpp file
	
	void blitInto(TileContainer& otherContainer);
	
	void addChild(InterfaceElement& child);
	void removeChild(Point pos);
	void removeChild(string childName);
	
	void setParent(InterfaceElement* newParent);
	
	string getName();
	
	void setPosition(Point newPosition);
	Point getPosition();
};
