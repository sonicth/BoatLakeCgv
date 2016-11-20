/*
	perlin nose

	the following code has been derived from Perlin Noise page pseudocode,
	that was referenced by landscape practical, #8

	no rendering will be done from this funtion, no GL functions
*/

#include "open.h"
//#include <stdlib.h>
//#include <stdio.h>
extern double cosInterpolate(double, double, double);

///////////////////////////////
//perlin noise bit
///////////////////////////////
double Noise(int x, int y)
{
	long n;
    n = x + y * 57;
	n = (n % 13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

double SmoothNoise(int x, int y)
{
	double corners, sides, center;

    corners = ( Noise(x-1, y-1)+Noise(x+1, y-1)+Noise(x-1, y+1)+Noise(x+1, y+1) ) / 16.0;
    sides   = ( Noise(x-1, y)  +Noise(x+1, y)  +Noise(x, y-1)  +Noise(x, y+1) ) /  8.0;
    center  =  Noise(x, y) / 4.0;
    return corners + sides + center;
}

double InterpolateNoise(double x, double y)
{
	//integer x and integer y
	int intX, intY;
	//fractional x and y
	double fractX, fractY, v1, v2, v3, v4, i1, i2;

    intX = (int)x;
    fractX = x - intX;

    intY = (int)y;
	fractY = y - intY;

      v1 = SmoothNoise(intX,     intY);
      v2 = SmoothNoise(intX + 1, intY);
      v3 = SmoothNoise(intX,     intY + 1);
      v4 = SmoothNoise(intX + 1, intY + 1);

      i1 = INTERPOLATE(v1 , v2 , fractX);
      i2 = INTERPOLATE(v3 , v4 , fractX);

      return INTERPOLATE(i1 , i2 , fractY);
}

double PerlinNoise_2D(double x, double y)
{
	double total = 0, p, frequency, amplitude;
	int n, i;

	p = PERSISTENCE;
	n = NUM_OF_OCTAVES - 1;

	for (i = 0; i < n; i++)
	{
		frequency = pow(2, i);
		amplitude = pow(p, i);

          total = total + InterpolateNoise(x * frequency, y * frequency) * amplitude;
	}
	return total;
}

