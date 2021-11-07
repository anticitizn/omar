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

		int operator ==(Color color)
		{
			if (color.r == r && color.g == g && color.b == b)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		int operator !=(Color color)
		{
			if(color == *this)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	};
}

#endif
