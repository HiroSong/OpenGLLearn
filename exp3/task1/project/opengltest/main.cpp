/*
1.	设置合适的相机位置和相机投影矩阵，使得OpenGL相机能够“看到”需要绘制的物体。
2.	简单地绘制一个正方体，要求
a.	总共6个面，每个面用2个三角形来表示
b.	为每个顶点指定颜色
c.	渲染模式分别采用smooth模式和flat模式 （请查glShadeMode函数）
d.	理解深度缓冲区的作用、用法，比较开启/不开启深度缓冲区（ glEnable(GL_DEPTHTEST) ）的效果
*******
* @author：宋灵冰
*/
#include <cmath> 
#include <cstdlib>
#include <gl/glut.h>

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
		1.0, 1.0, 1.0,
		-1.0, 1.0, -1.0);

	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100);
	glMatrixMode(GL_MODELVIEW);

	glShadeModel(GL_SMOOTH);
}

void displayScene() {
	//清空窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLdouble vex[8][3] = { { 25, 25, 25 },{ 25, -25, 25 },{ 25, 25, 75 },{ 25, -25, 75 },{ -25, 25, 25 },{ -25, 25, 75 },{ -25, -25, 25 },{ -25, -25, 75 } };

	glPushMatrix();
	{
		glBegin(GL_TRIANGLES);
		glColor3f(0.1, 0.2, 0.3);
		glVertex3dv(vex[0]);
		glVertex3dv(vex[1]);
		glVertex3dv(vex[2]);
		glColor3f(0.3, 0.2, 0.4);
		glVertex3dv(vex[3]);
		glVertex3dv(vex[1]);
		glVertex3dv(vex[2]);
		glColor3f(0.8, 0.2, 0.4);
		glVertex3dv(vex[3]);
		glVertex3dv(vex[1]);
		glVertex3dv(vex[7]);
		glColor3f(0.2, 0.8, 0.7);
		glVertex3dv(vex[7]);
		glVertex3dv(vex[1]);
		glVertex3dv(vex[6]);
		glColor3f(0.8, 0.4, 0.3);
		glVertex3dv(vex[7]);
		glVertex3dv(vex[3]);
		glVertex3dv(vex[2]);
		glColor3f(0.3, 0.8, 0.6);
		glVertex3dv(vex[7]);
		glVertex3dv(vex[5]);
		glVertex3dv(vex[2]);
		glColor3f(0.7, 0.6, 0.8);
		glVertex3dv(vex[5]);
		glVertex3dv(vex[7]);
		glVertex3dv(vex[6]);
		glColor3f(0.5, 0.6, 0.7);
		glVertex3dv(vex[4]);
		glVertex3dv(vex[5]);
		glVertex3dv(vex[6]);
		glColor3f(0.1, 0.6, 0.3);
		glVertex3dv(vex[5]);
		glVertex3dv(vex[2]);
		glVertex3dv(vex[0]);
		glColor3f(0.1, 0.5, 0.6);
		glVertex3dv(vex[4]);
		glVertex3dv(vex[5]);
		glVertex3dv(vex[0]);
		glColor3f(0.1, 0.7, 0.2);
		glVertex3dv(vex[6]);
		glVertex3dv(vex[1]);
		glVertex3dv(vex[0]);
		glColor3f(0.7, 0.5, 0.1);
		glVertex3dv(vex[4]);
		glVertex3dv(vex[6]);
		glVertex3dv(vex[0]);
		glEnd();
	}
	glPopMatrix();

	glFlush();
	//交换通道
	glutSwapBuffers();
}

//按下Esc退出
void pressToExit(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple OpenGL example");
	myinit();
	glutKeyboardFunc(pressToExit);
	glutDisplayFunc(displayScene);
	glutIdleFunc(displayScene);
	glutMainLoop();
	return 0;
}