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
#include <model.hpp>

void model::bind(GLuint LightID, camera cameraAtual) {

	cameraAtual.bind();
	ModelMatrix = glm::mat4(1.0) * transformation;

	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));

	MVP = cameraAtual.ProjectionMatrix * cameraAtual.ViewMatrix * ModelMatrix;
	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(cameraAtual.ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(cameraAtual.ViewMatrixID, 1, GL_FALSE, &cameraAtual.ViewMatrix[0][0]);

	glm::vec3 lightPos = glm::vec3(4, 4, 4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	position = glm::vec3(ModelMatrix[3][0], ModelMatrix[3][1], ModelMatrix[3][2]);
	//std::cout << position.x << " | " << position.y << " | " << position.z << std::endl;
}

//Faz a limpeza final
void model::clean() {
	glDeleteTextures(1, &Texture);
}

//Retorna a Mesh ao qual este model pertence
int model::getMeshID() {
	return MeshID;
}

//Adiciona transformação na fila
void model::addTransformation(struct transform transformation) {

	struct transform trans;


	trans.x = transformation.x;
	trans.y = transformation.y;
	trans.z = transformation.z;
	trans.angle = transformation.angle;
	trans.transformationID = transformation.transformationID;
	transformationQueue.push(trans);
	std::cout << "\nAdicionado na fila: " << transformation.x << " | " << transformation.y << " | " << transformation.z << "\n";
}

//Realiza translação
int model::setTransformation(float seconds) {

	if (transformationQueue.empty())
		return 0;

	int step = seconds * ANIMSTEP;
	if (step < 1)
		step = 1;

	while (transformationQueue.size() != 0) {

		//Fatia a transformação
		struct transform animation;

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

		//Para Escala
		else if (transformationQueue.front().transformationID == 1) {
			double raiz = (1.0 / (double)step);
			animation.x = pow(transformationQueue.front().x, raiz) / 2 + 0.5;
			animation.y = pow(transformationQueue.front().y, raiz) / 2 + 0.5;
			animation.z = pow(transformationQueue.front().z, raiz) / 2 + 0.5;
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

		//Para shear
		else if (transformationQueue.front().transformationID == 3 || transformationQueue.front().transformationID == 4 || transformationQueue.front().transformationID == 5) {
			animation.x = transformationQueue.front().x;
			animation.y = transformationQueue.front().y;
			animation.z = transformationQueue.front().z;
			animation.transformationID = transformationQueue.front().transformationID;
			transformationQueue.pop();

			//Coloca a transformação fatiada numa fila
			for (int i = 0; i < 1; i++)
				animationQueue.push(animation);
		}

		//Para composta
		else if (transformationQueue.front().transformationID == 6) {
			struct transform aux;
			animation.x = transformationQueue.front().x / step;
			animation.y = transformationQueue.front().y / step;
			animation.z = transformationQueue.front().z / step;
			animation.transformationID = 0;                 //Translada primeiro
			transformationQueue.pop();
			aux.x = transformationQueue.front().x;
			aux.y = transformationQueue.front().y;
			aux.z = transformationQueue.front().z;
			aux.angle = (transformationQueue.front().angle / step);
			aux.transformationID = 2;                //Depois rotaciona;
			transformationQueue.pop();

			for (int i = 0; i < step; i++) {
				animationQueue.push(animation);
				animationQueue.push(aux);
			}
		}

		//Para projeção
		else if (transformationQueue.front().transformationID == 7) {
			animation.x = transformationQueue.front().x;
			animation.y = transformationQueue.front().y;
			animation.z = transformationQueue.front().z;
			animation.transformationID = transformationQueue.front().transformationID;
			transformationQueue.pop();

			//Coloca a transformação fatiada numa fila
			for (int i = 0; i < 1; i++)
				animationQueue.push(animation);
		}

	}
	return 1;
}

int model::animate() {

	float firstTime = glfwGetTime(); //Tempo inicial
	float currentTime;

	if (animationQueue.empty())
		return 0;

	//Translada
	if (animationQueue.front().transformationID == 0)
		transformation = glm::translate(transformation, glm::vec3(animationQueue.front().x, animationQueue.front().y, animationQueue.front().z));

	//Escala
	if (animationQueue.front().transformationID == 1)
		transformation = glm::scale(transformation, glm::vec3(animationQueue.front().x, animationQueue.front().y, animationQueue.front().z));

	//Rotação
	if (animationQueue.front().transformationID == 2)
		transformation = glm::rotate(transformation, animationQueue.front().angle, glm::vec3(animationQueue.front().x, animationQueue.front().y, animationQueue.front().z));

	//Shear X
	if (animationQueue.front().transformationID == 3) {
		transformation[1][0] = animationQueue.front().y;
		transformation[2][0] = animationQueue.front().z;
		transformation = glm::shearX3D(transformation, transformation[1][0], transformation[2][0]);
	}

	//Shear Y
	if (animationQueue.front().transformationID == 4) {
		transformation[0][1] = animationQueue.front().x;
		transformation[2][0] = animationQueue.front().z;
		transformation = glm::shearY3D(transformation, transformation[0][1], transformation[2][0]);
	}
		
	//Shear Z
	if (animationQueue.front().transformationID == 5) {
		transformation[0][2] = animationQueue.front().x;
		transformation[1][2] = animationQueue.front().y;
		transformation = glm::shearZ3D(transformation, transformation[0][2], transformation[1][2]);
	}

	//Projeção
	if (animationQueue.front().transformationID == 7) {
		transformation = glm::proj3D(transformation, glm::vec3(animationQueue.front().x, animationQueue.front().y, animationQueue.front().z));
	}

	animationQueue.pop();

		
	return 1;
}

void model::arrowMoviment(glm::vec3 moviment) {
	transformation = glm::translate(transformation, glm::vec3(moviment.x, moviment.y, moviment.z));
}

void model::setShaderParameters(float R, float G, float B, float Roughness, float Fresnel, float Diffuse) {
	corR = R;
	corG = G;
	corB = B;
	roughness = Roughness;
	fresnel = Fresnel;
	diffuse = Diffuse;
}

void model::ballSize(glm::vec3 size) {
	transformation = glm::scale(size);
}

void model::ballMoviment(float xMoviment, float yMoviment) {
	transformation = glm::translate(transformation, glm::vec3(xMoviment, yMoviment, 0.0f));
}

glm::vec3 model::getPosition()
{
	return position;
}

glm::mat4 model::getModelMatrix() {
	return ModelMatrix;
}

void model::setModelMatrix(glm::mat4 modelMatrix) {
	ModelMatrix = modelMatrix;
}