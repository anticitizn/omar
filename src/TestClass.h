#pragma once

#include <vector>
#include <iostream>

#include <src/Clickable.h>

using namespace std;
using namespace omar;

class TestClass : public Clickable
{
private:
    void outputText()
    {
        cout << "Test method bound to button called!" << endl;
    }
public:
    void onClick(Point clickPos)
    {
        outputText();
    }
	void onHover(Point hoverPos)
    {

    }
};
