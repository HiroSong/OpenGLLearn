/*
* С�����
* ʹ�ö�άԲ��ʵ��
* ʵ�����������½��������ٶȵı仯�Լ����ʱ���α䣨�����ԣ�
*******
* @author�������
*/
#include <math.h> 
#include <gl/glut.h>

// ָ��Բ��Ե��ϸ�̶�
const int n = 100;
// ָ��Բ�İ뾶
const GLfloat R = 0.15;
// ָ��PIֵ
const GLfloat Pi = 3.1415926536;
// ָ�����㾫��
const GLfloat accuracy = 1e-7;
// ָ������λ��
const GLfloat minHeight = R - 1.0;
// ָ���������߶�
GLfloat maxHeight = 1.0 - R;
// ָ��Բ�ĸ߶�
GLfloat yPos = 1.0 - R;
// ָ���½��ٶ�
GLfloat gap = 0;
// ָ���˶��ļ��ٶ�
const GLfloat acceleration = 1e-6;
// ����˶�״̬���½�/������
bool isDown = true;

// ��ʾԭʼС��
void display() {
	// �����ɫ����
	glClear(GL_COLOR_BUFFER_BIT);
	// ��С��
	glBegin(GL_POLYGON);
	for (int i = 0; i<n; ++i)
		glVertex2f(R*cos(2 * Pi / n * i), R*sin(2 * Pi / n * i) + yPos);
	glEnd();
	glFlush();
	// ��������������ָ��
	glutSwapBuffers();
}

// ָ��С�����ʱ���α�
// a��b�ֱ�Ϊ��Բ�Ĳ���
void display(float a, float b) {
	// �����ɫ����
	glClear(GL_COLOR_BUFFER_BIT);
	// ����Բ
	glBegin(GL_POLYGON);
	for (int i = 0; i<n; ++i)
		glVertex2f(a * cos(2 * Pi / n * i), b * sin(2 * Pi / n * i) + yPos);
	glEnd();
	glFlush();
	// ��������������ָ��
	glutSwapBuffers();
}

// ָ��С���˶��ĺ���
void idle() {
	// �½�״̬���ٶȸı�
	if (yPos > minHeight && isDown) {
		yPos -= gap;
		gap += acceleration;
		display();
	}
	// ����״̬���ٶȸı�
	else if (yPos < maxHeight && !isDown) {
		yPos += gap;
		if (gap > accuracy)
			gap -= acceleration;
		// ���ٶȽ�Ϊ0��
		// �򽫵�ǰ�߶�����Ϊ��ߵ�
		else {
			// �ı��˶�״̬
			isDown = true;
			maxHeight = yPos;
		}
		display();
	}
	// �Ӵ�����
	else {
		// �������λ��ռ�ܸ߶ȵı��������α�̶�
		float scale = (maxHeight + 1) / 2 * R;
		// �����㾲ֹ�������򽫸߶���0
		if (maxHeight < minHeight + accuracy) {
			maxHeight = -1;
		}
		// ��ʼ��ѹ�α�
		else if (yPos > minHeight - scale && isDown) {
			yPos -= gap;
			gap += acceleration;
			display(R, 1 + yPos);
		}
		// ��ʼ�ָ�ԭ״
		else if (yPos < minHeight && !isDown) {
			yPos += gap;
			gap -= acceleration;
			display(R, 1 + yPos);
		}
		else {
			// ָ��ײ������󷴵������ٶȸı�
			gap *= 0.85;
			// �ı��˶�״̬
			isDown = false;
		}
	}
}

// ���ü����˳�ָ���
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	// ����˫���塢RGBģʽ
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ball");
	// ע��һ����ͼ����
	glutDisplayFunc(display);
	// Ϊ������Ϣע��ص�
	glutKeyboardFunc(keyboard);
	// ����ȫ�ֵĻص�����
	glutIdleFunc(idle);
	// ����GLUT�¼�����ѭ��
	glutMainLoop();
	return 0;
}
