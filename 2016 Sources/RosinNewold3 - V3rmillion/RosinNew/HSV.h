#pragma once

#include <math.h>

namespace Rosin {
	namespace HSV {
		int Clamp(int i) {
			if (i < 0)
				return 0;
			if (i >(int)255)
				return 255;
			return i;
		}

		void HsvToRgb(double h, double S, double V, int& r, int& g, int& b) {
			double num1 = h;
			while (num1 < 0.0)
				num1 += 360.0;
			while (num1 >= 360.0)
				num1 -= 360.0;
			double num2;
			double num3;
			double num4;
			if (V <= 0.0)
			{
				double num5;
				num2 = num5 = 0.0;
				num3 = num5;
				num4 = num5;
			}
			else if (S <= 0.0)
			{
				double num5;
				num2 = num5 = V;
				num3 = num5;
				num4 = num5;
			}
			else
			{
				double d = num1 / 60.0;
				int num5 = (int)floor(d);
				double num6 = d - (double)num5;
				double num7 = V * (1.0 - S);
				double num8 = V * (1.0 - S * num6);
				double num9 = V * (1.0 - S * (1.0 - num6));
				switch (num5)
				{
				case -1:
					num4 = V;
					num3 = num7;
					num2 = num8;
					break;
				case 0:
					num4 = V;
					num3 = num9;
					num2 = num7;
					break;
				case 1:
					num4 = num8;
					num3 = V;
					num2 = num7;
					break;
				case 2:
					num4 = num7;
					num3 = V;
					num2 = num9;
					break;
				case 3:
					num4 = num7;
					num3 = num8;
					num2 = V;
					break;
				case 4:
					num4 = num9;
					num3 = num7;
					num2 = V;
					break;
				case 5:
					num4 = V;
					num3 = num7;
					num2 = num8;
					break;
				case 6:
					num4 = V;
					num3 = num9;
					num2 = num7;
					break;
				default:
					double num10;
					num2 = num10 = V;
					num3 = num10;
					num4 = num10;
					break;
				}
			}
			r = Clamp((int)(num4 * (double)255));
			g = Clamp((int)(num3 * (double)255));
			b = Clamp((int)(num2 * (double)255));
		}
	}
}