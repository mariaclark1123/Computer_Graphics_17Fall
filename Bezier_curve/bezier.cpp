// Code skeleton for all my OpenGL related tests.

// Copied and reorganized from OpenGL superbible code.

//

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include "deCasteljau.h"



int windowHandle = -1;

Point2DVector controlPoints;

bool shouldDrawBezier = false;



void renderBezierCurve(const Point2DVector &points)

{

	float u = 0.0f;



	glPushAttrib(GL_CURRENT_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);



	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 50; ++i)

	{

		u = (float)1 / 50 * i;

		Point2D pt = deCasteljau(points, u);
		glVertex2f(pt.x, pt.y);

	}

	glEnd();



	glPopAttrib();

}



void renderControlPolygonOfBezierCurve(const Point2DVector &points)

{

	glPushAttrib(GL_CURRENT_BIT);

	glColor3f(1.0f, 0.0f, 1.0f);

	glPointSize(5.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); ++i)

	{

		glVertex2f(points[i].x, points[i].y);

	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points.size(); ++i)

	{

		glVertex2f(points[i].x, points[i].y);

	}
	glEnd();

	

	glPopAttrib();

}



void SetupRC()

{

	// Black background

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}



void ChangeSize(int w, int h)

{

	// Prevent a divide by zero, when window is too short

	// (you cant make a window of zero width).

	if (h == 0)

		h = 1;



	// Reset the coordinate system before modifying

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();



	// Set the viewport to be the entire window

	glViewport(0, 0, w, h);

	// Set the clipping volume

	glOrtho(0, 640, 0, 480, -10, 10);
}

void RenderScene(void)

{

	// Clear the window with current clearing color

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);



	if (shouldDrawBezier)

	{

		renderControlPolygonOfBezierCurve(controlPoints);

		renderBezierCurve(controlPoints);

	}



	// Flush drawing commands

	glutSwapBuffers();

}



void KeyPressFunc(unsigned char key, int x, int y)

{

	switch (key)

	{

	case VK_ESCAPE:

		glFinish();

		glutDestroyWindow(windowHandle);

		exit(0);

		break;

	}



	// Refresh the Window

	glutPostRedisplay();

}



void MouseFunc(int button, int state, int x, int y)

{

	if (button == GLUT_LEFT_BUTTON)

	{

		if (state == GLUT_DOWN)

		{

			controlPoints.push_back(Point2D(x, 480 - y));



			if (!shouldDrawBezier && controlPoints.size() >= 2)

			{

				shouldDrawBezier = true;

			}



			glutPostRedisplay();

		}

	}

}



int main(int argc, char* argv[])

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(200, 200);

	glutInitWindowSize(640, 480);

	windowHandle = glutCreateWindow("Bezier Curve Demo");

	glutReshapeFunc(ChangeSize);

	glutKeyboardFunc(KeyPressFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(RenderScene);



	SetupRC();



	glutMainLoop();

	return 0;

}
