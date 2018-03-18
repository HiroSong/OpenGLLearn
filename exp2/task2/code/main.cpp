/*
* 砖块挡反弹球
* 实现了砖块的移动和球的反弹以及游戏失败的判断
* 为了模拟实际物理效果，实现了小球在运动的砖块上反弹时，反射角的改变
* 键盘响应速度慢，暂未找到原因
*******
* @author：宋灵冰
*/
#include <cmath> 
#include <cstdlib>
#include <gl/glut.h>


//设置砖块中心
GLfloat brickCenter[2];
//设置砖块宽度一半
const GLfloat HALFBRICKWIDTH = 10;
//设置砖块高度一半
const GLfloat HALFBRICKHEIGHT = 2.5;
//设置砖块所在高度
const GLfloat BRICKHEIGHT = 12.5;
//砖块颜色
const GLfloat BRICKCOLOR[3] = { 1, 106 / 255.0, 106 / 255.0 };
//设置砖块移动幅度
const GLfloat BRICKSTEP = 2;
//设置砖块所能到达的边界
const GLfloat BRICKLEFTSIDE = HALFBRICKWIDTH;
const GLfloat BRICKRIGHTSIDE = 100 - HALFBRICKWIDTH;

//设置PI值
const GLfloat PI = 3.14159;
//设置直角
const GLfloat HALFPI = PI / 2;
//设置小球绘制精度
const int N = 100;
//设置小球中心
GLfloat ballCenter[2];
//设置小球半径
const GLfloat BALLRADIUS = 3;
//设置小球初始高度
const GLfloat INITBALLHEIGHT = BRICKHEIGHT + BALLRADIUS;
//设置小球所能到达的边界
const GLfloat BALLLEFTSIDE = BALLRADIUS;
const GLfloat BALLRIGHTSIDE = 100 - BALLRADIUS;
const GLfloat BALLMAXHEIGHT = 100 - BALLRADIUS;
//设置小球颜色
const GLfloat BALLCOLOR[3] = { 205 / 256.0, 201 / 256.0, 201 / 256.0 };
//设置小球移动幅度
const GLfloat BALLSTEP = 0.15;
//设置小球移动方向
GLfloat ballDirectionVector[2];

//设置角度改变幅度
const GLfloat ANGLESTEP = PI / 18;
//记录当前移动角度
GLfloat nowAngle;

//设置游戏开始标记
bool startGame = false;
//设置左方向键按下标记
bool leftOn = false;
//设置右方向键按下标记
bool rightOn = false;
//设置刚刚发球的标记
//防止小球在砖块上反复改变角度
bool justStart = false;

//改变小球中心位置
void setBallCenter(GLfloat x, GLfloat y) {
	ballCenter[0] = x;
	ballCenter[1] = y;
}

//改变砖块中心位置
void setBrickCenter(GLfloat x, GLfloat y = 10) {
	brickCenter[0] = x;
	brickCenter[1] = y;
}

//改变小球移动方向
void setBallDirectionVector(GLfloat angle) {
	nowAngle = angle;
	ballDirectionVector[0] = cos(angle);
	ballDirectionVector[1] = sin(angle);
}

//判断小球是否成功被接到
bool beingTouch() {
	bool heightSatisfied1 = (ballCenter[1] - INITBALLHEIGHT <= BALLSTEP);
	bool heightSatisfied2 = (ballCenter[1] - INITBALLHEIGHT >= 0);
	bool widthSatisfied = (abs(ballCenter[0] - brickCenter[0]) <= HALFBRICKWIDTH);
	return heightSatisfied1 && heightSatisfied2 && widthSatisfied;
}

//判断小球是否未被接到但与砖块接触
bool missButTouch() {
	bool heightSatisfied = (abs(brickCenter[1] - ballCenter[1]) < HALFBRICKHEIGHT);
	bool widthSatisfied1 = (abs(ballCenter[0] - brickCenter[0]) < (HALFBRICKWIDTH + BALLRADIUS));
	bool widthSatisfied2 = (abs(ballCenter[0] - brickCenter[0]) > HALFBRICKWIDTH);
	return heightSatisfied && widthSatisfied1 && widthSatisfied2;
}

