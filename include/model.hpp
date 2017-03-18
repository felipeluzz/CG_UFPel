#ifndef MODEL_H
#define MODEL_H
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

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>
#include <meshSimplification.hpp>
#include <Camera.hpp>

struct transform{

	double x;
	double y;
	double z;
	float angle;
	// 0: Translação  |  1: Escala  | 2: Rotação  |  3: Shear X   |  4: Shear Y  |  5: Shear Z  | 6: Composta  |  7: Projeção
	unsigned short transformationID;
};

class model {

	int MeshID = 0;
	GLuint MatrixID;
	GLuint Texture;
	GLuint TextureID;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP;
	glm::mat4 transformation;
	std::queue <struct transform> transformationQueue;
	std::queue <struct transform> animationQueue;
	glm::vec3 position;

public:
	float corR;
	float corG;
	float corB;
	float roughness;
	float fresnel;
	float diffuse;
	int destroyed = 0;
	int flag = 0;
	glm::mat4 firstMatrix;
	glm::mat4 firstTransformation;
	
public: 
	model(GLuint programID, const char* textura, glm::vec3 posicao, int mID) {
		MeshID = mID;
		MatrixID = glGetUniformLocation(programID, "MVP");
		Texture = loadDDS(textura);
		TextureID = glGetUniformLocation(programID, "myTextureSampler");
		transformation = glm::translate(transformation, glm::vec3(posicao.x, posicao.y, posicao.z));
	}

public:
	void bind(GLuint LightID, class camera cameraAtual);
	void clean();
	int setTransformation(float seconds);
	void addTransformation(struct transform);
	int animate();
	int getMeshID();
	void arrowMoviment(glm::vec3 transformation);
	void setShaderParameters(float R, float G, float B, float Roughness, float Fresnel, float Diffuse);
	void ballSize(glm::vec3 size);
	void ballMoviment(float xMoviment, float yMoviment);
	glm::vec3 getPosition();
	glm::mat4 getModelMatrix();
	void setModelMatrix(glm::mat4 modelMatrix);
	void setTransformation(glm::mat4 Transformation);
};


#endif
