/*
1.	���ú��ʵ����λ�ú����ͶӰ����ʹ��OpenGL����ܹ�����������Ҫ���Ƶ����塣
2.	�򵥵ػ���һ�������壬Ҫ��
a.	�ܹ�6���棬ÿ������2������������ʾ
b.	Ϊÿ������ָ����ɫ
c.	��Ⱦģʽ�ֱ����smoothģʽ��flatģʽ �����glShadeMode������
d.	�����Ȼ����������á��÷����ȽϿ���/��������Ȼ������� glEnable(GL_DEPTHTEST) ����Ч��
*******
* @author�������
*/
#include <cmath> 
#include <cstdlib>
#include <gl/glut.h>

//���ó�ʼ����
void myinit() {
	//���ñ���Ϊ��ɫ
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//������Ȼ���
	glEnable(GL_DEPTH_TEST);
	//���ù۲����
	glMatrixMode(GL_MODELVIEW);

	//��ʼ������ϵ
	glLoadIdentity();

	//���õ�ǰ�����λ�úͷ���
	gluLookAt(
		0.0, 0.0, 0.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, -1.0);

	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100);
	glMatrixMode(GL_MODELVIEW);

	glShadeModel(GL_SMOOTH);
}

void displayScene() {
	//��մ���
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
	//����ͨ��
	glutSwapBuffers();
}

//����Esc�˳�
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