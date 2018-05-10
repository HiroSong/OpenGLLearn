/*
* �ó���ʵ�����¹���
* ����ply�ļ����ݣ���ģ����ʾ����
* ��Ӳ˵�������ѡ�񻭵㡢���ߡ����߻��������ɫ��
* �������������м��ص�������ʹ��ֱ�ʵ�ֶ�ģ�ͽ�����ת��ƽ�ơ�
* �����Ҽ��ѱ��˵�ռ�ã������Ź���ʵ��[]����ɣ�ע���л�ΪӢ��ģʽ
* ���ù��ա�
*******
* @author�������
*/
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <gl/glut.h>

using namespace std;

typedef struct {
	GLdouble x, y, z;
}Point3d;

typedef struct {
	int index1, index2, index3;
}FaceIndex;

enum mouseButton { LEFT_CLICK, MIDDLE_CLICK, NO_CLICK };


GLfloat angleX = 0.0;
GLfloat angleY = 0.0;
GLfloat scaleLevel = 1.0;
GLfloat moveX = 0.0;
GLfloat moveY = 0.0;
const GLfloat moveZ = 0.0;
GLfloat tempX = 0.0;
GLfloat tempY = 0.0;
int orgX, orgY;
bool moveState = false;

int pointNum, faceNum;

vector<Point3d> pointContainer, normalContainer;
vector<FaceIndex> faceContainer;
GLenum mode = GL_POINT;
mouseButton mouseMode;

void readPly(const char* fileName) {
	FILE* f = fopen(fileName, "r");
	if (!f)
		return;

	char strBuffer[255];
	for (int i = 0; i < 3; i++) {
		fgets(strBuffer, 255, f);
		memset(strBuffer, '\0', 255);
	}

	fscanf(f, "element vertex %d\n", &pointNum);

	for (int i = 0; i < 6; i++) {
		fgets(strBuffer, 255, f);
		memset(strBuffer, '\0', 255);
	}
	fscanf(f, "element face %d\n", &faceNum);
	for (int i = 0; i < 2; i++) {
		fgets(strBuffer, 255, f);
		memset(strBuffer, '\0', 255);
	}

	Point3d tempPoint;
	Point3d tempNormal;
	FaceIndex tempFace;
	for (int i = 0; i < pointNum; i++) {
		fscanf(f, "%lf %lf %lf %lf %lf %lf\n",
			&tempPoint.x, &tempPoint.y, &tempPoint.z,
			&tempNormal.x, &tempNormal.y, &tempNormal.z);
		pointContainer.push_back(tempPoint);
		normalContainer.push_back(tempNormal);
	}

	for (int i = 0; i < faceNum; i++) {
		fscanf(f, "3 %d %d %d\n",
			&tempFace.index1, &tempFace.index2, &tempFace.index3);
		faceContainer.push_back(tempFace);
	}

	fclose(f);
}

void menuFunc(int value) {
	switch (value)
	{
	case 1:
		mode = GL_POINT;
		break;
	case 2:
		mode = GL_LINE;
		break;
	case 3:
		mode = GL_FILL;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

//���ó�ʼ����
void myinit() {
	//���ñ�����ɫ
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//���ù۲����
	glMatrixMode(GL_MODELVIEW);

	glEnableClientState(GL_NORMAL_ARRAY);
	//������Ȼ���
	glEnable(GL_DEPTH_TEST);


	//��ʼ������ϵ
	glLoadIdentity();

	//���õ�ǰ�����λ�úͷ���
	gluLookAt(
		0.0, 0.0, 0.0,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);

	glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, -10000.0, 10000.0);
	readPly("lizhenxiout-repaired.ply");

	glutCreateMenu(menuFunc);
	glutAddMenuEntry("Point Mode", 1);
	glutAddMenuEntry("Line Mode", 2);
	glutAddMenuEntry("Mesh Mode", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };  //���廷������ɫ
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  //�������������ɫ
	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //���徵�淴�����ɫ
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	GLfloat lightPosition[] = { 0.0f, 0.0f, -500.0f, 1.0f };  //�����Դλ��
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//�����ƹ�
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GLUT_MULTISAMPLE);
	glShadeModel(GL_SMOOTH);
}

