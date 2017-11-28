#include <gl/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "curve.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define SEG 10

float coef1[2] = { 0 }, coef2[2] = { 0 }, coef3[2] = { 0 }, judge[2] = { 0 };

//Coefficients
void update_coef(CubicBezierCurve curve);

//Draw
void DrawDot(Point pt);
void Draw_Point(Point pt);
void Draw_Line(Point pt1, Point pt2);
void DrawCircle(Point center, Point startpt, Point endpt);
void Draw_Biarc(CubicBezierCurve curve, float t_start, float t_junc, float t_end, float offset);
void Draw_Arc(CubicBezierCurve curve, float t_start, float t_end, float offset);
void Draw_Offset(Point center, Point start, Point end, float offset);
void drawString(const char* str);
void Draw_End(CubicBezierCurve curve, int offset);
//Judgement
int quadrant(float x, float y);
int hit_index(CubicBezierCurve *curve, int x, int y);

//Calculation
float Sin_Add(float sin_a, float cos_a, float sin_b, float cos_b);
float Cos_Add(float sin_a, float cos_a, float sin_b, float cos_b);
float CalCurEx(float t);
float Dis(Point pt1, Point pt2);
float Center_dis(CubicBezierCurve curve, float t_start, float t_end);
void Der_NorVec(float t, Point norvec);
void Cal_vertline(Point start, Point end, Point pt1, Point pt2);
void Cir_BCA(CubicBezierCurve curve, float t_start, float t_end);

bool line_intersect(Point pt1, Point pt2, Point pt3, Point pt4, Point inter);
bool Cal_JuncPt(CubicBezierCurve curve, float t_start, float t_end, float *t_junc);

/********************************************
-----------------Coefficient-----------------
*********************************************/
void update_coef(CubicBezierCurve curve)
{
	//CACULATE THE COEFFCIENTS
	for (int i = 0; i < 2; i++)
	{
		/* b1-b0 */
		coef1[i] = curve.control_pts[1][i] - curve.control_pts[0][i];
		/* b2-2b1+b0 */
		coef2[i] = curve.control_pts[2][i] - 2 * curve.control_pts[1][i] + curve.control_pts[0][i];
		/* b3-3b2+3b1-b0 */
		coef3[i] = curve.control_pts[3][i] - 3 * curve.control_pts[2][i] + 3 * curve.control_pts[1][i] - curve.control_pts[0][i];
		/* judge = b^2 - 4ac*/
		judge[i] = 4.0 * coef2[i] * coef2[i] - 4 * coef3[i] * coef1[i];
	}
}

/********************************************
-------------------Drawing-------------------
*********************************************/
void DrawDot(Point pt)
{
	float R = 5.0;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 20; ++i)
		glVertex2f(pt[0] + R*cos(2 * PI / 20 * i), pt[1] + R*sin(2 * PI / 20 * i));
	glEnd();
}

void Draw_Line(Point pt1, Point pt2)
{
	glBegin(GL_LINES);
	glVertex2fv(pt1);
	glVertex2fv(pt2);
	glEnd();
}

void Draw_Point(Point pt)
{
	glBegin(GL_POINTS);
	glVertex2fv(pt);
	glEnd();
}

