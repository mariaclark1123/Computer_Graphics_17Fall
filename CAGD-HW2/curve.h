#define PRECISION 1e -3
#define PI 3.1415926
#define MAX_CHAR 128

typedef float Point[2];

typedef struct CubicBezierCurve 
{
	Point control_pts[4];
}CubicBezierCurve;

enum XYZ { X = 0, Y, Z };
#define SET_PT2( V, V1, V2 ) do { (V)[X] = (V1); (V)[Y] = (V2); } while (0)
#define SET_MIDDLE(V, V1, V2) do { (V)[X] = ((V1)[X] + (V2)[X]) / 2.0; (V)[Y] = ((V1)[Y] + (V2)[Y]) / 2.0; } while (0)
#define SET_VECTOR(V, V1, V2) do { (V)[X] = ((V1)[X] - (V2)[X]); (V)[Y] = ((V1)[Y] - (V2)[Y]); } while (0)
#define SET_VECTOR_VER(V, V1) do { (V)[X] = (V1)[Y]; (V)[Y] = -(V1)[X]; } while (0)

void evaluate(const CubicBezierCurve *curve, const float t, Point value);