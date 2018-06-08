#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_
#include <gl/glut.h>
#include <math.h>

class Light {
public:
	Light();

	void setLightPosition(const GLfloat* params);
	void setLightPosition(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC);

	void setAmbientLight(const GLfloat* params);
	void setDiffuseLight(const GLfloat* params);
	void setSpecularLight(const GLfloat* params);
	void setAmbientLight(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC);
	void setDiffuseLight(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC);
	void setSpecularLight(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC);

	void setAmbientMaterial(const GLfloat* params);
	void setDiffuseMaterial(const GLfloat* params);
	void setSpecularMaterial(const GLfloat* params);
	void setAmbientMaterial(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC);
	void setDiffuseMaterial(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC);
	void setSpecularMaterial(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC);

	void setLightColor(const GLfloat* camPosition, const GLfloat* pointPosition, const GLfloat* vector);

private:
	GLfloat lightPosition[3];
	//��Դ
	GLfloat ambientLight[3];
	GLfloat diffuseLight[3];
	GLfloat specularLight[3];
	//����
	GLfloat ambientMaterial[3];
	GLfloat diffuseMaterial[3];
	GLfloat specularMaterial[3];
	//����ϵ��
	GLfloat specularStrength;
	
	void setMatrix(GLfloat* result, const GLfloat* params);
	GLfloat max(GLfloat numA, GLfloat numB);
	void normalize(GLfloat* result, const GLfloat* origin);
	GLfloat dot(const GLfloat* matA, const GLfloat* matB);
	void reflect(GLfloat* result, const GLfloat* matA, const GLfloat* matB);
};

Light::Light() {
	for (int i = 0; i < 3; i++) {
		lightPosition[i] = 0.0;
		ambientLight[i] = 0.0;
		diffuseLight[i] = 1.0;
		specularLight[i] = 1.0;
		ambientMaterial[i] = 0.2;
		diffuseMaterial[i] = 0.8;
		specularMaterial[i] = 0.0;
	}
	specularStrength = 3.0;
}

void Light::setLightColor(const GLfloat* camPosition, const GLfloat* pointPosition, const GLfloat* vector) {
	//�ö����ڶ�Ӧ�����µ���ɫ
	GLfloat ambientColor[3], diffuseColor[3], specularColor[3];

	//�󻷾�����ɫ
	for (int i = 0; i < 3; i++) {
		ambientColor[i] = ambientLight[i] * ambientMaterial[i];
	}

	//�淶���ķ���
	GLfloat normal[3];
	normalize(normal, vector);

	//���������������������������������
	GLfloat inVector[3], outVector[3], reverseInVector[3];
	for (int i = 0; i < 3; i++) {
		inVector[i] = pointPosition[i] - lightPosition[i];
		outVector[i] = lightPosition[i] - 2 * normal[i] * dot(normal, inVector);
		reverseInVector[i] = -inVector[i];
	}

	//����������
	GLfloat diffuseVector[3];
	normalize(diffuseVector, reverseInVector);
	//������ϵ��
	GLfloat diffuseFactor = max(dot(diffuseVector, normal), 0.0);
	//�����������ɫ
	for (int i = 0; i < 3; i++) {
		diffuseColor[i] = diffuseFactor * diffuseLight[i] * diffuseMaterial[i];
	}

	//���淴������
	GLfloat specularVector[3];
	normalize(specularVector, outVector);
	//���߷�������
	GLfloat viewVector[3];
	for (int i = 0; i < 3; i++) {
		viewVector[i] = camPosition[i] - pointPosition[i];
	}
	//���淴��ϵ��
	GLfloat specularFactor = pow(max(dot(specularVector, viewVector), 0.0), specularStrength);
	//���淴�����ɫ
	for (int i = 0; i < 3; i++) {
		specularColor[i] = specularFactor * specularLight[i] * specularMaterial[i];
	}

	//������ɫ
	GLfloat resultColor[3];
	for (int i = 0; i < 3; i++) {
		resultColor[i] = ambientColor[i] + diffuseColor[i] + specularColor[i];
	}
	glColor3fv(resultColor);
}

void Light::setLightPosition(const GLfloat* params) {
	setMatrix(lightPosition, params);
}

void Light::setAmbientLight(const GLfloat* params) {
	setMatrix(ambientLight, params);
}

void Light::setDiffuseLight(const GLfloat* params) {
	setMatrix(diffuseLight, params);
}

void Light::setSpecularLight(const GLfloat* params) {
	setMatrix(specularLight, params);
}

void Light::setAmbientMaterial(const GLfloat* params) {
	setMatrix(ambientMaterial, params);
}

void Light::setDiffuseMaterial(const GLfloat* params) {
	setMatrix(diffuseMaterial, params);
}

void Light::setSpecularMaterial(const GLfloat* params) {
	setMatrix(specularMaterial, params);
}

void Light::setLightPosition(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC) {
	lightPosition[0] = paramA;
	lightPosition[1] = paramB;
	lightPosition[2] = paramC;
}

void Light::setAmbientLight(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC) {
	ambientLight[0] = paramA;
	ambientLight[1] = paramB;
	ambientLight[2] = paramC;
}

void Light::setDiffuseLight(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC) {
	diffuseLight[0] = paramA;
	diffuseLight[1] = paramB;
	diffuseLight[2] = paramC;
}

void Light::setSpecularLight(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC) {
	specularLight[0] = paramA;
	specularLight[1] = paramB;
	specularLight[2] = paramC;
}

void Light::setAmbientMaterial(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC) {
	ambientMaterial[0] = paramA;
	ambientMaterial[1] = paramB;
	ambientMaterial[2] = paramC;
}

void Light::setDiffuseMaterial(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC) {
	diffuseMaterial[0] = paramA;
	diffuseMaterial[1] = paramB;
	diffuseMaterial[2] = paramC;
}

void Light::setSpecularMaterial(const GLfloat paramA, const GLfloat paramB, const GLfloat paramC) {
	specularMaterial[0] = paramA;
	specularMaterial[1] = paramB;
	specularMaterial[2] = paramC;
}

void Light::setMatrix(GLfloat* result, const GLfloat* params) {
	for (int i = 0; i < 3; i++) {
		result[i] = params[i];
	}
}

GLfloat Light::max(GLfloat numA, GLfloat numB) {
	return (numA > numB) ? numA : numB;
}

void Light::normalize(GLfloat* result, const GLfloat* origin) {
	float length = sqrt(origin[0] * origin[0] + origin[1] * origin[1] + origin[2] * origin[2]);
	for (int i = 0; i < 3; i++) {
		result[i] = origin[i] / length;
	}
}

GLfloat Light::dot(const GLfloat* matA, const GLfloat* matB) {
	GLfloat result = 0.0;
	for (int i = 0; i < 3; i++) {
		result += matA[i] * matB[i];
	}
	return result;
}

#endif // !_LIGHT_MANAGER_H_
