/*
* �ó���ʵ�����¹���
* 1.	���ú��ʵ����λ�ú����ͶӰ����ʹ��OpenGL����ܹ�����������Ҫ���Ƶ����塣
* 2.	��glutWireTeapot�������Ʋ����
* 3.	ʹ��glRotate��glTranslate��glScale�������������б任
* 4.  ʹ������϶��������ʵ���ƶ���ʹ�÷�������Կ�����ת��ʹ�� [ ]����Ӣ�ļ��̣����Կ�������
*******
* @author�������
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
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0);

	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

//���ڳ�ʼ��ʾͼ��
void displayScene() {
	//��մ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	{
		//���ò����ɫ
		glColor3d(153.0/255.0, 49.0/255.0, 30.0/255.0);
		glutWireTeapot(20);
	}
	glPopMatrix();
	glFlush();
	//����ͨ��
	glutSwapBuffers();
}

//���ÿ��лص�������������Ӧ״̬�仯
void changeView() {
	//��մ���
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
	//����ͨ��
	glutSwapBuffers();
}

//ʵ����ת�ص�������ʹ�÷����ʵ����ת
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

//ʵ�����Żص�������ʹ��[]��������
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

//ʵ���ƶ��ص�������ʹ���������ƶ�
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