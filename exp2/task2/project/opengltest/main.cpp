/*
* ש�鵲������
* ʵ����ש����ƶ�����ķ����Լ���Ϸʧ�ܵ��ж�
* Ϊ��ģ��ʵ������Ч����ʵ����С�����˶���ש���Ϸ���ʱ������ǵĸı�
* ������Ӧ�ٶ�������δ�ҵ�ԭ��
*******
* @author�������
*/
#include <cmath> 
#include <cstdlib>
#include <gl/glut.h>


//����ש������
GLfloat brickCenter[2];
//����ש����һ��
const GLfloat HALFBRICKWIDTH = 10;
//����ש��߶�һ��
const GLfloat HALFBRICKHEIGHT = 2.5;
//����ש�����ڸ߶�
const GLfloat BRICKHEIGHT = 12.5;
//ש����ɫ
const GLfloat BRICKCOLOR[3] = { 1, 106 / 255.0, 106 / 255.0 };
//����ש���ƶ�����
const GLfloat BRICKSTEP = 2;
//����ש�����ܵ���ı߽�
const GLfloat BRICKLEFTSIDE = HALFBRICKWIDTH;
const GLfloat BRICKRIGHTSIDE = 100 - HALFBRICKWIDTH;

//����PIֵ
const GLfloat PI = 3.14159;
//����ֱ��
const GLfloat HALFPI = PI / 2;
//����С����ƾ���
const int N = 100;
//����С������
GLfloat ballCenter[2];
//����С��뾶
const GLfloat BALLRADIUS = 3;
//����С���ʼ�߶�
const GLfloat INITBALLHEIGHT = BRICKHEIGHT + BALLRADIUS;
//����С�����ܵ���ı߽�
const GLfloat BALLLEFTSIDE = BALLRADIUS;
const GLfloat BALLRIGHTSIDE = 100 - BALLRADIUS;
const GLfloat BALLMAXHEIGHT = 100 - BALLRADIUS;
//����С����ɫ
const GLfloat BALLCOLOR[3] = { 205 / 256.0, 201 / 256.0, 201 / 256.0 };
//����С���ƶ�����
const GLfloat BALLSTEP = 0.15;
//����С���ƶ�����
GLfloat ballDirectionVector[2];

//���ýǶȸı����
const GLfloat ANGLESTEP = PI / 18;
//��¼��ǰ�ƶ��Ƕ�
GLfloat nowAngle;

//������Ϸ��ʼ���
bool startGame = false;
//������������±��
bool leftOn = false;
//�����ҷ�������±��
bool rightOn = false;
//���øոշ���ı��
//��ֹС����ש���Ϸ����ı�Ƕ�
bool justStart = false;

//�ı�С������λ��
void setBallCenter(GLfloat x, GLfloat y) {
	ballCenter[0] = x;
	ballCenter[1] = y;
}

//�ı�ש������λ��
void setBrickCenter(GLfloat x, GLfloat y = 10) {
	brickCenter[0] = x;
	brickCenter[1] = y;
}

//�ı�С���ƶ�����
void setBallDirectionVector(GLfloat angle) {
	nowAngle = angle;
	ballDirectionVector[0] = cos(angle);
	ballDirectionVector[1] = sin(angle);
}

//�ж�С���Ƿ�ɹ����ӵ�
bool beingTouch() {
	bool heightSatisfied1 = (ballCenter[1] - INITBALLHEIGHT <= BALLSTEP);
	bool heightSatisfied2 = (ballCenter[1] - INITBALLHEIGHT >= 0);
	bool widthSatisfied = (abs(ballCenter[0] - brickCenter[0]) <= HALFBRICKWIDTH);
	return heightSatisfied1 && heightSatisfied2 && widthSatisfied;
}

//�ж�С���Ƿ�δ���ӵ�����ש��Ӵ�
bool missButTouch() {
	bool heightSatisfied = (abs(brickCenter[1] - ballCenter[1]) < HALFBRICKHEIGHT);
	bool widthSatisfied1 = (abs(ballCenter[0] - brickCenter[0]) < (HALFBRICKWIDTH + BALLRADIUS));
	bool widthSatisfied2 = (abs(ballCenter[0] - brickCenter[0]) > HALFBRICKWIDTH);
	return heightSatisfied && widthSatisfied1 && widthSatisfied2;
}

//���ó�ʼ����
void myinit() {
	//���ñ���Ϊ��ɫ
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//���ù۲����
	glMatrixMode(GL_PROJECTION);

	//��ʼ������ϵ
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	//����ש���С���ʼλ��
	setBrickCenter(50, 10);
	setBallCenter(50, INITBALLHEIGHT);
}

