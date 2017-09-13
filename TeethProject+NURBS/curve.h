#ifndef _CURVE_H_
#define _CURVE_H_

#define PRECISION   1e-5
#define EPS         1e-6        /* data type is float */
#define RES			64
#define SNUM		21

typedef float REAL;
typedef int INT;

typedef REAL Point3[3];
typedef REAL Point2[2];
typedef int Face3[3];

typedef struct CubicBezierCurve {
	Point3 control_pts[4];
} CubicBezierCurve;

typedef struct BicubicBezierSurface {
	Point3 control_pts[4][4];
} BicubicBezierSurface;

typedef struct SliceBoundary {
	CubicBezierCurve cur[4];             //one slice boundary consists of 4 curves
	Face3 faces[10000];
	Point3 vertices[5000];
	Point3 center;
	INT verticenum = 0;
	INT facenum = 0;
	REAL z, xmin, xmax, ymin, ymax;      //to draw the bound box 
}SB;

enum XYZ { X = 0, Y, Z };

#ifdef DEBUG
void PRINT_CTRLPTS(CubicBezierCurve* crv);
#else
#define PRINT_CTRLPTS(X)
#endif

#define SET_PT2(V, V1, V2) do { (V)[0] = (V1); (V)[1] = (V2); } while (0)
#define SET_PT3(V, V1, V2, V3) do { (V)[0] = (V1); (V)[1] = (V2); (V)[2] = (V3); } while (0)

void draw_curve(CubicBezierCurve curve, INT x);
void evaluate_curve(const CubicBezierCurve *curve, const REAL t, Point2 value);
void evaluate_surface(const BicubicBezierSurface *curve, const REAL t1, const REAL t2, Point3 value);
//void calc_surface(INT res);
//void curvein3d();

#endif /* _CURVE_H_ */