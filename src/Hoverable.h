#ifndef omar_utils_hoverable
#define omar_utils_hoverable

#include <string>
#include <src/Point.h>

using namespace std;
using namespace omar;

class Hoverable
{
public:
	void virtual hover(string name, Point clickPos) { }
};

#endif