void drawModel() {
	Point3d tempPoint;
	Point3d tempVector;
	FaceIndex tempFace;
	Point3d tempFaceVertex[3];
	Point3d tempFaceNormal[3];
	GLfloat mat[] = { 0.0f, 0.0f, 1.0f, 1.0f };  //������ʵķ������ɫ
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat);
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < faceNum; i++) {
			tempFace = faceContainer[i];
			tempFaceVertex[0] = pointContainer[tempFace.index1];
			tempFaceVertex[1] = pointContainer[tempFace.index2];
			tempFaceVertex[2] = pointContainer[tempFace.index3];
			tempFaceNormal[0] = normalContainer[tempFace.index1];
			tempFaceNormal[1] = normalContainer[tempFace.index2];
			tempFaceNormal[2] = normalContainer[tempFace.index3];
			glNormal3d(tempFaceNormal[0].x, tempFaceNormal[0].y, tempFaceNormal[0].z);
			glVertex3d(tempFaceVertex[0].x, tempFaceVertex[0].y, tempFaceVertex[0].z);
			glNormal3d(tempFaceNormal[1].x, tempFaceNormal[1].y, tempFaceNormal[1].z);
			glVertex3d(tempFaceVertex[1].x, tempFaceVertex[1].y, tempFaceVertex[1].z);
			glNormal3d(tempFaceNormal[2].x, tempFaceNormal[2].y, tempFaceNormal[2].z);
			glVertex3d(tempFaceVertex[2].x, tempFaceVertex[2].y, tempFaceVertex[2].z);
		}
	}
	glEnd();
}

//���ڳ�ʼ��ʾͼ��
void displayScene() {
	//��մ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	{
		glTranslatef(moveX * 3.0, moveY * 3.0, moveZ);
		glRotatef(angleX, 1.0, 0.0, 0.0);
		glRotatef(angleY, 0.0, 1.0, 0.0);
		glScalef(scaleLevel, scaleLevel, scaleLevel);
		drawModel();
	}
	glPopMatrix();
	glFlush();
	//����ͨ��
	glutSwapBuffers();
}

//ʵ�����Żص�������ʹ��[]��������
void scaleFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case '[':
		if (scaleLevel > 0)
			scaleLevel -= 0.1;
		break;
	case ']':
		scaleLevel += 0.1;
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

//�������������м��ص�������ʹ��ֱ�ʵ�ֶ�ģ�ͽ�����ת��ƽ��
void mouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !moveState) {
		mouseMode = LEFT_CLICK;
		orgX = x;
		orgY = y;
		tempX = angleY;
		tempY = angleX;
		moveState = true;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && !moveState) {
		mouseMode = MIDDLE_CLICK;
		orgX = x;
		orgY = y;
		tempX = moveX;
		tempY = moveY;
		moveState = true;
	}

	if (state == GLUT_UP && moveState) {
		moveState = false;
		mouseMode = NO_CLICK;
	}
	glutPostRedisplay();
}

void moveFunc(int x, int y) {
	switch (mouseMode) {
	case LEFT_CLICK:
		angleY = tempX + (x - orgX);
		angleX = tempY - (y - orgY);
		break;
	case MIDDLE_CLICK:
		moveX = tempX + x - orgX;
		moveY = tempY - y + orgY;
		break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	//��ʼ������ϵ
	glLoadIdentity();

	if (w <= h)
		glOrtho(-1000, 1000,
			-1000.0*(GLfloat)h / (GLfloat)w, 1000.0*(GLfloat)h / (GLfloat)w,
			-10000, 10000);
	else
		glOrtho(-1000.0*(GLfloat)w / (GLfloat)h, 1000.0*(GLfloat)w / (GLfloat)h,
			-1000, 1000,
			-10000, 10000);
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple OpenGL example");
	myinit();
	glutKeyboardFunc(scaleFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(moveFunc);
	glutDisplayFunc(displayScene);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}