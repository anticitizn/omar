#include <iostream>

#include <src/omar.h>
#include <src/TileContainer.h>
#include <src/Panel.h>
#include <src/Point.h>

using namespace std;
using namespace omar;

int main()
{   
	omar::Terminal window(16.0f, ' ', "data/");
	window.initialize();
	
	// these constructors are really stupid
	Panel mainWindow(window.getDimensions().x, window.getDimensions().y, "main_window", Point(0, 0), ' ');
	
	Panel textBox(28, 3, "text_box", Point(40, 30), ' ');
	textBox.blitTransparency = false;
	
	mainWindow.addChild(&textBox);
	
	TileContainer finalWindow(120, 120, ' ', Color(255, 255, 255), Color(0, 0, 0));
	
	window.showFps = true;

	textBox.setChar(10, 2, '@');
	
	textBox.setString(0, 0, "OpenGL ", Color(120, 255, 87));
	textBox.setString(7, 0, "bitMap ", Color(255, 236, 110));
	textBox.setString(14, 0, "ASCII ", Color(255, 102, 134));
	textBox.setString(20, 0, "Renderer", Color(180, 120, 255));
	textBox.setString(3, 2, "easy - portable - cool");

	for (int i = 0; i < 28; i++)
	{
		textBox.setTileColor(i, 0, 50, 50, 50);
	}
	
	mainWindow.blitInto(window);

	bool running = true;
	while (running)
	{
		if (window.isLClickHeld())
		{
			mainWindow.click("", window.getMousePosTile());
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
