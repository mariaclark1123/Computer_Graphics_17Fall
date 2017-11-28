/********************************************
Part 1.
Design an interactive system that can control the shape of a cubic Bezier curve by dragging the control points. 
Moreover, subdivide the curve into x,y-monotone inflection free spiral segments by adding points at the solutions
for each of the following equations:
Part 2.
For each x,y-monotone spiral curve segment Ci(u)(0<=u<=1), which is also curvature-monotone and inflection-free,
construct a G1-biarc spline curve that approximates the curve Ci within an error bound of e=10^-3. Approximate the offset
of Ci using the G1-biarc spline curve
********************************************/

/***************************************
color setting(RGB):
1. x monotone extreme point(200,200,169)
2. y monotone extreme point(131,175,155)
2. curvature extreme pointz(252,157,154)
3. inflection point(256,67,101)
*****************************************/
#include "tool.h"
#include <math.h>
#include <iostream>
#include <gl/freeglut.h>
using namespace std;

#define Ep 0.001


static int num = 0, num_junc = 0, num_line = 0;
float offset;

CubicBezierCurve curve;
GLsizei width = 640, height = 480;
int edit_ctrlpts_idx = -1;

/***time segment***/
int exin_num, curex_num, all_num;
float exin_t[20] = { 0 }, curex_t[5] = { 0 }, all_t[20] = { 0 };

typedef struct BiNode {
	float start;
	float end;
	float center_dis;
	int depth;
	struct BiNode *lChild;
	struct BiNode *rChild;
}BiNode, *BiTree;


void SetColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, (ForeColor % 16) | (BackGroundColor % 16 * 16));
}

