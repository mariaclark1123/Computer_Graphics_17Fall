#ifndef _DECASTELJAU_H_

#define _DECASTELJAU_H_



#include <vector>

using namespace std;



class Point2D

{

public:

	Point2D(float xx = 0, float yy = 0) : x(xx), y(yy) {}



	Point2D scale(float t) { return Point2D(x * t, y * t); }

	Point2D& add(const Point2D &other) { x += other.x; y += other.y; return *this; }



public:

	float x;

	float y;

};



typedef vector<Point2D> Point2DVector;



Point2D deCasteljau(const Point2DVector& points, float u)

{

	// input : array P[0:n] of n+1 points and real number u in [0,1]

	// output : point on curve

	// working : point array Q[0:n]



	Point2DVector temp = points;
	int n = points.size() - 1;
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 0; j < n - i + 1; ++j)
		{
			temp[j] = temp[j].scale(1 - u);
			temp[j].add(temp[j + 1].scale(u));
		}
	}
	return temp[0];
}
#endif
