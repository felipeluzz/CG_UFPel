#ifndef CAMERA_H
#define CAMERA_H
#define ANIMSTEP 40

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <iterator>
#include <stack>
#include <queue>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include AntTweakBar
#include <AntTweakBar.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>
#include <meshSimplification.hpp>


struct transformC {

	double x;
	double y;
	double z;
	double p1;
	double p2;
	double p3;
	float angle;
	// 0: Translação  |  1: Escala  | 2: Rotação | 3: Look At  |  4: Rotação em um ponto  | 5: Linear  |  6: B-Spline  |  7: Bézier
	unsigned short transformationID;
};

//Classe que contém a câmera
class camera {

public:
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	glm::mat4 ProjectionMatrix;
	glm::mat4 transformation;
	glm::mat4 ViewMatrix;
	std::queue <struct transformC> transformationQueue;
	std::queue <struct transformC> animationQueue;
	int flagR = 0;

public:
	camera(GLuint programID) {
		ViewMatrixID = glGetUniformLocation(programID, "V");
		ModelMatrixID = glGetUniformLocation(programID, "M");
		transformation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,20.0f) * glm::vec3(-1));
		ProjectionMatrix = getProjectionMatrix() * transformation;
		ViewMatrix = getViewMatrix();
	}

	camera(float fieldOfView, float aspectRatio, float near, float far, glm::vec3 cameraPosition, glm::vec3 upVector, glm::vec3 sightDirection, GLuint programID) {
		ProjectionMatrix = glm::perspective(fieldOfView, aspectRatio, near, far);
		ViewMatrix = glm::lookAt(cameraPosition, cameraPosition + sightDirection, upVector);
		ViewMatrixID = glGetUniformLocation(programID, "V");
	}

	void bind();
	int setTransformation(float seconds);
	void addTransformation(struct transformC);
	int animate();
	glm::vec3 NaraujoNoise();
};

#endif