//Draw arc with circle center and start,end point
void DrawCircle(Point center, Point startpt, Point endpt)
{
	float vector_x[2], vector_y[2];
	float radius1, radius2, radius, theta1, theta2, theta_divide;
	float cos1, cos2, sin_d, cos_d, temp, temp_x, temp_y;
	Point circle_pt[9];
	int i;

	//vector for startpt
	vector_x[0] = startpt[0] - center[0];
	vector_y[0] = startpt[1] - center[1];
	//vector for endpt
	vector_x[1] = endpt[0] - center[0];
	vector_y[1] = endpt[1] - center[1];

	//Get radius
	radius1 = sqrt(vector_x[0] * vector_x[0] + vector_y[0] * vector_y[0]);
	radius2 = sqrt(vector_x[1] * vector_x[1] + vector_y[1] * vector_y[1]);
	radius = max(radius1, radius2);

	//When first point in the first or second quadrant
	if (quadrant(vector_x[0], vector_y[0]) <3)
	{
		//First point
		cos1 = vector_x[0] / radius;
		cos2 = vector_x[1] / radius;
		theta1 = acos(cos1);
		theta2 = acos(cos2);

		//1.If the second point is in the first or second quadrant
		//	theta2 = theta2;
		//2.If the second point is in the third or fourth quadrant
		if (quadrant(vector_x[1], vector_y[1]) > 2)
		{
			temp = 2 * PI - theta2;
			if (fabs(theta1 - temp) < PI)
				theta2 = 2 * PI - theta2;
			else
				theta2 = -theta2;
		}
	}

	//When first point in the third or fourth quadrant
	if (quadrant(vector_x[0], vector_y[0]) > 2)
	{
		cos1 = vector_x[0] / radius;
		cos2 = vector_x[1] / radius;
		theta1 = acos(cos1);
		theta2 = acos(cos2);

		//1.If the second point is in the first or second quadrant
		if (quadrant(vector_x[1], vector_y[1]) < 3)
		{
			//Judge the theta1
			temp = 2 * PI - theta1;
			if (fabs(temp - theta2) < PI)
				theta1 = 2 * PI - theta1;
			else
				theta1 = -theta1;
		}
		//2.If the second point is in the third or fourth quadrant
		if (quadrant(vector_x[1], vector_y[1]) > 2)
		{
			theta1 = 2 * PI - theta1;
			theta2 = 2 * PI - theta2;
		}
	}
	//divide the segement to 16 pieces
	theta_divide = (theta1 - theta2) / 8;
	sin_d = sin(theta_divide);
	cos_d = cos(theta_divide);
	for (i = 0; i < 9; i++)
	{
		//Circle1
		circle_pt[i][0] = center[0] + radius*cos(theta1 - i * theta_divide);
		circle_pt[i][1] = center[1] + radius*sin(theta1 - i * theta_divide);
	}
	//Draw circle
	for (i = 0; i < 8; i++)
	{
		glBegin(GL_LINES);
		{
			glVertex2f(circle_pt[i][0], circle_pt[i][1]);
			glVertex2f(circle_pt[i + 1][0], circle_pt[i + 1][1]);
		}
		glEnd();
	}
}

