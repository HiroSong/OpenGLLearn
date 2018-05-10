/*
* ʵ����Ƥ����ֱ��
* 1.	��Ӳ˵���ʹ�ÿ��Ըı��ߵ���ɫ�����͡��߿�����ԡ�
* 2.	��������ֱ�ߣ�ǰ�滭����ֱ�߲�����ʧ
*******
* @author�������
*/
#include<vector>
#include<iostream>
#include <gl/glut.h>

using namespace std;

int height = 500;
int width = 500;
GLfloat xMax = 500;
GLfloat yMax = 500;

int Pattern[7] =
{
	//ֱ��
	0xFFFF,

	//���� 1000100010001000, ��ʾʵ�ʻ��ߵĵ㣬�����ת����16���ƾ���0x1111 dotted  
	//.  .  .  .  .  .  .  .  .  .  .  .  .  .  
	//0x1111,  
	0x0101,

	//�㻮��    1111111111100100  dot dash  
	//____ . ____ . _____ . _____. _____  
	0x27FF,
	//0x1C47,  

	//������    1111111111001100  centre line  
	//_____ _ _____ _ _____ _ _____ _ _____  
	0x33FF,

	//����  1111110011111100   dashed  
	//____  ____  ____  ____  ____  ____  ____  
	0x3F3F,

	//˫�㻮��  1111111100100100  double dot dash  
	// ____ . . ____ . . ____ . . ____ . . ____  
	0x24FF,

	//���㻮��  111111110101010 tri_dot_dash  
	// ____ . . ____ . . ____ . . ____ . . ____  
	0x55FF
};

typedef struct {
	GLint x, y;
}Point2i;

vector<Point2i> pointVector;
Point2i curPoint;
GLfloat color[3] = { 0,0,0 };
int style = 0;
float lineWidth = 1.0;

void changeColor() {
	color[0] = rand() % 255 / 255.0;
	color[1] = rand() % 255 / 255.0;
	color[2] = rand() % 255 / 255.0;
}

void changeStyle() {
	style = (style + 1) % 7;
}

void addLineWidth() {
	lineWidth += 1;
}

void minusLineWidth() {
	if (lineWidth > 1)
		lineWidth -= 1;
}

void menuFunc(int value) {
	switch (value)
	{
	case 1:
		changeColor();
		break;
	case 2:
		changeStyle();
		break;
	case 3:
		addLineWidth();
		break;
	case 4:
		minusLineWidth();
		break;
	default:
		break;
	}
}

//���ó�ʼ����
void initial() {
	//���ñ���Ϊ��ɫ
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//���÷�����
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//��������
	glEnable(GL_LINE_STIPPLE);

	//���ù۲����
	glMatrixMode(GL_MODELVIEW);

	//��ʼ������ϵ
	glLoadIdentity();

	gluOrtho2D(0, 500.0, 0, 500.0);
	glMatrixMode(GL_MODELVIEW);

	glShadeModel(GL_FLAT);

	glutCreateMenu(menuFunc);
	glutAddMenuEntry("Change Color", 1);
	glutAddMenuEntry("Change Style", 2);
	glutAddMenuEntry("Add Line Width", 3);
	glutAddMenuEntry("Minus Line Width", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//���ڳ�ʼ��ʾͼ��
void displayScene() {
	//��մ���
	glClear(GL_COLOR_BUFFER_BIT);

	glLineStipple(1, Pattern[style]);
	glLineWidth(lineWidth);
	glColor3fv(color);

	glBegin(GL_LINE_STRIP);
	for (vector<Point2i>::iterator position = pointVector.begin();
		position != pointVector.end(); position++) {
		Point2i fixedPoint = *position;
		glVertex2d(fixedPoint.x, fixedPoint.y);
	}
	glVertex2d(curPoint.x, curPoint.y);
	glEnd();

	glFlush();
	//����ͨ��
	glutSwapBuffers();
}

//ʵ���ƶ��ص�������ʹ���������ƶ�
void drawFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		Point2i newPoint{ ((int)(x / (GLfloat)width * xMax)), ((int)((height - y) / (GLfloat)height * yMax)) };
		pointVector.push_back(newPoint);
	}
	glutPostRedisplay();
}

void slideFunc(int x, int y) {
	curPoint = Point2i{ ((int)(x / (GLfloat)width * xMax)), ((int)((height - y) / (GLfloat)height * yMax)) };
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	GLfloat wScale = (GLfloat)width / (GLfloat)w;
	GLfloat hScale = (GLfloat)height / (GLfloat)h;
	//��ʼ������ϵ
	glLoadIdentity();

	if (w <= h) {
		yMax = 500.0*(GLfloat)h / (GLfloat)w;
		gluOrtho2D(0, 500,
			0, yMax);
	}
	else {
		xMax = 500.0*(GLfloat)w / (GLfloat)h;
		gluOrtho2D(0, 500.0*(GLfloat)w / (GLfloat)h,
			0, 500);
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple OpenGL example");
	initial();
	glutMouseFunc(drawFunc);
	glutPassiveMotionFunc(slideFunc);
	glutDisplayFunc(displayScene);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}