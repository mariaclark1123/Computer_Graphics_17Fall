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
#include <iostream>
#include <gl/freeglut.h>
using namespace std;

CubicBezierCurve curve;
GLsizei width = 640, height = 480;
int edit_ctrlpts_idx = -1;
int pt_num;
float res_pt[20] = {0};
float coef1[2] = { 0 }, coef2[2] = { 0 }, coef3[2] = { 0 }, judge[2] = { 0 };

void SetColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, (ForeColor % 16) | (BackGroundColor % 16 * 16));
}

void update_coef()
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

void CurEx(float t_start, float t_end)
{
	float interval, t, temp;
	float curvature[32];
	int i;

	//initialization
	for (i = 0; i < 32; i++)
		curvature[i] = 0;

	interval = (t_end - t_start) / 31.0;

	glColor3ub(252, 157, 154);

	for (i = 0; i < 32; i++)
	{
		float t = t_start + interval * i;
		curvature[i] = CalCurEx(t);
	}

	for (i = 0; i < 31; i++)
	{
		//When curvature_extreme's sign is different
		if (curvature[i] * curvature[i + 1] < 0)
		{
			Point pt;
			float middle_point;
			middle_point = t_start + interval * (2 * i + 1) / 2.0;
			evaluate(&curve, middle_point, pt);
			SetColor(4, 0);
			DrawDot(pt[0], pt[1]);
			printf("Curvature extreme point---t is %f, point is (%.2f,%.2f)\n", middle_point, pt[0], pt[1]);
			SetColor(7, 0);
		}
		else if (curvature[i] == 0)
		{
			if (i >= 1 && i <= 30)
			{
				if (curvature[i - 1] * curvature[i + 1] < 0)
				{
					Point pt;
					float middle_point;
					middle_point = t_start + interval*(2 * i + 1) / 2.0;
					evaluate(&curve, middle_point, pt);
					SetColor(4, 0);
					DrawDot(pt[0], pt[1]);
					SetColor(7, 0);
				}
			}
		}
	}
}

void Der_Vector(float t, Point* vec)
{
	//	x'(t)/3  = (b3-3b2+3b1-b0)t^2 + (2b2-4b1+2b0)t+(b1-b0)
	//		  	 = coef3[0] * t ^ 2 + 2 * coef2[0] * t + coef1[0]
	float der_x, der_y, length;
	der_x = coef3[0] * t * t + 2 * coef2[0] * t + coef1[0];
	der_y = coef3[1] * t * t + 2 * coef2[1] * t + coef1[1];
	length = der_x * der_x + der_y * der_y;
	length = sqrt(length);
	*vec[0] = der_x / length;
	*vec[1] = der_y / length;
}

void Cal_midpt(Point start, Point end, Point* mid)
{
	*mid[0] = (start[0] + end[0]);
	*mid[1] = (start[0] + end[1]);
}

void Cal_lineinters(Point l1_start, POINT l1_end, Point l2_start, POINT l2_end, Point* inter)
{
	//1st line
	//y = (l1_end[1]-l1_start[1])/(l1_end[0]-l1_start[0]) x + (l1_start[1]*l1_end[0]-l1_end[1]*l1_start[0])/(l1_end[0]-l1_start[0])
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
					DrawDot(pt[0], pt[1]);

					res_pt[pointnum] = t;
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
					DrawDot(pt[0], pt[1]);
					res_pt[pointnum] = t1;
					printf("Extreme point-------------t is %f, point is (%.2f,%.2f)\n", t1, pt[0], pt[1]);
					pointnum++;
				}
				if (t2 > 0 && t2 < 1)
				{
					Point pt;
					evaluate(&curve, t2, pt);
					DrawDot(pt[0], pt[1]);
					//draw_points(t2);
					res_pt[pointnum] = t2;
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
					DrawDot(pt[0], pt[1]);
					res_pt[pointnum] = t;
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
				DrawDot(pt[0], pt[1]);

				res_pt[pointnum] = t;
				printf("inflection point-----------t is %f, point is (%.2f,%.2f)\n", t, res_pt[0], res_pt[1]);
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
				DrawDot(pt[0], pt[1]);

				res_pt[pointnum] = t1;
				printf("inflection point----------t is %f, point is (%.2f,%.2f)\n", t1, res_pt[0], res_pt[1]);
				pointnum++;
			}
			if (t2 > 0 && t2 < 1)
			{
				Point pt;
				evaluate(&curve, t2, pt);
				DrawDot(pt[0], pt[1]);

				res_pt[pointnum] = t2;
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
				DrawDot(pt[0], pt[1]);

				res_pt[pointnum] = t;
				printf("Inflection point----------t is %f, point is (%.2f,%.2f)\n", t, pt[0], pt[1]);
				pointnum++;
			}
		}

	}
	//The number of all used points' timing
	pt_num = pointnum + 1;
	//printf("pnum is %d\n", pnum);
	res_pt[pt_num - 1] = 1.0;

	float temp;
	//Sorting the points' timing
	for (i = 0; i < pt_num; i++)
	{
		for (int j = 0; j < pt_num - 1 - i; j++)
		{
			if (res_pt[j] > res_pt[j + 1])
			{
				temp = res_pt[j];
				res_pt[j] = res_pt[j + 1];
				res_pt[j + 1] = temp;
			}
		}
	}
	//Set the word color to white
	SetColor(7, 0);
}

//void Biarc(float t_start, float t_end)
//{
//	Point pt_start, p_end;
//	pt_start = evaluate()
//
//}

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

//Curve initialization
void init()
{
	SET_PT2(curve.control_pts[0], 50, 100);
	SET_PT2(curve.control_pts[1], 200, 300);
	SET_PT2(curve.control_pts[2], 400, 300);
	SET_PT2(curve.control_pts[3], 550, 100);

	update_coef();

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
	//extreme points and inflection points
	ExInf();
	
	for (i = 0; i < pt_num - 1; i++)
		CurEx(res_pt[i], res_pt[i + 1]);

	printf("\n\n");
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
	update_coef();
	//array point_t Initialization, first t should be 0
	for (int i = 0; i < 20; i++)
		res_pt[i] = 0;
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(800, 200);
	glutInitWindowSize(width, height);
	glutCreateWindow("CAGD_HW1");

	init();
	glutReshapeFunc(reshape_callback);
	glutDisplayFunc(display_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(mouse_move_callback);

	glutMainLoop();
	return 0;
}