#include "omar.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	Terminal window;
	window.initialize(16.0f, ' ');
	window.setChar(0, 0, '1');
	window.setChar(10, 10, 'w');
	
	while(true)
	{
		window.draw();
	}
}
