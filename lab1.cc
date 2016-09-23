#include <stdio.h>
#include <math.h>
#include <string>
#include <cstdlib>
class Point2
{
private:
	float x;
	float y;
	Point2(float _x, float _y): x(_x), y(_y) {};
public:
	float getx() { return x;};
	float gety() { return y;};
	static Point2 Polar(float, float);
	Point2() { x = y = 0.0;};
};
inline Point2 Point2::Polar(float R, float Angle)
{
	Point2 p(R * cos(Angle), R * sin(Angle));
	
	return p;
};
int main(int argc, char* argv[])
{
	float edge = 1.0;
	float angle = 0.0;
	float pi = acos(-1.0);
	float radius;
	int i = 0;
	Point2 vertex[8];

	if(argc > 1)
		edge = atof(argv[1]);
	radius = edge / sqrt(2 * (1 - cos(pi / 4)));
	while(i < 8)
	{
		vertex[i] = Point2::Polar(radius, angle);
		angle += pi / 4.0;
		i++;
	}
	do
	{
		--i;
		printf("x%d = %f y%d = %f\n", 8 - i, vertex[i].getx(), 8 - i , vertex[i].gety());
	}while(i > 0);
	
	return 0;
}







