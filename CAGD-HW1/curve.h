#define PRECISION 1e -3
#define PI 3.1415926
#define MAX_CHAR 128

typedef float Point[2];

typedef struct CubicBezierCurve 
{
	Point control_pts[4];
}CubicBezierCurve;

enum XYZ { X = 0, Y, Z };

#define SET_PT2( V, V1, V2 ) do { (V)[0] = (V1); (V)[1] = (V2); } while (0)

void evaluate(const CubicBezierCurve *curve, const float t, Point value);