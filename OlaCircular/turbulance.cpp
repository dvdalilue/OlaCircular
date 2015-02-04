#include "noises.h" 
#include "turbulance.h"

double turbulence(float x, float y, double size)
{
    double value = 0.0, initialSize = size;
    
    while(size >= 1)
    {
		float v[2];
		v[0] = x / size;
		v[1]= y / size;
		value += (noiseFunction(v)) * size;
        size /= 2.0;
    }
    
    return(128.0 * value / initialSize);
}