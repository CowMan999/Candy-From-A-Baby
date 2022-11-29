#include <cmath>
#define pi_num 3.14159265

float dir_to_coords_x(float angle)
{
    return cos(angle*pi_num/180);
}

float dir_to_coords_y(float angle)
{
    return sin(angle*pi_num/180);
}

float coords_to_dir(float x, float y, float px, float py)
{
    float dx = x - px;
    float dy = y - py;

    return ((atan2(dy, dx)) * 180 / pi_num) + 180;
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float lerp_angle(float a, float b, float t)
{
	// determine rotation direction - ternary style
	b = (b-a > 180 ? b-a - 360 : b-a) + a;
	b = (b-a < -180 ? b-a + 360 : b-a) + a;
    
    return a + t * (b - a);
}