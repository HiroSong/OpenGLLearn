/*
* 该程序实现以下功能
* 1. 定义了myRotateX、myRotateY、myRotateZ函数，
* 分别实现绕X、Y、Z轴旋转的旋转矩阵计算
* 2. 分别调用自定义函数与opengl内置函数，输出旋转矩阵结果，进行验证
*******
* @author：宋灵冰
*/
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

void PrintMatrix(float matrix[16])
{
	printf("%8.3f%8.3f%8.3f%8.3f\n"
		"%8.3f%8.2f%8.3f%8.3f\n"
		"%8.3f%8.2f%8.3f%8.3f\n"
		"%8.3f%8.2f%8.3f%8.3f\n\n",
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}

void myRotateZ(float alpha)
{
	float a = alpha / 180 * 3.14159;
	float R[16] = { 0 };
	R[0] = cos(a);
	R[1] = sin(a);
	R[4] = -sin(a);
	R[5] = cos(a);
	R[10] = 1;
	R[15] = 1;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(R);
}


void myRotateX(float alpha)
{
	float a = alpha / 180 * 3.14159;
	float R[16] = { 0 };
	R[0] = 1;
	R[5] = cos(a);
	R[6] = sin(a);
	R[9] = -sin(a);
	R[10] = cos(a);
	R[15] = 1;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(R);
}

void myRotateY(float alpha)
{
	float a = alpha / 180 * 3.14159;
	float R[16] = { 0 };
	R[0] = cos(a);
	R[2] = -sin(a);
	R[5] = 1;
	R[8] = sin(a);
	R[10] = cos(a);
	R[15] = 1;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(R);
}

void MyOrtho(float left, float right, float bottom, float top, float near0, float far0)
{
	float M[16] = { 0 };
	M[0] = 2 / (right - left);
	M[5] = 2 / (top - bottom);
	M[10] = -2 / (far0 - near0);
	M[12] = -(right + left) / (right - left);
	M[13] = -(top + bottom) / (top - bottom);
	M[14] = -(far0 + near0) / (far0 - near0);
	M[15] = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(M);
}

void display()
{
	float R[16];
	glMatrixMode(GL_MODELVIEW);

	printf("Test: myRotateX----------\n");
	printf("myRotateX(60.0);----------\n");
	glLoadIdentity();
	myRotateX(60.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, R);
	PrintMatrix(R);

	printf("glRotatef(60.0, 1.0f, 0.0f, 0.0f);----------\n");
	glLoadIdentity();
	glRotatef(60.0, 1.0f, 0.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, R);
	PrintMatrix(R);

	printf("Test: myRotateY----------\n");
	printf("myRotateY(60.0);----------\n");
	glLoadIdentity();
	myRotateY(60.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, R);
	PrintMatrix(R);

	printf("glRotatef(60.0, 0.0f, 1.0f, 0.0f);----------\n");
	glLoadIdentity();
	glRotatef(60.0, 0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, R);
	PrintMatrix(R);

	printf("Test: myRotateZ----------\n");
	printf("myRotateZ(60.0);----------\n");
	glLoadIdentity();
	myRotateZ(60.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, R);
	PrintMatrix(R);

	printf("glRotatef(60.0, 0.0f, 0.0f, 1.0f);----------\n");
	glLoadIdentity();
	glRotatef(60.0, 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, R);
	PrintMatrix(R);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(10, 10);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Test");
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

