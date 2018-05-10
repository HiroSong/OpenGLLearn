/*
* ����̫�����������������˶�����
* ʵ�������¹���
* 1. �ò�ͬ��С�������̫������������
* 2. ���������ͬ��̫����ת������ͬʱ���Ƶ�����ת
* 3. ��̫����Ϊ�����壬ʵ�ֵ���������̫����ķ���Ч��
* 4. Ϊ�˸�����ʵ��ʹ�����������������Ϊ����
*******
* @author�������
*/
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <gl/glut.h>

using namespace std;

const GLdouble SunRadius = 10;
const GLdouble HaloRadius = 12;
const GLdouble EarthRadius = 8;
const GLdouble MoonRadius = 5;

const GLdouble EarthToSun = 40;
const GLdouble MoonToEarth = 15;

int earthAngle = 0;
int moonAngle = 0;
int countNum = 0;
GLfloat width = 100, height = 100;

//���ó�ʼ����
void myinit() {
	//���ñ�����ɫ
	glClearColor(0.0, 15.0/255.0, 48.0/255.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);

	//���ù۲����
	glMatrixMode(GL_MODELVIEW);
	//��ʼ������ϵ
	glLoadIdentity();

	//������Ȼ���
	glEnable(GL_DEPTH_TEST);

	GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };  //���廷������ɫ
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  //�������������ɫ
	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //���徵�淴�����ɫ
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //�����Դλ��
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


	glEnable(GL_LIGHT0);
	//�����ƹ�
	glEnable(GL_LIGHTING);
}

void drawSun() {
	GLfloat color[2][3][3] = { { { 0.8,0.0,0.0 },{ 1.0,0.0,0.0 },{ 0.4,0.0,0.0 } },
	{ { 0.9,113.0 / 255.0, 0.0 }, { 1.0,113.0 / 255.0,0.0 }, { 0.4,0.0,0.0 } } };
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[0][0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[0][1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color[0][2]);
	glutSolidSphere(SunRadius, 100, 100);
	glPopMatrix();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[1][0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[1][1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color[1][2]);
	glTranslatef(0, 0, -HaloRadius);
	glutSolidSphere(HaloRadius, 100, 100);
	glPopMatrix();
}

void drawEarth() {
	GLfloat color[3][3] = { { 30.0 / 255.0,144.0 / 255.0,1.0 },{ 0.0,191.0 / 255.0,1.0 },{ 0.2,0.2,0.2 } };
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color[2]);
	glRotatef(earthAngle, 0, 1, 0);
	glTranslatef(EarthToSun, 0, 0);
	glutSolidSphere(EarthRadius, 100, 100);
	glPopMatrix();
}

void drawMoon() {
	GLfloat color[3][3] = { { 238.0 / 255.0,173.0 / 255.0,14.0 / 255.0 },{ 1.0,185.0 / 255.0,15.0 / 255.0 },{ 0.2,0.2,0.2 } };
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color[2]);
	glRotatef(earthAngle, 0, 1, 0);
	glTranslatef(EarthToSun, 0, 0);
	glRotatef(moonAngle, 0, 1, 0);
	glTranslatef(MoonToEarth, 0, 0);
	glutSolidSphere(MoonRadius, 100, 100);
	glPopMatrix();
}

void drawStar() {
	GLfloat color[3][3] = { { 1.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ 0.2,0.2,0.2 } };
	srand((unsigned)time(NULL));
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color[2]);
	glBegin(GL_POINTS);
	for (int i = 0; i < 100; i++) {
		int x = rand() % (int)(2 * width) - (int)width;
		int y = rand() % (int)(2 * height) - (int)height;
		int z = rand() % 100 - 100;
		glVertex3i(x, y, z);
	}
	glEnd();
	glPopMatrix();
}

//���ڳ�ʼ��ʾͼ��
void displayScene() {
	//��մ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	{
		drawSun();
		drawEarth();
		drawMoon();
		drawStar();
	}
	glPopMatrix();
	glFlush();
	//����ͨ��
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	//��ʼ������ϵ
	glLoadIdentity();

	if (w <= h) {
		height = 100.0*(GLfloat)h / (GLfloat)w;
		glOrtho(-100, 100,
			-height, height,
			-100, 100);
	}
	else {
		width = 100.0*(GLfloat)w / (GLfloat)h;
		glOrtho(-width, width,
			-100, 100,
			-100, 100);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

void rotatePlanet(int value) {
	if (countNum == 0) {
		earthAngle = (earthAngle + 1) % 360;
	}
	moonAngle = (moonAngle + 1) % 360;
	countNum = (countNum + 1) % 10;
	glutPostRedisplay();
	glutTimerFunc(5, rotatePlanet, 1);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple OpenGL example");
	myinit();
	glutDisplayFunc(displayScene);
	glutReshapeFunc(reshape);
	glutTimerFunc(5, rotatePlanet, 1);
	glutMainLoop();
	return 0;
}