#ifndef OMAR_POINT
#define OMAR_POINT

namespace omar
{
	struct Point
	{
		Point(int X, int Y)
		{
			x = X;
			y = Y;
		}
		
		Point() { }
		
		int x;
		int y;
	};
}

#endif
