﻿#include <windows.h>
#include "myNurbs.h"

bool showmodel = true;
bool showsurface = false;
int part_1 = 1, part_2 = 1, part_3 = 1, part_4 = 1, part_5 = 1, part_6 = 1;

float x[] = { 17,17,-17,-17,-17,17,17,
10,10,-10,-10,-10,10,10,
15,15,-15,-15,-15,15,15,
30,30,-30,-30,-30,30,30,
20,20,-20,-20,-20,20,20,
10,10,-10,-10,-10,10, 10,
0,0,0,0,0,0,0
 };

float x1[] = {86, 87, 101.90, 134, 153, 163, 164,
              50, 71, 139, 152, 175.82, 187.81, 188,
              26, 42, 114, 152, 179, 206, 230,
              14, 28, 67, 146, 178, 235.76, 236,
              8, 34, 64, 128, 163, 234, 236,
              14, 28, 68, 122, 162, 228, 230,         
	          26, 43, 57, 98, 147, 205, 212          
};

float x11[] = { 164.00, 163.84, 157.84, 146.00, 139.00, 94.00, 86.00,
				188.00, 181.00, 170.00, 146.00, 103.00, 41.00, 50.00,
				230.00, 238.00, 222.00, 158.00, 80.00, 13.00, 26.00,
				236.00, 223.00, 202.00, 140.00, 89.00, -11.00, 14.00,
	            236.00, 230.00, 203.00, 146.00, 98.00, -18.00, 8.00,
	            230.00, 229.00, 212.00, 188.00, 118.00, -13.00, 14.00,
	            212.00, 211.79, 201.80, 182.00, 77.92, 8.00, 26.00
};

float x2[] = { 26, 43, 57, 98, 147, 205, 212,
               38.00, 43.00, 67.00, 110.00, 158.00, 202.00, 212.00,
			   44.00, 52.00, 79.00, 116.00, 155.00, 208.00, 212.00,
			   50.00, 49.95, 90.00, 134.00, 177.00, 204.00, 212.00,
			   62.00, 83.00, 116.00, 140.00, 160.00, 210.00, 212.00,
			   74.00, 73.93, 97.90, 146.00, 182.00, 201.00, 212.00,
			   80.00, 79.92, 101.90, 146.00, 182.00, 205.00, 212.00,
};
float x22[] = {
			   212.00, 211.79, 201.80, 182.00, 77.92, 8.00, 26.00,
	           212.00, 216.00, 208.00, 176.00, 110.00, 19.00, 38.00,
			   212.00, 222.00, 204.00, 176.00, 109.00, 43.96, 44.00,
			   212.00, 219.00, 199.80, 176.00, 91.91, 58.00, 50.00,
			   212.00, 219.00, 203.80, 188.00, 101.00, 51.00, 62.00,
			   212.00, 217.00, 209.79, 206.00, 128.00, 66.00, 74.00,
			   212.00, 219.00, 207.79, 200.00, 121.00, 66.00, 80.00,
};

float x3[] = {
	80.00, 79.92, 101.90, 146.00, 182.00, 205.00, 212.00,
	86.00,105.00,121.00,158.00,188.00,206.00,212.00,
	104.00,111.00,129.00,158.00,184.00,209.00,212.00,
	116.00,120.00,138.00,176.00,192.00,206.00,212.00,
	128.00,134.00,147.00,188.00,200.00,218.00,224.00,
	134.00,133.87,143.00,188.00,207.00,221.00,224.00,
	146.00,149.00,164.00,181.00,204.00,217.00,224.00,
//	158.00,163.00,170.00,177.00,196.00,213.00,218.00
};

float x33[] = {
	212.00, 219.00, 207.79, 200.00, 121.00, 66.00, 80.00,
	212.00,219.00,205.79,194.00,121.88,81.00,86.00,
	212.00,211.79,216.00,206.00,137.86,94.00,104.00,
	212.00,218.00,209.79,206.00,145.85,108.00,116.00,
	224.00,231.00,217.78,206.00,154.00,117.00,128.00,
	224.00,223.78,221.78,218.00,159.00,123.00,134.00,
	224.00,223.78,221.78,218.00,170.00,138.00,146.00,
//	218.00,217.78,215.78,212.00,178.00,148.00,158.00
};