void ExInf()
{
	int i, pointnum = 1;
	float t, t_sq, t1, t2, a, b, c, judgement;
	float	derivative1[2] = { 0 }, derivative2[2] = { 0 }, result2 = 0;

	//Set the [extreme point] word color and point color to red
	SetColor(8, 0);
	//CACULATE THE EXTREME POINTS' T
	for (i = 0; i < 2; i++)
	{
		//if a==0
		if(i == 0)
			glColor3ub(200, 200, 169);
		else
			glColor3ub(131, 175, 155);
		if (coef3[i] * coef3[i] < 0.0001)
		{
			if (coef2[i] != 0)
			{
				t = -coef1[i] / (2.0 * coef2[i]);
				if (t > 0 && t < 1)
				{
					Point pt;
					evaluate(&curve, t, pt);
					DrawDot(pt);

					exin_t[pointnum] = t;
					printf("Extreme point-------------t is %f, point is (%.2f,%.2f)\n", t, pt[0], pt[1]);
					pointnum++;
				}
			}
		}
		//if a != 0
		else
		{
			if (judge[i] > 0)
			{
				t1 = (-2.0 * coef2[i] + sqrt(judge[i])) / (2.0 * coef3[i]);
				t2 = (-2.0 * coef2[i] - sqrt(judge[i])) / (2.0 * coef3[i]);
				if (t1 > 0 && t1 < 1)
				{
					Point pt;
					evaluate(&curve, t1, pt);
					DrawDot(pt);
					exin_t[pointnum] = t1;
					printf("Extreme point-------------t is %f, point is (%.2f,%.2f)\n", t1, pt[0], pt[1]);
					pointnum++;
				}
				if (t2 > 0 && t2 < 1)
				{
					Point pt;
					evaluate(&curve, t2, pt);
					DrawDot(pt);
					//draw_points(t2);
					exin_t[pointnum] = t2;
					printf("Extreme point-------------t is %f, point is (%.2f,%.2f)\n", t2, pt[0], pt[1]);
					pointnum++;
				}
			}

			if (judge[i] == 0)
			{
				t = -coef2[i] / coef3[i];
				if (t > 0 && t < 1)
				{
					Point pt;
					evaluate(&curve, t, pt);
					DrawDot(pt);
					exin_t[pointnum] = t;
					printf("Extreme point--------------t is %f, point is (%.2f,%.2f)\n", t, pt[0], pt[1]);
					pointnum++;
				}
			}
		}
	}
	//Set the [inflection point] word color and point color to blue
	SetColor(2, 0);
	glColor3ub(256, 67, 101);
	//Inflection points' requirement: (x''(t)y'(t) - x'(t)y''(t))/18 = 0.
	/*
	x''(t)/6 = (b3-3b2+3b1-b0)t + (b2-2b1+b0)
			 = coef3[0]*t + coef2[0]
	x'(t)/3  = (b3-3b2+3b1-b0)t^2 + (2b2-4b1+2b0)t+(b1-b0)
		     = coef3[0]*t^2 + 2 * coef2[0]*t + coef1[0]

	y''(t)/6 = (b3-3b2+3b1-b0)t+(b2-2b1+b0)
			 = coef3[1]*t + coef2[1]
	y'(t)/3  = (b3-3b2+3b1-b0)t^2+(2b2-4b1+2b0)t+(b1-b0)
		     = coef3[1]*t^2 + 2 * coef2[1]*t + coef1[1]

	(x''(t)y'(t) ? x'(t)y''(t))/18 
			 = (coef3[0] * coef2[1] - coef3[1] * coef2[0]) * t * t + (coef3[0] * coef1[1] - coef3[1] * coef1[0]) * t + (coef2[0] * coef1[1] - coef2[1] * coef1[0])
	*/
	c = coef2[0] * coef1[1] - coef2[1] * coef1[0];
	b = coef3[0] * coef1[1] - coef3[1] * coef1[0];
	a = coef3[0] * coef2[1] - coef3[1] * coef2[0];
	judgement = b * b - 4 * a * c;
	if (a*a < 0.0001)
	{
		if (b != 0)
		{
			t = -c / b;
			if (t > 0 && t < 1)
			{
				Point pt;
				evaluate(&curve, t, pt);
				DrawDot(pt);

				exin_t[pointnum] = t;
				printf("inflection point-----------t is %f, point is (%.2f,%.2f)\n", t, exin_t[0], exin_t[1]);
				pointnum++;
			}
		}
	}
	else
	{
		if (judgement>0)
		{
			t1 = (-b + sqrt(judgement)) / (2.0*a);
			t2 = (-b - sqrt(judgement)) / (2.0*a);
			if (t1 > 0 && t1 < 1)
			{
				Point pt;
				evaluate(&curve, t1, pt);
				DrawDot(pt);

				exin_t[pointnum] = t1;
				printf("inflection point----------t is %f, point is (%.2f,%.2f)\n", t1, exin_t[0], exin_t[1]);
				pointnum++;
			}
			if (t2 > 0 && t2 < 1)
			{
				Point pt;
				evaluate(&curve, t2, pt);
				DrawDot(pt);

				exin_t[pointnum] = t2;
				printf("inflection point----------t is %f, point is (%.2f,%.2f)\n", t2, pt[0], pt[1]);
				pointnum++;
			}
		}
		if (judgement == 0)
		{
			t = -b / (2.0*a);
			if (t > 0 && t < 1)
			{
				Point pt;
				evaluate(&curve, t, pt);
				DrawDot(pt);

				exin_t[pointnum] = t;
				printf("Inflection point----------t is %f, point is (%.2f,%.2f)\n", t, pt[0], pt[1]);
				pointnum++;
			}
		}

	}
	//The number of all used points' timing
	exin_num = pointnum + 1;
	//printf("pnum is %d\n", pnum);
	exin_t[exin_num - 1] = 1.0;

	float temp;
	//Sorting the points' timing
	for (i = 0; i < exin_num; i++)
	{
		for (int j = 0; j < exin_num - 1 - i; j++)
		{
			if (exin_t[j] > exin_t[j + 1])
			{
				temp = exin_t[j];
				exin_t[j] = exin_t[j + 1];
				exin_t[j + 1] = temp;
			}
		}
	}
	//Set the word color to white
	SetColor(7, 0);
}

//Time sorting
void Time_Sort()
{
	float temp;
	//Get the all needed points for drawing arc
	all_num = curex_num + exin_num;
	for (int i = 0; i < exin_num; i++)
		all_t[i] = exin_t[i];
	for (int i = 0; i < curex_num; i++)
		all_t[i + exin_num] = curex_t[i];

	//Sorting the points' timing
	for (int i = 0; i < all_num; i++)
	{
		for (int j = 0; j < all_num - 1 - i; j++)
		{
			if (all_t[j] > all_t[j + 1])
			{
				temp = all_t[j];
				all_t[j] = all_t[j + 1];
				all_t[j + 1] = temp;
			}
		}
	}
}

