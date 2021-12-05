#ifndef omar_utils_keypressable
#define omar_utils_keypressable

#include <string>
#include <src/Point.h>

using namespace std;
using namespace omar;

class Keypressable
{
public:
	void virtual keypress(string name, char key) { }
};

#endif