float testx33[] = {
	212.00, 219.00, 207.79, 200.00, 121.00, 66.00, 80.00,
	212.00,219.00,205.79,194.00,121.88,81.00,86.00,
	212.00,211.79,216.00,206.00,137.86,94.00,104.00,
	212.00,218.00,209.79,206.00,145.85,108.00,116.00,
	224.00,231.00,217.78,206.00,154.00,117.00,128.00,
	224.00,223.78,221.78,218.00,159.00,123.00,134.00,
	224.00,223.78,221.78,218.00,170.00,138.00,146.00,
	/*	218.00,217.78,215.78,212.00,178.00,148.00,158.00*/
};

float y[] = { 100,100,100,100,100,100,100,
90,90,90,90,90,90,90,
60,60,60,60,60,60,60,
30,30,30,30,30,30,30,
10,10,10,10,10,10,10,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0
};

float y2[] = {224, 239.76, 247.75, 248, 247.75, 229.77, 194,
              218, 248, 317, 302, 301.7, 269.73, 206,
              206, 259, 322, 308, 304, 277, 248,
	          224, 257, 323, 308, 296, 267.73, 188,
			  224, 275, 320, 308, 292, 255, 188,
	          230, 278, 323, 308, 286, 237, 182,
	          248, 273, 300, 308, 292, 256, 218,	          
              };
float y22[] = { 194.00, 181.82, 175.82, 176.00, 170.00, 203.00, 224.00,
                206.00, 174.00, 155.00, 152.00, 142.00, 181.00, 218.00,
	            248.00, 148.00, 136.00, 104.00, 107.00, 142.00, 206.00,
	            188.00, 133.00, 97.00, 92.00, 114.00, 145.00, 224.00,
	            188.00, 134.00, 107.00, 98.00, 102.00, 175.00, 224.00,
	            182.00, 146.00, 133.00, 122.00, 111.00, 171.00, 230.00,
	            218.00, 165.83, 139.86, 140.00, 139.86, 184.00, 248.00
};

float y3[] = { 248, 273, 300, 308, 292, 256, 218,
			   248.00, 272.00, 301.00, 294.00, 285.00, 248.00, 212.00,
               236.00, 268.00, 289.00, 290.00, 284.00, 253.00, 224.00,
	           230.00, 265.73, 287.00, 284.00, 270.00, 247.00, 236.00,
	           260.00, 270.00, 289.00, 284.00, 276.00, 256.00, 242.00,
	           260.00, 271.73, 277.72, 278.00, 276.00, 257.00, 242.00,
			   260.00, 271.73, 277.72, 278.00, 268.00, 254.00, 242.00,
};

float y33[] = { 218.00, 165.83, 139.86, 140.00, 139.86, 184.00, 248.00,
	            212.00, 168.00, 150.00, 146.00, 136.00, 189.00, 248.00,
				224.00, 158.00, 156.00, 152.00, 143.00, 179.82, 236.00,
				236.00, 167.00, 157.84, 158.00, 157.84, 186.00, 230.00,
	            242.00, 172.00, 163.84, 164.00, 155.00, 192.00, 260.00,
	            242.00, 180.00, 169.83, 170.00, 162.00, 176.00, 260.00,
	            242.00, 179.00, 169.83, 170.00, 168.00, 181.00, 260.00,
};
float y4[] = {
	260.00, 271.73, 277.72, 278.00, 268.00, 254.00, 242.00,
	248.00,262.00,271.00,272.00,265.00,250.00,242.00,
	242.00,254.00,274.00,272.00,266.00,249.00,236.00,
	230.00,250.00,272.00,266.00,263.00,252.00,242.00,
	242.00,255.00,270.00,266.00,260.00,243.00,236.00,
	224.00,247.75,267.00,260.00,256.00,241.00,230.00,
	224.00,238.00,244.00,245.00,244.00,227.00,212.00,
//	212.00,219.00,226.00,226.00,229.00,213.00,200.00
};

float y44[] = {
	242.00, 179.00, 169.83, 170.00, 168.00, 181.00, 260.00,
	242.00,182.00,175.82,176.00,175.82,172.00,248.00,
	236.00,199.80,181.00,182.00,181.82,170.00,242.00,
	242.00,191.00,181.82,182.00,181.82,178.00,230.00,
	236.00,201.00,181.82,182.00,174.00,189.00,242.00,
	230.00,197.80,181.82,182.00,175.00,189.00,224.00,
	212.00,191.81,181.82,182.00,177.00,183.00,224.00,
//	200.00,187.81,181.82,182.00,174.00,184.00,212.00

};

