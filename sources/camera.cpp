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
#include <Camera.hpp>
#include <glm/gtx/spline.hpp>

void camera::bind() {
	ProjectionMatrix = getProjectionMatrix();
	if (flagR == 0)
		ViewMatrix = transformation * getViewMatrix();
	else if (flagR == 1)
		ViewMatrix = transformation;
}

//Adiciona transformação na fila
void camera::addTransformation(struct transformC transformation) {

	struct transformC trans;

	trans = transformation;
	transformationQueue.push(trans);
	std::cout << "\nAdicionado na fila da câmera: " << transformation.x << " | " << transformation.y << " | " << transformation.z << "\n";
}

int camera::setTransformation(float seconds) {

	if (transformationQueue.empty())
		return 0;

	glm::vec3 linear, pontoAntigo, p0, p1, p2, p3, newPos, currentPos(ViewMatrix[3][0],ViewMatrix[3][1],ViewMatrix[3][2]);
	int step = seconds * ANIMSTEP, flag = 0;
	float tam = transformationQueue.size() - 1.0f;
	if (step < 1)
		step = 1;

	while (transformationQueue.size() != 0) {

		//Fatia a transformação
		struct transformC animation;

		//Para translação
		if (transformationQueue.front().transformationID == 0) {
			animation.x = (transformationQueue.front().x / step);
			animation.y = (transformationQueue.front().y / step);
			animation.z = (transformationQueue.front().z / step);
			animation.angle = (transformationQueue.front().angle / step);
			animation.transformationID = transformationQueue.front().transformationID;
			transformationQueue.pop();

			//Coloca a transformação fatiada numa fila
			for (int i = 0; i < step; i++)
				animationQueue.push(animation);
		}

		//Para rotação
		else if (transformationQueue.front().transformationID == 2) {
			animation.x = transformationQueue.front().x;
			animation.y = transformationQueue.front().y;
			animation.z = transformationQueue.front().z;
			animation.angle = (transformationQueue.front().angle / step);
			animation.transformationID = transformationQueue.front().transformationID;
			transformationQueue.pop();

			//Coloca a transformação fatiada numa fila
			for (int i = 0; i < step; i++)
				animationQueue.push(animation);
		}

		//Para Look At
		else if (transformationQueue.front().transformationID == 3) {
			animation.p1 = transformationQueue.front().p1;
			animation.p2 = transformationQueue.front().p2;
			animation.p3 = transformationQueue.front().p3;
			animation.x = transformationQueue.front().x;
			animation.y = transformationQueue.front().y;
			animation.z = transformationQueue.front().z;
			animation.transformationID = transformationQueue.front().transformationID;
			transformationQueue.pop();

			//Coloca a transformação fatiada numa fila
			for (int i = 0; i < 1; i++)
				animationQueue.push(animation);
		}

		//Para rotação em um ponto
		else if (transformationQueue.front().transformationID == 4) {
			animation.x = transformationQueue.front().x;
			animation.y = transformationQueue.front().y;
			animation.z = transformationQueue.front().z;
			animation.angle = (transformationQueue.front().angle / step);
			animation.transformationID = transformationQueue.front().transformationID;
			transformationQueue.pop();

			//Coloca a transformação fatiada numa fila
			for (int i = 0; i < step; i++)
				animationQueue.push(animation);
		}

		//Para linear
		else if (transformationQueue.front().transformationID == 5) {
			if (flag == 0) { //Caso primeiro ponto
				animation.x = (transformationQueue.front().x);
				animation.y = (transformationQueue.front().y);
				animation.z = (transformationQueue.front().z);
				animation.transformationID = transformationQueue.front().transformationID;
				pontoAntigo.x = animation.x;
				pontoAntigo.y = animation.y;
				pontoAntigo.z = animation.z;
				transformationQueue.pop();

				//Coloca a transformação fatiada numa fila
				for (int i = 0; i < 1; i++)
					animationQueue.push(animation);
				flag = 1;
			}
			else {  //Caso demais pontos
				linear.x = transformationQueue.front().x - pontoAntigo.x;
				linear.y = transformationQueue.front().y - pontoAntigo.y;
				linear.z = transformationQueue.front().z - pontoAntigo.z;
				pontoAntigo.x = transformationQueue.front().x;
				pontoAntigo.y = transformationQueue.front().y;
				pontoAntigo.z = transformationQueue.front().z;
				animation.x = (linear.x / (step / tam));
				animation.y = (linear.y / (step / tam));
				animation.z = (linear.z / (step / tam));
				animation.transformationID = transformationQueue.front().transformationID;
				transformationQueue.pop();

				//Coloca a transformação fatiada numa fila
				for (int i = 0; i < (step / tam); i++)
					animationQueue.push(animation);
			}
		}
		//Para B-Spline
		else if (transformationQueue.front().transformationID == 6) {

			double lod = step;

			p0.x = transformationQueue.front().x;
			p0.y = transformationQueue.front().y;
			p0.z = transformationQueue.front().z;
			transformationQueue.pop();
			p1.x = transformationQueue.front().x;
			p1.y = transformationQueue.front().y;
			p1.z = transformationQueue.front().z;
			transformationQueue.pop();
			p2.x = transformationQueue.front().x;
			p2.y = transformationQueue.front().y;
			p2.z = transformationQueue.front().z;
			transformationQueue.pop();
			p3.x = transformationQueue.front().x;
			p3.y = transformationQueue.front().y;
			p3.z = transformationQueue.front().z;
			transformationQueue.pop();

			for (double i = 0.0f; i < lod; i++) {
				newPos = glm::cubic(p3, p2, p1, p0, i / lod);
				newPos -= currentPos;	
				currentPos += newPos;
				animation.x = newPos.x;
				animation.y = newPos.y;
				animation.z = newPos.z;
				animation.transformationID = 6;
				animationQueue.push(animation);
			}
		}
		//Para bezier
		else if (transformationQueue.front().transformationID == 7) {

			p0.x = transformationQueue.front().x;
			p0.y = transformationQueue.front().y;
			p0.z = transformationQueue.front().z;
			transformationQueue.pop();
			p1.x = transformationQueue.front().x;
			p1.y = transformationQueue.front().y;
			p1.z = transformationQueue.front().z;
			transformationQueue.pop();
			p2.x = transformationQueue.front().x;
			p2.y = transformationQueue.front().y;
			p2.z = transformationQueue.front().z;
			transformationQueue.pop();
			p3.x = transformationQueue.front().x;
			p3.y = transformationQueue.front().y;
			p3.z = transformationQueue.front().z;
			transformationQueue.pop();

			for (int i = 0; i < step; i++) {
				newPos = glm::hermite(p3, p2, p1, p0, i / step);
				newPos -= currentPos;
				currentPos += newPos;
				animation.x = newPos.x;
				animation.y = newPos.y;
				animation.z = newPos.z;
				std::cout << "Ponto " << i << " com t = " << i / step << ": (" << newPos.x << "," << newPos.y << "," << newPos.z << ")" << std::endl;
				animation.transformationID = 7;
				animationQueue.push(animation);
			}
		}
	}
	return 1;
}

