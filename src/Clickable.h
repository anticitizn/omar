#include <src/Point.h>

using namespace omar;

class Clickable
{
public:
	bool virtual click(Point clickPos) = 0;
	bool virtual hover(Point hoverPos) = 0;
};
