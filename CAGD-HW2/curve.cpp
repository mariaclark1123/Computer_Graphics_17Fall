#include "curve.h"
#include <iostream>
#include <math.h>
#include <gl/freeglut.h>

#define SET_VECTOR2(V, V1, V2)	do { (V)[X] = (V1); V[Y] = (V2); } while (0)
#define COPY_PT(DST,SRC)		do { DST[X] = SRC[X]; DST[Y] = SRC[Y]; } while (0)
#define VECTOR2_SCALA_ADD(O, V, S)		do { O[X] += (S) * (V)[X]; O[Y] += (S) * V[Y]; } while (0)

void evaluate(const CubicBezierCurve *curve, const float t, Point value)
{
	const float t_inv = 1.0 - t;
	const float t_inv_sq = t_inv * t_inv;
	const float t_sq = t * t;
	const float b0 = t_inv_sq * t_inv;
	const float b1 = 3 * t_inv_sq * t;
	const float b2 = 3 * t_inv * t_sq;
	const float b3 = t_sq * t;
	SET_VECTOR2(value, 0, 0);
	VECTOR2_SCALA_ADD(value, curve->control_pts[0], b0);
	VECTOR2_SCALA_ADD(value, curve->control_pts[1], b1);
	VECTOR2_SCALA_ADD(value, curve->control_pts[2], b2);
	VECTOR2_SCALA_ADD(value, curve->control_pts[3], b3);
}