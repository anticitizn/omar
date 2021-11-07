#pragma once

#include <vector>
#include <string>

#include <src/Clickable.h>
#include <src/TileContainer.h>
#include <src/Point.h>

using namespace omar;

class Panel : public TileContainer, public Clickable
{
private:
	string name;
	Panel* parent; // Is this property even necessary? Children seemingly never act on their parents
	vector<Panel*> children;
	Point position;
public:
	Panel(int Width, int Height, string Name, Point Position, char FillSymbol); // some parameters have default values, check .cpp file
	
	void blitInto(TileContainer& otherContainer);
	
	void addChild(Panel* child);
	void removeChild(Point pos);
	void removeChild(string childName);
	
	void setParent(Panel* newParent);

	void onClick(Point clickPos);
	void onHover(Point hoverPos);
	
	string getName();
	
	void setPosition(Point newPosition);
	Point getPosition();
};
