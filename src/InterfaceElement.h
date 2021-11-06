#pragma once

#include <vector>
#include <string>

#include <src/Clickable.h>
#include <src/TileContainer.h>
#include <src/Point.h>

using namespace omar;

class InterfaceElement : public TileContainer, public Clickable
{
private:
	string name;
	InterfaceElement* parent; // Is this property even necessary? Children seemingly never act on their parents
	vector<InterfaceElement*> children;
	Point position;
public:
	InterfaceElement(int Width, int Height, string Name, Point Position, char FillSymbol); // some parameters have default values, check .cpp file
	
	void blitInto(TileContainer& otherContainer);
	
	void addChild(InterfaceElement* child);
	void removeChild(Point pos);
	void removeChild(string childName);
	
	void setParent(InterfaceElement* newParent);

	void onClick(Point clickPos);
	void onHover(Point hoverPos);
	
	string getName();
	
	void setPosition(Point newPosition);
	Point getPosition();
};
