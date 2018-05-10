/*
* 该程序实现以下功能
* 1. 读入ply文件内容，将模型显示出来
* 2. 添加菜单，允许选择画点、画线、或者画多边形着色。
* 3. 设置鼠标左键、中键回调函数，使其分别实现对模型进行旋转、平移。
* 4. 由于右键已被菜单占用，故缩放功能实用[]键完成，注意切换为英文模式
* 5. 设置光照。
* 6. 以模型重心为旋转中心，旋转模型
* 7. 设置了轨迹球
*******
* @author：宋灵冰
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
	int index[3];
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
Point3d corePoint = { 0,0,0 };

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
		fscanf(f, "%lf %lf %lf %lf %lf %lf \n",
			&tempPoint.x, &tempPoint.y, &tempPoint.z,
			&tempNormal.x, &tempNormal.y, &tempNormal.z);
		corePoint.x += tempPoint.x;
		corePoint.y += tempPoint.y;
		corePoint.z += tempPoint.z;
		pointContainer.push_back(tempPoint);
		normalContainer.push_back(tempNormal);
	}

	corePoint.x /= (double)pointNum;
	corePoint.y /= (double)pointNum;
	corePoint.z /= (double)pointNum;

	for (int i = 0; i < faceNum; i++) {
		fscanf(f, "3 %d %d %d \n",
			&tempFace.index[0], &tempFace.index[1], &tempFace.index[2]);
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

//设置初始属性
void myinit() {
	//设置背景颜色
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, -10000.0, 10000.0);

	//设置观察参数
	glMatrixMode(GL_MODELVIEW);

	//启用深度缓存
	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);

	//初始化坐标系
	glLoadIdentity();
	GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义光源位置
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	readPly("lizhenxiout-repaired.ply");

	glutCreateMenu(menuFunc);
	glutAddMenuEntry("Point Mode", 1);
	glutAddMenuEntry("Line Mode", 2);
	glutAddMenuEntry("Mesh Mode", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glEnable(GL_LIGHT0);
	//开启灯光
	glEnable(GL_LIGHTING);
}

void drawModel() {
	FaceIndex tempFace;
	Point3d tempFaceVertex[3];
	Point3d tempFaceNormal[3];
	GLfloat color[2][3] = { {0.6,0.6,0.6}, { 0.8,0.8,0.8 } };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[1]);
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < faceNum; i++) {
			tempFace = faceContainer[i];
			for (int j = 0; j < 3; j++) {
				tempFaceVertex[j] = pointContainer[tempFace.index[j]];
				tempFaceNormal[j] = normalContainer[tempFace.index[j]];
				glNormal3d(tempFaceNormal[j].x, tempFaceNormal[j].y, tempFaceNormal[j].z);
				glVertex3d(tempFaceVertex[j].x, tempFaceVertex[j].y, tempFaceVertex[j].z);
			}
		}
	}
	glEnd();
}

void drawTraceBall() {
	const double ballRadius = 800;
	const double PI = 3.1415926535;
	GLfloat color[3][3] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[0]);
	glTranslatef(corePoint.x - ballRadius, corePoint.y, corePoint.z);
	glutWireTorus(ballRadius, ballRadius + 1, 1000, 1);
	glPopMatrix();
	
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[1]);
	glTranslatef(corePoint.x - ballRadius, corePoint.y, corePoint.z);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutWireTorus(ballRadius, ballRadius + 1, 1000, 1);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[2]);
	glTranslatef(corePoint.x, corePoint.y - ballRadius, corePoint.z);
	glRotatef(90, 0.0, 0.0, 1.0);
	glutWireTorus(ballRadius, ballRadius + 1, 1000, 1);
	glPopMatrix();
	
}

//用于初始显示图像
void displayScene() {
	//清空窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	{
		glPushMatrix();
		glScalef(scaleLevel, scaleLevel, scaleLevel);
		glTranslatef(moveX * 3.0, moveY * 3.0, moveZ);
		glTranslatef(corePoint.x, corePoint.y, corePoint.z);
		glRotatef(angleX, 1.0, 0.0, 0.0);
		glRotatef(angleY, 0.0, 1.0, 0.0);
		glTranslatef(-corePoint.x, -corePoint.y, -corePoint.z);
		drawModel();
		glPopMatrix();

		
		glPushMatrix();
		glTranslatef(corePoint.x, corePoint.y, corePoint.z);
		glRotatef(angleX, 1.0, 0.0, 0.0);
		glRotatef(angleY, 0.0, 1.0, 0.0);
		glTranslatef(-corePoint.x, -corePoint.y, -corePoint.z);
		drawTraceBall();
		glPopMatrix();
	}
	glPopMatrix();
	glFlush();
}

//实现缩放回调函数，使用[]控制缩放
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

//设置鼠标左键、中键回调函数，使其分别实现对模型进行旋转、平移
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
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	//初始化坐标系
	glLoadIdentity();

	if (w <= h)
		glOrtho(-1000, 1000,
			-1000.0*(GLfloat)h / (GLfloat)w, 1000.0*(GLfloat)h / (GLfloat)w,
			-10000, 10000);
	else
		glOrtho(-1000.0*(GLfloat)w / (GLfloat)h, 1000.0*(GLfloat)w / (GLfloat)h,
			-1000, 1000,
			-10000, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
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