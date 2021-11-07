#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <src/Panel.h>
#include <src/Clickable.h>

using namespace omar;

class Button : public Panel
{
private:
	vector<Clickable*> subscribed; // please like and subscribe
public:
	Button(int Width, int Height, string Name, Point Position, char FillSymbol);
    void subscribe(Clickable& subscriber);
    void unsubscribe(Clickable& subscriber);
    void onClick(Point clickPos) override;
	void onHover(Point hoverPos) override;
};
