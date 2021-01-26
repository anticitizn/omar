#include "omar.h"

using namespace std;

int main(int argc, char* argv[])
{
	Terminal window;
	window.initialize(16.0f, ' ');
	window.setChar(10, 10, '@');
	window.setChar(20, 20, 'w');
	window.setChar(22, 20, 'w');
	window.setChar(30, 30, 'O');
	while(true)
		window.draw();
}


