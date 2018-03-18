/*
* Sierpinski
* 实现了变色、缩放（改变切分层数）、旋转
* 为了保证旋转的流畅性，颜色改变频率较快，略有不适
*******
* @author：宋灵冰
*/
#include <cmath> 
#include <cstdlib>
#include <gl/glut.h>

//设置PI值
const double PI = 3.14159265358979323846;
//设置PI/30
const double PIDIV30 = PI / 30.0;

//设置初始旋转角
const double ANGLE1 = PI / 2;
const double ANGLE2 = PI * 7 / 6;
const double ANGLE3 = PI * 11 / 6;

//边长为60的等边三角形
GLfloat vertices[3][2] = { { 10.0,15.0 },{ 40.0, 66.9615 },{ 70.0, 15.0 } };

//标记三角形中心到定点的距离是否已经计算
bool beSetValue;
//三角形中心到顶点的距离
double coreDistant;
//旋转比例：三角形旋转角相对PI/30的倍数
int shapeAngle = -1;
//指定切分次数以改变缩放
int sizeNum;

//设置属性
void myinit() {
	//设置背景为白色
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//设置观察参数
	glMatrixMode(GL_PROJECTION);

	//初始化坐标系
	glLoadIdentity();
	gluOrtho2D(0.0, 80.0, 0.0, 80.0);
	glMatrixMode(GL_MODELVIEW);
}

//指定划分次数为[2,5]次
int setSizeNum() {
	int sizeNum = rand() % 4 + 2;
	return sizeNum;
}

//获取随机颜色值，各保留三位小数
void setColor() {
	int rand();
	double colorR = rand() % 1000 / 1000.0;
	double colorG = rand() % 1000 / 1000.0;
	double colorB = rand() % 1000 / 1000.0;
	glColor3f(colorR, colorG, colorB);
}

//获取三角形中心到顶点的距离
double getDistCoreToVer(GLfloat *a, GLfloat *b) {
	double sideLen = sqrt((a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1]));
	return sideLen * sqrt(3.0) / 3.0;
}

//设置旋转以后的坐标点
void changePosition(GLfloat *core) {
	GLfloat a[2], b[2], c[2];
	a[0] = core[0] + cos(ANGLE1 + shapeAngle * PIDIV30)*coreDistant;
	a[1] = core[1] + sin(ANGLE1 + shapeAngle * PIDIV30)*coreDistant;
	b[0] = core[0] + cos(ANGLE2 + shapeAngle * PIDIV30)*coreDistant;
	b[1] = core[1] + sin(ANGLE2 + shapeAngle * PIDIV30)*coreDistant;
	c[0] = core[0] + cos(ANGLE3 + shapeAngle * PIDIV30)*coreDistant;
	c[1] = core[1] + sin(ANGLE3 + shapeAngle * PIDIV30)*coreDistant;
	glVertex2fv(a);
	glVertex2fv(b);
	glVertex2fv(c);
}

//绘制三角形顶点
void triangle(GLfloat *a, GLfloat *b, GLfloat *c) {
	if (!beSetValue) {
		coreDistant = getDistCoreToVer(a, b);
		beSetValue = true;
	}

	//获取中心坐标
	GLfloat core[2] = { (a[0] + b[0] + c[0]) / 3.0, (a[1] + b[1] + c[1]) / 3.0 };

	//设置旋转以后的坐标点
	changePosition(core);
}

//切分三角形
void divideTriangle(GLfloat *a, GLfloat *b, GLfloat *c, int k) {
	GLfloat ab[2], ac[2], bc[2];
	int j;
	if (k > 0) {
		for (j = 0; j < 2; j++) ab[j] = (a[j] + b[j]) / 2;
		for (j = 0; j < 2; j++) ac[j] = (a[j] + c[j]) / 2;
		for (j = 0; j < 2; j++) bc[j] = (b[j] + c[j]) / 2;

		divideTriangle(a, ab, ac, k - 1);
		divideTriangle(c, ac, bc, k - 1);
		divideTriangle(b, bc, ab, k - 1);
	}
	else {
		setColor();
		triangle(a, b, c);
	}
}

void display() {

	//改变旋转角比重
	shapeAngle = (shapeAngle + 1) % 20;

	if (shapeAngle == 0) {
		//重置标记
		beSetValue = false;
		sizeNum = setSizeNum();
	}

	//清空窗口
	glClear(GL_COLOR_BUFFER_BIT);

	//开始绘图
	glBegin(GL_TRIANGLES);
	divideTriangle(vertices[0], vertices[1], vertices[2], sizeNum);
	glEnd();

	glFlush();
	//交换通道
	glutSwapBuffers();
}

//设置动画效果
void changeTriangle(int value) {
	display();
	glutTimerFunc(100, changeTriangle, 1);
}

//按下Esc退出
void pressToExit(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple OpenGL example");
	myinit();
	glutKeyboardFunc(pressToExit);
	glutDisplayFunc(display);
	glutTimerFunc(100, changeTriangle, 1);
	glutMainLoop();
	return 0;
}