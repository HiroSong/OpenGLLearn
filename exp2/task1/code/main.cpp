/*
* Sierpinski
* ʵ���˱�ɫ�����ţ��ı��зֲ���������ת
* Ϊ�˱�֤��ת�������ԣ���ɫ�ı�Ƶ�ʽϿ죬���в���
*******
* @author�������
*/
#include <cmath> 
#include <cstdlib>
#include <gl/glut.h>

//����PIֵ
const double PI = 3.14159265358979323846;
//����PI/30
const double PIDIV30 = PI / 30.0;

//���ó�ʼ��ת��
const double ANGLE1 = PI / 2;
const double ANGLE2 = PI * 7 / 6;
const double ANGLE3 = PI * 11 / 6;

//�߳�Ϊ60�ĵȱ�������
GLfloat vertices[3][2] = { { 10.0,15.0 },{ 40.0, 66.9615 },{ 70.0, 15.0 } };

//������������ĵ�����ľ����Ƿ��Ѿ�����
bool beSetValue;
//���������ĵ�����ľ���
double coreDistant;
//��ת��������������ת�����PI/30�ı���
int shapeAngle = -1;
//ָ���зִ����Ըı�����
int sizeNum;

//��������
void myinit() {
	//���ñ���Ϊ��ɫ
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//���ù۲����
	glMatrixMode(GL_PROJECTION);

	//��ʼ������ϵ
	glLoadIdentity();
	gluOrtho2D(0.0, 80.0, 0.0, 80.0);
	glMatrixMode(GL_MODELVIEW);
}

//ָ�����ִ���Ϊ[2,5]��
int setSizeNum() {
	int sizeNum = rand() % 4 + 2;
	return sizeNum;
}

//��ȡ�����ɫֵ����������λС��
void setColor() {
	int rand();
	double colorR = rand() % 1000 / 1000.0;
	double colorG = rand() % 1000 / 1000.0;
	double colorB = rand() % 1000 / 1000.0;
	glColor3f(colorR, colorG, colorB);
}

//��ȡ���������ĵ�����ľ���
double getDistCoreToVer(GLfloat *a, GLfloat *b) {
	double sideLen = sqrt((a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1]));
	return sideLen * sqrt(3.0) / 3.0;
}

//������ת�Ժ�������
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

//���������ζ���
void triangle(GLfloat *a, GLfloat *b, GLfloat *c) {
	if (!beSetValue) {
		coreDistant = getDistCoreToVer(a, b);
		beSetValue = true;
	}

	//��ȡ��������
	GLfloat core[2] = { (a[0] + b[0] + c[0]) / 3.0, (a[1] + b[1] + c[1]) / 3.0 };

	//������ת�Ժ�������
	changePosition(core);
}

//�з�������
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

	//�ı���ת�Ǳ���
	shapeAngle = (shapeAngle + 1) % 20;

	if (shapeAngle == 0) {
		//���ñ��
		beSetValue = false;
		sizeNum = setSizeNum();
	}

	//��մ���
	glClear(GL_COLOR_BUFFER_BIT);

	//��ʼ��ͼ
	glBegin(GL_TRIANGLES);
	divideTriangle(vertices[0], vertices[1], vertices[2], sizeNum);
	glEnd();

	glFlush();
	//����ͨ��
	glutSwapBuffers();
}

//���ö���Ч��
void changeTriangle(int value) {
	display();
	glutTimerFunc(100, changeTriangle, 1);
}

//����Esc�˳�
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