/*
* 小球落地
* 使用二维圆形实现
* 实现了上升与下降过程中速度的变化以及落地时的形变（不明显）
*******
* @author：宋灵冰
*/
#include <math.h> 
#include <gl/glut.h>

// 指定圆边缘精细程度
const int n = 100;
// 指定圆的半径
const GLfloat R = 0.15;
// 指定PI值
const GLfloat Pi = 3.1415926536;
// 指定运算精度
const GLfloat accuracy = 1e-7;
// 指定地面位置
const GLfloat minHeight = R - 1.0;
// 指定上升最大高度
GLfloat maxHeight = 1.0 - R;
// 指定圆心高度
GLfloat yPos = 1.0 - R;
// 指定下降速度
GLfloat gap = 0;
// 指定运动的加速度
const GLfloat acceleration = 1e-6;
// 标记运动状态（下降/上升）
bool isDown = true;

// 显示原始小球
void display() {
	// 清除颜色缓冲
	glClear(GL_COLOR_BUFFER_BIT);
	// 画小球
	glBegin(GL_POLYGON);
	for (int i = 0; i<n; ++i)
		glVertex2f(R*cos(2 * Pi / n * i), R*sin(2 * Pi / n * i) + yPos);
	glEnd();
	glFlush();
	// 交换两个缓冲区指针
	glutSwapBuffers();
}

// 指定小球落地时的形变
// a、b分别为椭圆的参数
void display(float a, float b) {
	// 清除颜色缓冲
	glClear(GL_COLOR_BUFFER_BIT);
	// 画椭圆
	glBegin(GL_POLYGON);
	for (int i = 0; i<n; ++i)
		glVertex2f(a * cos(2 * Pi / n * i), b * sin(2 * Pi / n * i) + yPos);
	glEnd();
	glFlush();
	// 交换两个缓冲区指针
	glutSwapBuffers();
}

// 指定小球运动的函数
void idle() {
	// 下降状态，速度改变
	if (yPos > minHeight && isDown) {
		yPos -= gap;
		gap += acceleration;
		display();
	}
	// 上升状态，速度改变
	else if (yPos < maxHeight && !isDown) {
		yPos += gap;
		if (gap > accuracy)
			gap -= acceleration;
		// 若速度降为0，
		// 则将当前高度设置为最高点
		else {
			// 改变运动状态
			isDown = true;
			maxHeight = yPos;
		}
		display();
	}
	// 接触地面
	else {
		// 根据最高位置占总高度的比例计算形变程度
		float scale = (maxHeight + 1) / 2 * R;
		// 若满足静止条件，则将高度置0
		if (maxHeight < minHeight + accuracy) {
			maxHeight = -1;
		}
		// 开始挤压形变
		else if (yPos > minHeight - scale && isDown) {
			yPos -= gap;
			gap += acceleration;
			display(R, 1 + yPos);
		}
		// 开始恢复原状
		else if (yPos < minHeight && !isDown) {
			yPos += gap;
			gap -= acceleration;
			display(R, 1 + yPos);
		}
		else {
			// 指定撞击地面后反弹回来速度改变
			gap *= 0.85;
			// 改变运动状态
			isDown = false;
		}
	}
}

// 设置键盘退出指令函数
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
	// 设置双缓冲、RGB模式
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ball");
	// 注册一个绘图函数
	glutDisplayFunc(display);
	// 为键盘消息注册回调
	glutKeyboardFunc(keyboard);
	// 设置全局的回调函数
	glutIdleFunc(idle);
	// 进入GLUT事件处理循环
	glutMainLoop();
	return 0;
}
