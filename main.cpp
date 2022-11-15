#include <iostream>

#include <src/omar.h>
#include <src/TileContainer.h>
#include <src/Point.h>

using namespace std;
using namespace omar;

int main()
{   
	omar::Terminal window(16.0f, ' ', "data/");
	window.initialize();
	
	window.showFps = true;

	window.setChar(10, 2, '@');
	
	int x = 45;
	int y = 30;
	window.setString(0 + x, 0 + y, "OpenGL ", Color(120, 255, 87));
	window.setString(7 + x, 0 + y, "bitMap ", Color(255, 236, 110));
	window.setString(14+ x, 0 + y, "ASCII ", Color(255, 102, 134));
	window.setString(20+ x, 0 + y, "Renderer", Color(180, 120, 255));
	window.setString(3 + x, 2 + y, "easy - portable - cool");

	for (int i = 0; i < 28; i++)
	{
		window.setTileColor(i + x, 0 + y, 50, 50, 50);
	}

	bool running = true;
	while (running)
	{		
		if(window.getKeyPress() == 27)
			running = false;
		
		window.draw();
	}
}
