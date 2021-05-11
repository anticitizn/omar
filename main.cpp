#include "omar.h"

#define X_POS 40
#define Y_POS 31

using namespace std;

int main()
{   
	omar::Terminal window(16.0f, ' ', "");
	window.initialize();
	
	window.setString(X_POS, Y_POS, "OpenGL ", 120, 255, 87);
	window.setString(X_POS + 7, Y_POS, "bitMap ", 255, 236, 110);
	window.setString(X_POS + 14, Y_POS, "ASCII ", 255, 102, 134);
	window.setString(X_POS + 20, Y_POS, "Renderer", 180, 120, 255);
	window.setString(X_POS, Y_POS + 1, "                       ");
	window.setString(X_POS + 3, Y_POS + 2, "easy - portable - cool");
	
	for (int i = X_POS; i < X_POS + 28; i++)
	{
		window.setTileColor(i, Y_POS, 50, 50, 50);
	}
	
	while(true)
	{
		window.draw();
	}
}