void Draw_End(CubicBezierCurve curve, int offset)
{
	Point vec_start, vec_end;
	Point ver_start, ver_end;
	Point start_mid, end_mid;
	Point pt_start, pt_end;
	Point s1, s2, e1, e2;

	Der_NorVec(0, vec_start);
	Der_NorVec(1, vec_end);

	ver_start[0] = vec_start[1];
	ver_start[1] = -vec_start[0];

	ver_end[0] = vec_end[1];
	ver_end[1] = -vec_end[0];

	evaluate(&curve, 0, pt_start);
	evaluate(&curve, 1, pt_end);

	//start point
	s1[0] = pt_start[0] + ver_start[0] * offset;
	s1[1] = pt_start[1] + ver_start[1] * offset;

	start_mid[0] = pt_start[0] - vec_start[0] * offset;
	start_mid[1] = pt_start[1] - vec_start[1] * offset;

	s2[0] = pt_start[0] - ver_start[0] * offset;
	s2[1] = pt_start[1] - ver_start[1] * offset;

	//end point
	e1[0] = pt_end[0] + ver_end[0] * offset;
	e1[1] = pt_end[1] + ver_end[1] * offset;

	end_mid[0] = pt_end[0] + vec_end[0] * offset;
	end_mid[1] = pt_end[1] + vec_end[1] * offset;

	e2[0] = pt_end[0] - ver_end[0] * offset;
	e2[1] = pt_end[1] - ver_end[1] * offset;

	DrawCircle(pt_start, s1, start_mid);
	DrawCircle(pt_start, start_mid, s2);
	DrawCircle(pt_end, e1, end_mid);
	DrawCircle(pt_end, end_mid, e2);
}

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall)
	{
		isFirstCall = 0;
		lists = glGenLists(MAX_CHAR);

		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

/********************************************
-----------------Judgement-----------------
*********************************************/
int hit_index(CubicBezierCurve *curve, int x, int y)
{
	int i;
	for (i = 0; i < 4; ++i)
	{
		float tx = curve->control_pts[i][0] - x;
		float ty = curve->control_pts[i][1] - y;
		//In the point area
		if ((tx * tx + ty * ty) < 30)
		{
			printf("mouse location:(%d %d)\n", x, y);
			return i;
		}
	}
	return -1;
}

int quadrant(float x, float y)
{
	if (x >= 0 && y >= 0)
		return 1;
	else if (x < 0 && y > 0)
		return 2;
	else if (x <= 0 && y <= 0)
		return 3;
	else if (x > 0 && y < 0)
		return 4;
}

/********************************************
-----------------Calculation-----------------
*********************************************/
float Sin_Add(float sin_a, float cos_a, float sin_b, float cos_b)
{
	return sin_a*cos_b + cos_a*sin_b;
}

float Cos_Add(float sin_a, float cos_a, float sin_b, float cos_b)
{
	return cos_a*cos_b - sin_a*sin_b;
}

float Dis(Point pt1, Point pt2)
{
	float distance;

	distance = (pt1[0] - pt2[0]) * (pt1[0] - pt2[0]) + (pt1[1] - pt2[1]) * (pt1[1] - pt2[1]);
	distance = sqrt(distance);
	return distance;
}

void Der_NorVec(float t, Point norvec)
{
	//	C'(t)/3  = (b3-3b2+3b1-b0)t^2 + (2b2-4b1+2b0)t+(b1-b0)
	//		  	 = coef3[0] * t ^ 2 + 2 * coef2[0] * t + coef1[0]
	float der_x, der_y, length;

	der_x = coef3[0] * t * t + 2 * coef2[0] * t + coef1[0];
	der_y = coef3[1] * t * t + 2 * coef2[1] * t + coef1[1];
	length = der_x * der_x + der_y * der_y;
	length = sqrt(length);

	norvec[0] = 1.0 * der_x / length;
	norvec[1] = 1.0 * der_y / length;
}

bool line_intersect(Point pt1, Point pt2, Point pt3, Point pt4, Point inter)
{
	float x12 = pt1[0] - pt2[0];
	float x34 = pt3[0] - pt4[0];
	float y12 = pt1[1] - pt2[1];
	float y34 = pt3[1] - pt4[1];

	float c = x12 * y34 - y12 * x34;

	if (fabs(c) < 0.01)
	{
		// No intersection
		return false;
	}
	else
	{
		// Intersection
		float a = pt1[0] * pt2[1] - pt1[1] * pt2[0];
		float b = pt3[0] * pt4[1] - pt3[1] * pt4[0];

		float x = (a * x34 - b * x12) / c;
		float y = (a * y34 - b * y12) / c;

		inter[0] = x;
		inter[1] = y;

		return true;
	}
}

float CalCurEx(float t)
{
	float result_x, result_xx, result_xxx, result_y, result_yy, result_yyy, part1, part2, result;
	//C'(t)/3 = (b1-b0) + t(2b2-4b1+2b0) + t^2(b3-3b2+3b1-b0)
	//C''(t)/6 = (b2-2b1+b0) + t(b3-3b2+3b1-b0)
	//C'''(t)/6 = (b3-3b2+3b1-b0)
	//x'(t)
	result_x = 3 * (coef1[0] + 2 * t * coef2[0] + t*t*coef3[0]);
	//x''(t)
	result_xx = 6 * (coef2[0] + t*coef3[0]);
	//x'''(t)
	result_xxx = 6 * coef3[0];
	//y'(t)
	result_y = 3 * (coef1[1] + 2 * t*coef2[1] + t*t*coef3[1]);
	//y''(t)
	result_yy = 6 * (coef2[1] + t*coef3[1]);
	//y'''(t)
	result_yyy = 6 * coef3[1];

	//(x'(t)*x'(t)+y'(t)*y'(t)*(x'(t)*y'''(t)-x'''(t)*y'(t)) - 3(x'(t)*x''(t)+y'(t)*y''(t))*(x'(t)*y''(t)-x''(t)*y'(t)))
	part1 = (result_x * result_x + result_y * result_y) * (result_x * result_yyy - result_y * result_xxx);
	part2 = 3 * (result_x * result_xx + result_y * result_yy) * (result_x * result_yy - result_y * result_xx);
	result = part1 - part2;
	return result;
}

float Center_dis(CubicBezierCurve curve, float t_start, float t_end)
{
	Point pt_start, pt_end;
	Point vec_start, vec_end;
	Point center1, center2;
	Point vert_start, vert_end; //vertical vector of derivative vector
	Point vertl_start, vertl_end;
	Point mid_start, mid_end;
	float distance;

	evaluate(&curve, t_start, pt_start);
	evaluate(&curve, t_end, pt_end);
	
	//Caculate normal vector of start point and end point
	Der_NorVec(t_start, vec_start);
	Der_NorVec(t_end, vec_end);

	//normal vector * 10
	vert_start[0] = 10 * vec_start[1];
	vert_start[1] = -10 * vec_start[0];

	vert_end[0] = 10 * vec_end[1];
	vert_end[1] = -10 * vec_end[0];

	//Find vertical line of vector_start and vector_end
	for (int i = 0; i < 2; i++)
	{
		vertl_start[i] = pt_start[i] + vert_start[i];
		vertl_end[i] = pt_end[i] + vert_end[i];
	}

	//Find vertical line of start - end line
	Cal_vertline(pt_start, pt_end, mid_start, mid_end);

	//Calculate center1 and center2
	if (line_intersect(mid_start, mid_end, pt_start, vertl_start, center1))
	{
		if (line_intersect(mid_start, mid_end, pt_end, vertl_end, center2))
		{
			distance = Dis(center1, center2);
			return distance;
		}
	}
	else
	{
		if (!line_intersect(mid_start, mid_end, pt_end, vertl_end, center2))
			return 0;
	}
}

void Draw_LineOffset(Point start, Point end, float offset)
{
	glColor3f(0, 1, 0);
	Point vec1, vec2, off1, off2;
	float length;

	vec1[0] = end[0] - start[0];
	vec1[1] = end[1] - start[1];

	vec2[0] = vec1[1];
	vec2[1] = -vec1[0];

	length = vec2[0] * vec2[0] + vec2[1] * vec2[1];
	length = sqrt(length);
	vec2[0] = vec2[0]/length;
	vec2[1] = vec2[1]/length;

	off1[0] = start[0] + offset * vec2[0];
	off1[1] = start[1] + offset * vec2[1];

	off2[0] = end[0] + offset * vec2[0];
	off2[1] = end[1] + offset * vec2[1];

	glLineWidth(2.0);
	Draw_Line(off1, off2);
}

void Draw_Offset(Point center, Point start, Point end, float offset)
{
	glColor3f(0, 1, 0);
	Point vec1, vec2, res_vec1, res_vec2;
	float radius;

	vec1[0] = start[0] - center[0];
	vec1[1] = start[1] - center[1];

	vec2[0] = end[0] - center[0];
	vec2[1] = end[1] - center[1];

	radius = vec1[0] * vec1[0] + vec1[1] * vec1[1];
	radius = sqrt(radius);

	res_vec1[0] = start[0] + vec1[0] / radius * offset;
	res_vec1[1] = start[1] + vec1[1] / radius * offset;

	res_vec2[0] = end[0] + vec2[0] / radius * offset;
	res_vec2[1] = end[1] + vec2[1] / radius * offset;

	DrawCircle(center, res_vec1, res_vec2);
}

void Draw_Biarc(CubicBezierCurve curve, float t_start, float t_junc, float t_end, float offset)
{
	Point pt_start, pt_end, pt_junc;
	Point vec_start, vec_end, vec_junc;
	Point center1, center2;
	Point vert_start, vert_end; //vertical vector of derivative vector
	Point vertl_start, vertl_end;
	Point sj1, sj2, se1, se2;

	evaluate(&curve, t_start, pt_start);
	evaluate(&curve, t_end, pt_end);
	evaluate(&curve, t_junc, pt_junc);

	//Caculate normal vector of start point and end point
	Der_NorVec(t_start, vec_start);
	Der_NorVec(t_end, vec_end);
	Der_NorVec(t_junc, vec_junc);

	//normal vector * 10
	vert_start[0] = 10 * vec_start[1];
	vert_start[1] = -10 * vec_start[0];

	vert_end[0] = 10 * vec_end[1];
	vert_end[1] = -10 * vec_end[0];

	//Find vertical line of vector_start and vector_end
	for (int i = 0; i < 2; i++)
	{
		vertl_start[i] = pt_start[i] + vert_start[i];
		vertl_end[i] = pt_end[i] + vert_end[i];
	}

	//1. Find vertical line of start-junction line
	Cal_vertline(pt_start, pt_junc, sj1, sj2);
	//2. Find vertical line of junction-end line
	Cal_vertline(pt_junc, pt_end, se1, se2);

	if (line_intersect(sj1, sj2, pt_start, vertl_start, center1))
	{
		glColor3f(1, 0, 0);
	//	DrawCircle(center1, pt_start, pt_junc);
		//Draw offset
		Draw_Offset(center1, pt_start, pt_junc, offset);
		Draw_Offset(center1, pt_start, pt_junc, -offset);
	}
	else
	{
		glColor3f(1, 0, 0);
	//	Draw_Line(pt_start, pt_junc);
		Draw_LineOffset(pt_start, pt_junc, offset);
		Draw_LineOffset(pt_start, pt_junc, -offset);
	}
	if (line_intersect(se1, se2, pt_end, vertl_end, center2))
	{
		glColor3f(1, 0, 0);
	//	DrawCircle(center2, pt_junc, pt_end);
		Draw_Offset(center2, pt_junc, pt_end, offset);
		Draw_Offset(center2, pt_junc, pt_end, -offset);
	}
	else
	{
		glColor3f(1, 0, 0);
	//	Draw_Line(pt_junc, pt_end);
		Draw_LineOffset(pt_junc, pt_end, offset);
		Draw_LineOffset(pt_junc, pt_end, -offset);
	}
}

void Draw_Arc(CubicBezierCurve curve, float t_start, float t_end, float offset)
{
	Point pt_start, pt_end, pt_junc;
	Point vec_start, vec_end, vec_junc;
	Point center;
	Point vert_start, vert_end; //vertical vector of derivative vector
	Point vertl_start, vertl_end;
	Point se1, se2;

	evaluate(&curve, t_start, pt_start);
	evaluate(&curve, t_end, pt_end);

	//Caculate normal vector of start point and end point
	Der_NorVec(t_start, vec_start);
	Der_NorVec(t_end, vec_end);

	//normal vector * 10
	vert_start[0] = 10 * vec_start[1];
	vert_start[1] = -10 * vec_start[0];

	vert_end[0] = 10 * vec_end[1];
	vert_end[1] = -10 * vec_end[0];

	//Find vertical line of vector_start and vector_end
	for (int i = 0; i < 2; i++)
	{
		vertl_start[i] = pt_start[i] + vert_start[i];
		vertl_end[i] = pt_end[i] + vert_end[i];
	}

	//1. Find vertical line of start-end line
	Cal_vertline(pt_start, pt_end, se1, se2);

	if (line_intersect(se1, se2, pt_start, vertl_start, center))
	{
		glColor3f(1, 0, 0);
	//	DrawCircle(center, pt_start, pt_end);
		Draw_Offset(center, pt_start, pt_end, offset);
		Draw_Offset(center, pt_start, pt_end, -offset);
	}
	else
	{
		glColor3f(1, 0, 0);
	//	Draw_Line(pt_start, pt_end);
		Draw_LineOffset(pt_start, pt_end, offset);
		Draw_LineOffset(pt_start, pt_end, -offset);
	}
}

void Cal_vertline(Point start, Point end, Point pt1, Point pt2)
{
	/*Use line's start, end point to calculate the vertical line's two point pt1 and pt2*/
	Point mid;
	Point lvec, lvvec;
	SET_MIDDLE(mid, start, end);
	SET_VECTOR(lvec, start, end);
	SET_VECTOR_VER(lvvec, lvec);

	pt1[0] = mid[0];
	pt1[1] = mid[1];
	pt2[0] = mid[0] + lvvec[0];
	pt2[1] = mid[1] + lvvec[1];
}

void Cir_BCA(CubicBezierCurve curve,float t_start, float t_end)
{
	glColor3f(0, 0, 1);
	Point pt_start, pt_end, center1, center2;
	Point vec_start, vec_end;
	Point vert_start, vert_end; //vertical vector of derivative vector
	Point vertl_start, vertl_end;
	Point stvert1, stvert2;

	evaluate(&curve, t_start, pt_start);
	evaluate(&curve, t_end, pt_end);

	//Caculate normal vector of start point and end point
	/*Der_NorVec(t_start, &vec_start[0], &vec_start[1]);
	Der_NorVec(t_end, &vec_end[0], &vec_end[1]);*/
	Der_NorVec(t_start, vec_start);
	Der_NorVec(t_end, vec_end);

	//normal vector * 10
	vert_start[0] = 10 * vec_start[1];
	vert_start[1] = -10 * vec_start[0];

	vert_end[0] = 10 * vec_end[1];
	vert_end[1] = -10 * vec_end[0];

	//Find vertical line of vector_start and vector_end
	for (int i = 0; i < 2; i++)
	{
		vertl_start[i] = pt_start[i] + vert_start[i];
		vertl_end[i] = pt_end[i] + vert_end[i];
	}

	//Find vertical line of start-end line
	Cal_vertline(pt_start, pt_end, stvert1, stvert2);

	if (line_intersect(stvert1, stvert2, pt_start, vertl_start, center1))
		DrawCircle(center1, pt_start, pt_end);
	else
	{
		Draw_Line(pt_start, pt_end);
	}
	if (line_intersect(stvert1, stvert2, pt_end, vertl_end, center2))
		DrawCircle(center2, pt_start, pt_end);
	else
	{
		Draw_Line(pt_start, pt_end);
	}
}

bool Cal_JuncPt(CubicBezierCurve curve, float t_start, float t_end, float *t_junc)
{
	float distance, radius;
	Point pt_start, pt_end, pt_junc;
	Point vec_start, vec_end, vecl_start, vecl_end, vec_temp;
	Point l1_start, l1_end, l2_start, l2_end;
	Point center;

	evaluate(&curve, t_start, pt_start);
	evaluate(&curve, t_end, pt_end);

	//Find intersection circle
	//1st line: vector line
	Der_NorVec(t_start, vec_start);
	Der_NorVec(t_end, vec_end);

	for (int i = 0; i < 2; i++)
	{
		vecl_start[i] = pt_start[i] + 20 * vec_start[i];
		vecl_end[i] = pt_end[i] + 20 * vec_end[i];
	}
	Cal_vertline(vecl_start, vecl_end, l1_start, l1_end);

	//2nd line: start-end line
	Cal_vertline(pt_start, pt_end, l2_start, l2_end);

	//Find intersect center point and radius
	if (line_intersect(l1_start, l1_end, l2_start, l2_end, center))
	{
		radius = Dis(pt_start, center);

		//Calculate the distance between curve's point and center
		int segment;
		float interval;
		interval = (t_end - t_start)/SEG;
//		printf("segment is %d\n", segment);
		for (int i = 0; i < SEG; ++i)
		{
			Point pt1,pt2;
			float distance1, distance2;
			evaluate(&curve, t_start + i * interval, pt1);
			evaluate(&curve, t_start + (i + 1) * interval, pt2);

			distance1 = Dis(pt1, center) - radius;
			distance2 = Dis(pt2, center) - radius;
			if (distance1 * distance2 <= 0)
				*t_junc = t_start + (i + 0.5)*interval;
			else
				*t_junc = (t_start + t_end) / 2.0;
		}
		evaluate(&curve, *t_junc, pt_junc);

		glPointSize(5.0);
		glBegin(GL_POINTS);
		glVertex2fv(pt_junc);
		glEnd();
		return true;
	}
	else
	{
	//	printf("no junction point between t(%f, %f)\n", t_start, t_end);
		return false;
	}
}