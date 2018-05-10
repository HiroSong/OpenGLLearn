/*
* 实现橡皮条画直线
* 1.	添加菜单，使得可以改变线的颜色、线型、线宽等属性。
* 2.	允许画多条直线，前面画过的直线不会消失
*******
* @author：宋灵冰
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
	//直线
	0xFFFF,

	//点线 1000100010001000, 表示实际画线的点，反序后转换成16进制就是0x1111 dotted  
	//.  .  .  .  .  .  .  .  .  .  .  .  .  .  
	//0x1111,  
	0x0101,

	//点划线    1111111111100100  dot dash  
	//____ . ____ . _____ . _____. _____  
	0x27FF,
	//0x1C47,  

	//中心线    1111111111001100  centre line  
	//_____ _ _____ _ _____ _ _____ _ _____  
	0x33FF,

	//虚线  1111110011111100   dashed  
	//____  ____  ____  ____  ____  ____  ____  
	0x3F3F,

	//双点划线  1111111100100100  double dot dash  
	// ____ . . ____ . . ____ . . ____ . . ____  
	0x24FF,

	//三点划线  111111110101010 tri_dot_dash  
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

//设置初始属性
void initial() {
	//设置背景为白色
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//启用反走样
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//启用虚线
	glEnable(GL_LINE_STIPPLE);

	//设置观察参数
	glMatrixMode(GL_MODELVIEW);

	//初始化坐标系
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

//用于初始显示图像
void displayScene() {
	//清空窗口
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
	//交换通道
	glutSwapBuffers();
}

//实现移动回调函数，使用鼠标控制移动
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
	//初始化坐标系
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