//设置初始属性
void myinit() {
	//设置背景为白色
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//设置观察参数
	glMatrixMode(GL_PROJECTION);

	//初始化坐标系
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	//设置砖块和小球初始位置
	setBrickCenter(50, 10);
	setBallCenter(50, INITBALLHEIGHT);
}

void displayScene() {
	//清空窗口
	glClear(GL_COLOR_BUFFER_BIT);

	//设置小球颜色
	glColor3f(BALLCOLOR[0], BALLCOLOR[1], BALLCOLOR[2]);
	//开始绘制小球
	glBegin(GL_POLYGON);
	for (int i = 0; i<N; ++i)
		glVertex2f(BALLRADIUS*cos(2 * PI / N * i) + ballCenter[0], BALLRADIUS*sin(2 * PI / N * i) + ballCenter[1]);
	glEnd();

	//设置砖块颜色
	glColor3f(BRICKCOLOR[0], BRICKCOLOR[1], BRICKCOLOR[2]);
	//开始绘制砖块
	glBegin(GL_POLYGON);
	glVertex2f(brickCenter[0] - HALFBRICKWIDTH, brickCenter[1] + HALFBRICKHEIGHT);
	glVertex2f(brickCenter[0] + HALFBRICKWIDTH, brickCenter[1] + HALFBRICKHEIGHT);
	glVertex2f(brickCenter[0] + HALFBRICKWIDTH, brickCenter[1] - HALFBRICKHEIGHT);
	glVertex2f(brickCenter[0] - HALFBRICKWIDTH, brickCenter[1] - HALFBRICKHEIGHT);
	glEnd();

	glFlush();
	//交换通道
	glutSwapBuffers();
}

//设置小球移动轨迹
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
		//设置背景为白色
		glClearColor(1.0, 1.0, 1.0, 1.0);
		return;
	}

	if (beingTouch()) {
		//成功接到球的情况
		setBallDirectionVector(2 * PI - nowAngle);
	}else if (missButTouch()) {
		//未成功接球但触碰到球的情况
		setBallDirectionVector(PI - nowAngle);
	}
}

//设置小球动画效果
void modifyScene(int value) {
	setBallTrail();
	displayScene();
	glutTimerFunc(1, modifyScene, 1);
}

//响应左方向键
void responLeft() {
	GLfloat newPosition = brickCenter[0] - BRICKSTEP;
	if (newPosition >= BRICKLEFTSIDE)
		setBrickCenter(newPosition);
	if (!startGame) {
		justStart = true;
		startGame = true;
		setBallDirectionVector(3 * PI / 4);
	}else if (beingTouch()) {
		//成功接到球的情况
		setBallDirectionVector(2 * PI - nowAngle + ANGLESTEP);
	}else if (missButTouch()) {
		//未成功接球但触碰到球的情况
		setBallDirectionVector(PI - nowAngle);
	}
}

//响应右方向键
void responRight() {
	GLfloat newPosition = brickCenter[0] + BRICKSTEP;
	if(newPosition <= BRICKRIGHTSIDE)
		setBrickCenter(newPosition);
	if (!startGame) {
		justStart = true;
		startGame = true;
		setBallDirectionVector(PI / 4);
	}else if (beingTouch()){
		//成功接到球的情况
		setBallDirectionVector(2 * PI - nowAngle - ANGLESTEP);
	}else if (missButTouch()) {
		//未成功接球但触碰到球的情况
		setBallDirectionVector(PI - nowAngle);
	}
}

//控制砖块移动
void controlGame(int key, int x, int y) {
	switch (key) {
		//点击键盘左右控制砖块方向
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

	//重置标记
	startGame = false;

	//设置背景为红色
	glClearColor(1.0, 0, 0, 1.0);
	//清空窗口
	glClear(GL_COLOR_BUFFER_BIT);

	//设置砖块和小球初始位置
	setBrickCenter(50, 10);
	setBallCenter(50, INITBALLHEIGHT);
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
	glutSpecialFunc(controlGame);
	glutDisplayFunc(displayScene);
	glutTimerFunc(5, modifyScene, 1);
	glutIdleFunc(gameFailed);
	glutMainLoop();
	return 0;
}