float testy44[] = {
	242.00, 179.00, 169.83, 170.00, 168.00, 181.00, 260.00,
	242.00,182.00,175.82,176.00,175.82,172.00,248.00,
	236.00,199.80,181.00,182.00,181.82,170.00,242.00,
	242.00,191.00,181.82,182.00,181.82,178.00,230.00,
	236.00,201.00,181.82,182.00,174.00,189.00,242.00,
	230.00,197.80,181.82,182.00,175.00,189.00,224.00,
	212.00,191.81,181.82,182.00,177.00,183.00,224.00,
	/*200.00,187.81,181.82,182.00,174.00,184.00,212.00*/
};
float z[] = { 60,77,77,60,43,43,60,
60,70,70,60,50,50,60,
60,75,75,60,45,45,60,
60,90,90,60,30,30,60,
60,80,80,60,40,40,60,
60,70,70,60,50,50,60,
60,60,60,60,60,60,60
 };

float z1[] = {-60, - 60, -60, -60, -60, -60, -60,
              -30, -30, -30, -30, -30, -30, -30,
	           0, 0, 0, 0, 0, 0, 0,
	           30, 30, 30, 30, 30, 30, 30,
               60, 60, 60, 60, 60, 60, 60,
			   90, 90, 90, 90, 90, 90, 90,
	           120, 120, 120, 120, 120, 120, 120,
};
float z2[] = { 120, 120, 120, 120, 120, 120, 120,
150, 150, 150, 150, 150, 150, 150,
180, 180, 180, 180, 180, 180, 180,
210, 210, 210, 210, 210, 210, 210,
240, 240, 240, 240, 240, 240, 240,
270, 270, 270, 270, 270, 270, 270,
300, 300, 300, 300, 300, 300, 300
};

float z3[] = { 300, 300, 300, 300, 300, 300, 300,
330, 330, 330, 330, 330, 330, 330,
360, 360, 360, 360, 360, 360, 360,
390, 390, 390, 390, 390, 390, 390,
420, 420, 420, 420, 420, 420, 420,
450, 450, 450, 450, 450, 450, 450,
480, 480, 480, 480, 480, 480, 480
};

float testz3[] = { 300, 300, 300, 300, 300, 300, 300,
330, 330, 330, 330, 330, 330, 330,
360, 360, 360, 360, 360, 360, 360,
390, 390, 390, 390, 390, 390, 390,
420, 420, 420, 420, 420, 420, 420,
450, 450, 450, 450, 450, 450, 450,
480, 480, 480, 480, 480, 480, 480,
//510, 510, 510, 510, 510, 510, 510
};

float w[] = { 
	1,0.5,0.5,1,0.5,0.5,1,
	1,0.5,0.5,1,0.5,0.5,1,
	1,0.5,0.5,1,0.5,0.5,1,
	1,0.5,0.5,1,0.5,0.5,1,
	1,0.5,0.5,1,0.5,0.5,1,
	1,0.5,0.5,1,0.5,0.5,1,
	1,0.5,0.5,1,0.5,0.5,1
};

float w1[] = { 1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1
};

float testw[] = {
	1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
1,0.5,0.5,1,0.5,0.5,1,
//1,0.5,0.5,1,0.5,0.5,1
};

float u[] = { 0,0,0,0.25,0.5,0.5,0.75,1,1,1 };
float v[] = { 0,0,0,0.1,0.4,0.6,0.95,1,1,1 };
float suv[161][161][3];
float r = 0.0f;
float dx = 1.0f;
float mx = 0.0f;

void lightm()
{
	GLfloat lamb[4] = { 0.35f,0.35f,0.35f,1.0f };
	GLfloat ldif[4] = { 0.35f,0.35f,0.35f,1.0f };
	GLfloat lspe[4] = { 0.55f,0.55f,0.55f,1.0f };
	GLfloat lpos[4] = { 200.0f,200.0f,-60.0f,1.0f };

	GLfloat mamb[4] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat mdif[4] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat mspe[4] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat memi[4] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat mshininess = 128.0f;

	glLightfv(GL_LIGHT1, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ldif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lspe);
	glLightfv(GL_LIGHT1, GL_POSITION, lpos);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mdif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mspe);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, memi);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mshininess);
}