int camera::animate() {

	float firstTime = glfwGetTime(); //Tempo inicial
	float currentTime;

	if (animationQueue.empty())
		return 0;

	glm::vec3 noise = NaraujoNoise();


	//Faz o noise
	for (int i = 0; i < 1; i++) {
		transformation = glm::translate(transformation, glm::vec3(noise.x, noise.y, noise.z));
	}

	flagR = 0;

	//Translada
	if (animationQueue.front().transformationID == 0 || animationQueue.front().transformationID == 5 || animationQueue.front().transformationID == 6 || animationQueue.front().transformationID == 7) {
		transformation = glm::translate(transformation, glm::vec3(animationQueue.front().x, animationQueue.front().y, animationQueue.front().z) * glm::vec3(-1));
	}

	//Rotação
	if (animationQueue.front().transformationID == 2) {
		transformation = glm::rotate(transformation, animationQueue.front().angle, glm::vec3(animationQueue.front().x, animationQueue.front().y, animationQueue.front().z));
	}

	//LookAt
	if (animationQueue.front().transformationID == 3) {
		flagR = 1;
		transformation = glm::lookAt(glm::vec3(animationQueue.front().x, animationQueue.front().y, animationQueue.front().z), glm::vec3(animationQueue.front().p1, animationQueue.front().p2, animationQueue.front().p3), glm::vec3(0, 1, 0));
	}

	//Rotação ao redor de um ponto
	if (animationQueue.front().transformationID == 4) {
		glm::vec3 movimentacao(animationQueue.front().x - ViewMatrix[3][0], animationQueue.front().y - ViewMatrix[3][1], animationQueue.front().z - ViewMatrix[3][2]);
		transformation = glm::translate(transformation, movimentacao*glm::vec3(-1));
		transformation = glm::rotate(transformation, animationQueue.front().angle, glm::vec3(0,1,0));
		transformation = glm::translate(transformation, movimentacao);
	}

	animationQueue.pop();

	return 1;
}

glm::vec3 camera::NaraujoNoise() {

	glm::vec3 noise;
	double noiseIntensity = 1.0f / 70.0f;
	noise.x = sin((float)(rand() % 360)) * noiseIntensity;
	noise.y = sin((float)(rand() % 360)) * noiseIntensity;
	noise.z = sin((float)(rand() % 360)) * noiseIntensity;
	return noise;
}