//void DrawCircle(Point center, Point start, Point end)
//{
//	Point vec1, vec2;
//	float radius;
//	float cos1, cos2, sin1, sin2, cos_temp, sin_temp, theta1, theta2, theta_diff;
//
//	float cos_1 = cos(1);
//	float sin_1 = sin(1);
//
//	radius = (start[X] - center[X]) * (start[X] - center[X]) + (start[Y] - center[Y]) * (start[Y] - center[Y]);
//	radius = sqrt(radius);
//
//	//calculate vec1 and vec2
//	vec1[X] = start[X] - center[X];
//	vec1[Y] = start[Y] - center[Y];
//
//	vec2[X] = end[X] - center[X];
//	vec2[Y] = end[Y] - center[Y];
//
//	cos1 = vec1[X] / radius;
//	cos2 = vec2[X] / radius;
//	sin1 = vec1[Y] / radius;
//	sin2 = vec2[Y] / radius;
//
//	theta1 = acos(cos1);
//	if (sin1 < 0)
//		theta1 = 360 - theta1;
//
//	theta2 = acos(cos2);
//	if (sin2 < 0)
//		theta2 = 360 - theta2;
//	theta_diff = theta2 - theta1;
//	printf("\nradius is %f\n", radius);
//	printf("vec1 is (%f, %f), theta1 is %f\n", vec1[0], vec1[1], theta1);
//	printf("vec2 is (%f, %f), theta2 is %f\n", vec2[0], vec2[1], theta2);
//	//printf("theta2 is %f\n", theta2);
//	/*
//	1st quadrant: cos+sin+
//	2nd quadrant: cos-sin+
//	3rd quadrant: cos-sin-
//	4th quadrant: cos+sin-
//	*/
//	cos_temp = cos1;
//	sin_temp = sin1;
//
//	glColor3ub(0, 0, 0);
//	glBegin(GL_LINE_LOOP);
//	glVertex2fv(start);
//	printf("theta_diff is %f\n", theta_diff);
//	if (theta_diff >= 0)
//	{
//		for (int i = 1; i < theta_diff; i++)
//		{
//			cos_temp = Cos_Add(sin_temp, cos_temp, sin_1, cos_1);
//			sin_temp = Sin_Add(sin_temp, cos_temp, sin_1, cos_1);
//			glVertex2f(radius*cos_temp, radius*sin_temp);
//		}
//	}
//	glVertex2fv(end);
//	glEnd();
//}
//Judge the point in which quadrant

void CurEx(float t_start, float t_end)
{
#define RES 100
	float interval, t, temp;
	float curvature[RES];
	int i;

	//initialization
	for (i = 0; i < RES; i++)
		curvature[i] = 0;

	interval = (t_end - t_start) / (RES - 1);

	glColor3ub(252, 157, 154);

	for (i = 0; i < RES; i++)
	{
		float t = t_start + interval * i;
		curvature[i] = CalCurEx(t);
	}

	for (i = 0; i < RES - 1; i++)
	{
		//When curvature_extreme's sign is different
		if (curvature[i] * curvature[i + 1] < 0)
		{
			Point pt;
			float middle_point;
			middle_point = t_start + interval * (2 * i + 1) / 2.0;
			evaluate(&curve, middle_point, pt);
			SetColor(4, 0);
			DrawDot(pt);
			printf("Curvature extreme point---t is %f, point is (%.2f,%.2f)\n", middle_point, pt[0], pt[1]);

			curex_t[curex_num] = middle_point;
			curex_num++;

			SetColor(7, 0);
		}
	}
}

int CreateBiTree(BiTree T, CubicBezierCurve curve, float offset)
{
	(*T).depth++;
	//	printf("depth is£º%d \n", (*T).depth);
	float start = T->start;
	float end = T->end;
	float dis = T->center_dis;
	Point pt_start, pt_end;
	//	printf("interval is£º(%f,%f) \n", start, end);

	if (dis > Ep)
	{
		BiNode Tl;
		BiNode Tr;

		Tl.start = start;
		Tl.end = (start + end) / 2;
		Tl.center_dis = Center_dis(curve, start, (start + end) / 2);

		Tr.start = (start + end) / 2;
		Tr.end = end;
		Tr.center_dis = Center_dis(curve, (start + end) / 2, end);

		Tr.depth = T->depth;
		Tl.depth = T->depth;

		T->rChild = &Tr;
		T->lChild = &Tl;

		CreateBiTree(T->lChild, curve, offset);
		CreateBiTree(T->rChild, curve, offset);
		//printf("distance is %f, you have to divide\n",distance);
	}
	else
	{
		//Draw bi-arc
		num++;
		float t_junc;
		glLineWidth(2.0);
		glColor3f(1, 0, 0);
		//	printf("distance is smaller than 0.001, you can draw biarc\n");
		if (Cal_JuncPt(curve, start, end, &t_junc))
		{
			Draw_Biarc(curve, start, t_junc, end, offset);
			num_junc++;
		}
		//	Draw_Offset(Point center, Point start, Point end, int offset) in up function
		else
		{
			evaluate(&curve, start, pt_start);
			evaluate(&curve, end, pt_end);
		//	Draw_Arc(curve, start, end, offset);
			Draw_LineOffset(pt_start, pt_end, offset);
			Draw_LineOffset(pt_start, pt_end, -offset);
			num_line++;
		}
	}
	//	printf("Depth is %d\n", (*T).depth);
	return 1;
}

