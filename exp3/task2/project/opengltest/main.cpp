/*
* 该程序实现以下功能
* 1.	设置合适的相机位置和相机投影矩阵，使得OpenGL相机能够“看到”需要绘制的物体。
* 2.	用glutWireTeapot函数绘制茶壶。
* 3.	使用glRotate，glTranslate，glScale，对这个茶壶进行变换
* 4.  使用鼠标拖动茶壶可以实现移动，使用方向键可以控制旋转，使用 [ ]键（英文键盘）可以控制缩放
*******
* @author：宋灵冰
*/
#include <gl/glut.h>

GLfloat angleX = 0.0;
GLfloat angleY = 0.0;
GLfloat scaleLevel = 1.0;
GLfloat moveX = 0.0;
GLfloat moveY = 0.0;
const GLfloat moveZ = 0.0;
GLfloat tempMoveX = 0.0;
GLfloat tempMoveY = 0.0;
int orgX, orgY;
bool move = false;

//设置初始属性
void myinit() {
	//设置背景为白色
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//启用深度缓存
	glEnable(GL_DEPTH_TEST);
	//设置观察参数
	glMatrixMode(GL_MODELVIEW);

	//初始化坐标系
	glLoadIdentity();

	//设置当前相机的位置和方向
	gluLookAt(
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0);

	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

//用于初始显示图像
void displayScene() {
	//清空窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	{
		//设置茶壶颜色
		glColor3d(153.0/255.0, 49.0/255.0, 30.0/255.0);
		glutWireTeapot(20);
	}
	glPopMatrix();
	glFlush();
	//交换通道
	glutSwapBuffers();
}

//设置空闲回调函数，用于响应状态变化
void changeView() {
	//清空窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	{
		glTranslatef(moveX / 3.0, moveY / 3.0, moveZ);
		glRotatef(angleX, 1.0, 0.0, 0.0);
		glRotatef(angleY, 0.0, 1.0, 0.0);
		glScalef(scaleLevel, scaleLevel, scaleLevel);
		glColor3d(153.0 / 255.0, 49.0 / 255.0, 30.0 / 255.0);
		glutWireTeapot(50);
	}
	glPopMatrix();
	glFlush();
	//交换通道
	glutSwapBuffers();
}

//实现旋转回调函数，使用方向键实现旋转
void rotateFunc(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		angleX += 5.0;
		break;
	case GLUT_KEY_DOWN:
		angleX -= 5.0;
		break;
	case GLUT_KEY_LEFT:
		angleY += 5.0;
		break;
	case GLUT_KEY_RIGHT:
		angleY -= 5.0;
		break;
	default:
		break;
	}
}

//实现缩放回调函数，使用[]控制缩放
void scaleFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case '[':
		if(scaleLevel > 0)
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
}

//实现移动回调函数，使用鼠标控制移动
void translateFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !move) {
		orgX = x;
		orgY = y;
		tempMoveX = moveX;
		tempMoveY = moveY;
		move = true;
	}
	if (state == GLUT_UP && move) {
		move = false;
	}
}

void moveFunc(int x, int y) {
	moveX = tempMoveX - x + orgX;
	moveY = tempMoveY - y + orgY;
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple OpenGL example");
	myinit();
	glutKeyboardFunc(scaleFunc);
	glutSpecialFunc(rotateFunc);
	glutMouseFunc(translateFunc);
	glutMotionFunc(moveFunc);
	glutDisplayFunc(displayScene);
	glutIdleFunc(changeView);
	glutMainLoop();
	return 0;
}