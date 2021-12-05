#ifndef omar_utils_clickable
#define omar_utils_clickable

#include <string>
#include <src/Point.h>

using namespace std;
using namespace omar;

class Clickable
{
public:
	void virtual click(string name, Point clickPos) { }
};

#endif