//Curve initialization
void init()
{
	SET_PT2(curve.control_pts[0], 50, 100);
	SET_PT2(curve.control_pts[1], 200, 300);
	SET_PT2(curve.control_pts[2], 400, 300);
	SET_PT2(curve.control_pts[3], 550, 100);
	update_coef(curve);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, width, 0, height);
}

void reshape_callback(GLint nw, GLint nh)
{
	width = nw;
	height = nh;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
}

void display_callback(void)
{
#define RES 100
	int i;
	float start, end;

	num = 0;
	num_junc = 0;
	num_line = 0;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(209, 73, 78);
	//notation
	glRasterPos2f(20.0f, height - 25.0f);
	drawString("Cubic Bezier Curve");
	glRasterPos2f(20.0f, height - 40.0f);
	drawString("---by Fangda Chen");
	glColor3ub(130, 57, 53);

	//Draw curve
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= RES; ++i)
	{
		Point pt;
		//t=i/100
		const float t = (float)i / (float)RES;
		//pt is point value
		evaluate(&curve, t, pt);
		glVertex2f(pt[0], pt[1]);
	}
	glEnd();

	/* control mesh */
	glColor3ub(130, 57, 53);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<4; ++i)
	{
		float *pt = curve.control_pts[i];
		glVertex2f(pt[0], pt[1]);
	}
	glEnd();

	/* Draw control pts */
	glColor3ub(130, 57, 53);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	for (i = 0; i < 4; ++i)
	{
		float *pt = curve.control_pts[i];
		glVertex2f(pt[0], pt[1]);
	}
	glEnd();

	glPointSize(10.0);
	//draw monotone extreme points and inflection points
	ExInf();

	//draw curvature extreme points
	curex_num = 0;
	for (i = 0; i < exin_num - 1; i++)
		CurEx(exin_t[i], exin_t[i + 1]);
	printf("\n");
	//All sudivision time sorting
	Time_Sort();

	//Draw BCA
	/*for (int i = 0; i < all_num - 1; i++)
		Cir_BCA(curve, all_t[i], all_t[i + 1]);*/

	//Draw biarc
	BiNode *Root;
	Root = (BiNode *)malloc((all_num - 1) * sizeof(BiNode));
	for (int i = 0; i < all_num - 1; i++)
	{
		if (all_t[i + 1] - all_t[i] > 0.001)
		{
			Root[i].depth = 0;
			Root[i].start = all_t[i];
			Root[i].end = all_t[i + 1];
			Root[i].center_dis = Center_dis(curve, all_t[i], all_t[i + 1]);
		}
	}
	for (int i = 0; i < all_num - 1; i++)
	{
		CreateBiTree(&Root[i], curve, offset);
	}
	printf("bi-arc's num is %d\n", num);
	printf("junc's num is %d\n", num_junc);
	printf("non-junc's num is %d\n", num_line);

	Draw_End(curve, offset);
	/*float t_junc;
	glLineWidth(2.0);
	glColor3f(1, 0, 0);
	for (int i = 0; i < all_num - 1; i++)
	{
		Cal_JuncPt(curve, all_t[i], all_t[i+1], &t_junc);
		Draw_Biarc(curve, all_t[i], t_junc, all_t[i+1]);
	}*/

	glutSwapBuffers();
}

void mouse_callback(GLint button, GLint action, GLint x, GLint y)
{
	if (GLUT_LEFT_BUTTON == button)
	{
		switch (action)
		{
		case GLUT_DOWN:
			edit_ctrlpts_idx = hit_index(&curve, x, height - y);
			break;

		case GLUT_UP:
			edit_ctrlpts_idx = -1;
			break;
		}
	}
}

void mouse_move_callback(GLint x, GLint y)
{
	if (edit_ctrlpts_idx != -1)
	{
		curve.control_pts[edit_ctrlpts_idx][0] = (float)x;
		curve.control_pts[edit_ctrlpts_idx][1] = (float)(height - y);
	}
	update_coef(curve);
	//array point_t Initialization, first t should be 0
	for (int i = 0; i < 20; i++)
		exin_t[i] = 0;
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(800, 200);
	glutInitWindowSize(width, height);
	glutCreateWindow("CAGD_HW1");

	printf("Please input OFFSET(float type):\n");
	cin >> offset;
	init();
	glutReshapeFunc(reshape_callback);
	glutDisplayFunc(display_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(mouse_move_callback);

	glutMainLoop();
	return 0;
}