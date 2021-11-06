#include <iostream>

#include <src/omar.h>
#include <src/TileContainer.h>
#include <src/InterfaceElement.h>
#include <src/Button.h>
#include <src/Point.h>
#include <src/TestClass.h>

using namespace std;
using namespace omar;

int main()
{   
	omar::Terminal window(16.0f, ' ', "data/");
	window.initialize();
	
	// these constructors are really stupid
	InterfaceElement mainWindow(50, 30, "main_window", Point(3, 3), '.');
	
	InterfaceElement childWindow1(20, 20, "child_window1", Point(5, 5), ' ');
	childWindow1.blitTransparency = false;
	
	Button button(20, 20, "child_window2", Point(25, 5), 'o');
	button.blitTransparency = false;

	TestClass test;
	button.subscribe(test);
	
	childWindow1.setChar(5, 5, '@');
	childWindow1.setChar(6, 5, '@');
	childWindow1.setChar(5, 6, '@');
	childWindow1.setChar(6, 6, '@');
	
	mainWindow.addChild(&childWindow1);
	mainWindow.addChild(&button);
	
	TileContainer finalWindow(120, 120, ' ', Color(255, 255, 255), Color(0, 0, 0));
	mainWindow.blitInto(finalWindow);
	window.blit(finalWindow, 0, 0);
	
	window.showFps = true;
	
	int x_pos = window.getTileDimensions().x / 3;
	int y_pos = window.getTileDimensions().y / 2;
	
	window.setString(x_pos, y_pos, "OpenGL ", Color(120, 255, 87));
	window.setString(x_pos + 7, y_pos, "bitMap ", Color(255, 236, 110));
	window.setString(x_pos + 14, y_pos, "ASCII ", Color(255, 102, 134));
	window.setString(x_pos + 20, y_pos, "Renderer", Color(180, 120, 255));
	window.setString(x_pos + 3, y_pos + 2, "easy - portable - cool");

	for (int i = x_pos; i < x_pos + 28; i++)
	{
		window.setTileColor(i, y_pos, 50, 50, 50);
	}
	
	bool running = true;
	while (running)
	{
		if (window.isLClickHeld())
		{
			mainWindow.onClick(window.getMousePosTile());
		}
		window.setString(0, 2, to_string(window.getMousePosTile().x));
		window.setString(0, 3, to_string(window.getMousePosTile().y));
		window.setChar(0, 1, window.getKeyPress() > 0 ? window.getKeyPress() : ' ');
		
		if(window.getKeyPress() == 27)
			running = false;
		
		window.draw();
		
		window.setString(0, 1, "     ");
		window.setString(0, 2, "     ");
		window.setString(0, 3, "     ");
	}
}