void displayScene() {
	//��մ���
	glClear(GL_COLOR_BUFFER_BIT);

	//����С����ɫ
	glColor3f(BALLCOLOR[0], BALLCOLOR[1], BALLCOLOR[2]);
	//��ʼ����С��
	glBegin(GL_POLYGON);
	for (int i = 0; i<N; ++i)
		glVertex2f(BALLRADIUS*cos(2 * PI / N * i) + ballCenter[0], BALLRADIUS*sin(2 * PI / N * i) + ballCenter[1]);
	glEnd();

	//����ש����ɫ
	glColor3f(BRICKCOLOR[0], BRICKCOLOR[1], BRICKCOLOR[2]);
	//��ʼ����ש��
	glBegin(GL_POLYGON);
	glVertex2f(brickCenter[0] - HALFBRICKWIDTH, brickCenter[1] + HALFBRICKHEIGHT);
	glVertex2f(brickCenter[0] + HALFBRICKWIDTH, brickCenter[1] + HALFBRICKHEIGHT);
	glVertex2f(brickCenter[0] + HALFBRICKWIDTH, brickCenter[1] - HALFBRICKHEIGHT);
	glVertex2f(brickCenter[0] - HALFBRICKWIDTH, brickCenter[1] - HALFBRICKHEIGHT);
	glEnd();

	glFlush();
	//����ͨ��
	glutSwapBuffers();
}

//����С���ƶ��켣
void setBallTrail() {
	if (!startGame)
		return;
	GLfloat x = ballCenter[0] + ballDirectionVector[0] * BALLSTEP;
	GLfloat y = ballCenter[1] + ballDirectionVector[1] * BALLSTEP;
	if (x<(BALLLEFTSIDE + BALLSTEP) || x>(BALLRIGHTSIDE - BALLSTEP))
		setBallDirectionVector(PI - nowAngle);
	else if (y > (BALLMAXHEIGHT - BALLSTEP))
		setBallDirectionVector(2 * PI - nowAngle);
	setBallCenter(x, y);

	if (leftOn || rightOn)
		return;

	if (justStart) {
		justStart = false;
		//���ñ���Ϊ��ɫ
		glClearColor(1.0, 1.0, 1.0, 1.0);
		return;
	}

	if (beingTouch()) {
		//�ɹ��ӵ�������
		setBallDirectionVector(2 * PI - nowAngle);
	}else if (missButTouch()) {
		//δ�ɹ����򵫴�����������
		setBallDirectionVector(PI - nowAngle);
	}
}

//����С�򶯻�Ч��
void modifyScene(int value) {
	setBallTrail();
	displayScene();
	glutTimerFunc(1, modifyScene, 1);
}

//��Ӧ�����
void responLeft() {
	GLfloat newPosition = brickCenter[0] - BRICKSTEP;
	if (newPosition >= BRICKLEFTSIDE)
		setBrickCenter(newPosition);
	if (!startGame) {
		justStart = true;
		startGame = true;
		setBallDirectionVector(3 * PI / 4);
	}else if (beingTouch()) {
		//�ɹ��ӵ�������
		setBallDirectionVector(2 * PI - nowAngle + ANGLESTEP);
	}else if (missButTouch()) {
		//δ�ɹ����򵫴�����������
		setBallDirectionVector(PI - nowAngle);
	}
}

//��Ӧ�ҷ����
void responRight() {
	GLfloat newPosition = brickCenter[0] + BRICKSTEP;
	if(newPosition <= BRICKRIGHTSIDE)
		setBrickCenter(newPosition);
	if (!startGame) {
		justStart = true;
		startGame = true;
		setBallDirectionVector(PI / 4);
	}else if (beingTouch()){
		//�ɹ��ӵ�������
		setBallDirectionVector(2 * PI - nowAngle - ANGLESTEP);
	}else if (missButTouch()) {
		//δ�ɹ����򵫴�����������
		setBallDirectionVector(PI - nowAngle);
	}
}

//����ש���ƶ�
void controlGame(int key, int x, int y) {
	switch (key) {
		//����������ҿ���ש�鷽��
		case GLUT_KEY_LEFT:
			leftOn = true;
			responLeft();
			leftOn = false;
			break;
		case GLUT_KEY_RIGHT:
			rightOn = true;
			responRight();
			rightOn = false;
			break;
		default:
			break;
	}
}

void gameFailed() {
	if (ballCenter[1] >= -BALLRADIUS)
		return;

	//���ñ��
	startGame = false;

	//���ñ���Ϊ��ɫ
	glClearColor(1.0, 0, 0, 1.0);
	//��մ���
	glClear(GL_COLOR_BUFFER_BIT);

	//����ש���С���ʼλ��
	setBrickCenter(50, 10);
	setBallCenter(50, INITBALLHEIGHT);
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
	glutSpecialFunc(controlGame);
	glutDisplayFunc(displayScene);
	glutTimerFunc(5, modifyScene, 1);
	glutIdleFunc(gameFailed);
	glutMainLoop();
	return 0;
}