#ifndef omar_utils_clickable
#define omar_utils_clickable

#include <src/Point.h>

using namespace omar;

class Clickable
{
public:
	void virtual onClick(Point clickPos) { }
	void virtual onHover(Point hoverPos) { }
};

#endif