void TimerFunction(int value)
{
	if (r<360.f)
		r += 1.0f;
	else
		r = 0.0f;
	mx += dx;
	if (mx>50)
		dx = -dx;
	if (mx<-100)
		dx = -dx;
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
//	glTranslatef(10, -60, -90);
	glTranslatef(0, -60, 0);
	glRotatef(-30, 1.0, 1.0, 1.0);
	glRotatef(-2 * r, 0.0, 1.0, 0.0);
	glRotatef(-r, 1.0, 1.0, 1.0);
	glScalef(0.3, 0.3, 0.3);
//	NurbsFace(2, 9, u, 20, 9, v, 20, x, y, z, w, suv);
	
	if (part_1)
	{
		NurbsFace(2, 9, u, 20, 9, v, 20, x1, y2, z1, w, suv);
		if (showsurface)
			ShowSurface(20, 20, suv, 0);
		if (showmodel)
			ShowSurface(20, 20, suv, 1);
	}

	if (part_2)
	{
		NurbsFace(2, 9, u, 20, 9, v, 20, x11, y22, z1, w, suv);
		if (showsurface)
			ShowSurface(20, 20, suv, 0);
		if (showmodel)
			ShowSurface(20, 20, suv, 1);
	}

	if (part_3)
	{
		NurbsFace(2, 9, u, 20, 9, v, 20, x2, y3, z2, w, suv);
		if (showsurface)
			ShowSurface(20, 20, suv, 0);
		if (showmodel)
			ShowSurface(20, 20, suv, 1);
	}

	if (part_4)
	{
		NurbsFace(2, 9, u, 20, 9, v, 20, x22, y33, z2, w, suv);
		if (showsurface)
			ShowSurface(20, 20, suv, 0);
		if (showmodel)
			ShowSurface(20, 20, suv, 1);
	}

	if (part_5)
	{
		NurbsFace(2, 9, u, 20, 9, v, 20, x3, y4, z3, w, suv);
		if (showsurface)
			ShowSurface(20, 20, suv, 0);
		if (showmodel)
			ShowSurface(20, 20, suv, 1);
	}

	if (part_6)
	{
		NurbsFace(2, 9, u, 20, 9, v, 20, x33, y44, z3, w, suv);
		if (showsurface)
			ShowSurface(20, 20, suv, 0);
		if (showmodel)
			ShowSurface(20, 20, suv, 1);
	}
	//NurbsFace(2, 9, u, 20, 9, v, 20, testx33, testy44, testz3, testw, suv);
	//if (showsurface)
	//	ShowSurface(20, 20, suv, 0);
	//if (showmodel)
	//	ShowSurface(20, 20, suv, 1);

	//NurbsFace(2, 9, u, 160, 9, v, 160, x, y, z, w, suv);
	//ShowSurface(160, 160, suv, 1);
	glPopMatrix();
	glutSwapBuffers();
}
void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	lightm();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
}

void WindowSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	GLfloat tmb = 110.0;

	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho(-tmb, tmb, -tmb / aspectRatio, tmb / aspectRatio, 10 * tmb, -10 * tmb);
	else
		glOrtho(-tmb*aspectRatio, tmb*aspectRatio, -tmb, tmb, 10 * tmb, -10 * tmb);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		showsurface = true;
		showmodel = false;
		break;
	case 'm':
		showmodel = true;
		showsurface = false;
		break;
	case '1':
		part_1 = (part_1 + 1) % 2;
		break;
	case '2':
		part_2 = (part_2 + 1) % 2;
		break;
	case '3':
		part_3 = (part_3 + 1) % 2;
		break;
	case '4':
		part_4 = (part_4 + 1) % 2;
		break;
	case '5':
		part_5 = (part_5 + 1) % 2;
		break;
	case '6':
		part_6 = (part_6 + 1) % 2;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("B-NURBS");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(WindowSize);
	glutTimerFunc(33, TimerFunction, 1);
	glutKeyboardFunc(keyboard);
	SetupRC();
	glutMainLoop();
	return 0;
}
