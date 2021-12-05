#pragma once

#include <vector>
#include <string>

#include <src/Clickable.h>
#include <src/Hoverable.h>
#include <src/Keypressable.h>
#include <src/TileContainer.h>
#include <src/Point.h>

using namespace omar;

class Panel : public TileContainer, public Clickable, public Hoverable, public Keypressable
{
private:
	string name;
	Panel* parent; // Is this property even necessary? Children seemingly never act on their parents
	vector<Panel*> children;
	vector<Clickable*> clickSubscribers;
	vector<Hoverable*> hoverSubscribers;
	vector<Keypressable*> keypressSubscribers;
	Point position;
public:
	Panel(int Width, int Height, string Name, Point Position, char FillSymbol); // some parameters have default values, check .cpp file
	
	void blitInto(TileContainer& otherContainer);
	
	void addChild(Panel* child);
	void removeChild(Point pos);
	void removeChild(string childName);
	
	void setParent(Panel* newParent);
	
	void subscribeClick(Clickable& subscriber);
	void subscribeHover(Hoverable& subscriber);
	void subscribeKeypress(Keypressable& subscriber);

	void click(string name, Point clickPos);
	void hover(string name, Point hoverPos);
	void keypress(string name, char keypress);
	
	string getName();
	
	void setPosition(Point newPosition);
	Point getPosition();
};
