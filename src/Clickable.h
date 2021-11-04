#include <src/Point.h>

class Clickable
{
public:
	bool click(Position clickPos) = 0;
	bool hover(Position hoverPos) = 0;
}
