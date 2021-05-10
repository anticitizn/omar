#ifndef OMAR_COLOR
#define OMAR_COLOR

namespace omar
{
	struct Color
	{
		Color(int R, int G, int B)
		{
			r = R;
			g = G;
			b = B;
		}
		
		Color() { }
		
		int r;
		int g;
		int b;
	};
}

#endif
