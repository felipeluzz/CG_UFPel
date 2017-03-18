#ifndef MODELMANAGER_H
#define MODELMANAGER_H

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

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>
#include <meshSimplification.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <Camera.hpp>

class modelManager {

	std::vector<mesh> meshes;
	std::vector<model> models;
	double initialTime = glfwGetTime();
	double lastTime = 0;
	double animationTime = glfwGetTime();
	int transformando = 0, drawns = 1, cameraAtual = 0, transformandoCamera = 0;
	TwBar *g_pToolBar;
	glm::vec3 translacao;
	glm::vec3 escala;
	glm::vec3 rotacao;
	glm::vec3 shearX;
	glm::vec3 shearY;
	glm::vec3 shearZ;
	glm::vec3 projecao;
	glm::vec3 lookAt;
	glm::vec3 eye;
	glm::vec3 pontos;
	int modelN = 0;
	float angulo = 0, seconds = 0;
	struct transform transformation;
	struct transformC transformationC;
	std::vector<camera> cameras;
	GLuint programID;
	float limite = 0.0;
	bool started = false;
	float sideBorder = 0.0;
	float upBorder = 0.0;
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	float xMoviment = 0.03;
	float yMoviment = 0.1;

public:

	modelManager(TwBar *ToolBar, GLuint programID) : programID(programID) {
		g_pToolBar = ToolBar;
		////Adciona "Camera" a barra
		//TwAddVarRW(g_pToolBar, "Camera", TW_TYPE_INT16, &cameraAtual, " label='Câmera Atual'");
		////Adiciona "Translação" a barra
		//TwAddVarRW(g_pToolBar, "Translação", TW_TYPE_DIR3F, &translacao, " label='Translação'");
		////Adiciona "LookAt" a barra
		//TwAddVarRW(g_pToolBar, "LookAt", TW_TYPE_DIR3F, &eye, " label='Look At - Posição'");
		////Adiciona "LookAt" a barra
		//TwAddVarRW(g_pToolBar, "LookAtC", TW_TYPE_DIR3F, &lookAt, " label='Look At - Centro'");
		////Adciona "Rotação" a barra
		//TwAddVarRW(g_pToolBar, "Rotação", TW_TYPE_DIR3F, &rotacao, " label='Rotação'");
		//TwAddVarRW(g_pToolBar, "Angulo", TW_TYPE_FLOAT, &angulo, " label='Angulo Rotação'");
		////Adiciona "Pontos de curva" a barra
		//TwAddVarRW(g_pToolBar, "curva", TW_TYPE_DIR3F, &pontos, " label='Pontos de Curva'");
		////Adiciona "Tempo de transformação a barra
		//TwAddVarRW(g_pToolBar, "Tempo", TW_TYPE_FLOAT, &seconds, " label='Tempo de Transformação' min = 0.0");
		////Adiciona "Escala" a barra
		//TwAddVarRW(g_pToolBar, "Escala", TW_TYPE_DIR3F, &escala, " label='Escala'");
		////Adiciona "Shear X" a barra
		//TwAddVarRW(g_pToolBar, "ShearX", TW_TYPE_DIR3F, &shearX, " label='Shear X'");
		////Adiciona "Shear Y" a barra
		//TwAddVarRW(g_pToolBar, "ShearY", TW_TYPE_DIR3F, &shearY, " label='Shear Y'");
		////Adiciona "Shear Z" a barra
		//TwAddVarRW(g_pToolBar, "ShearZ", TW_TYPE_DIR3F, &shearZ, " label='Shear Z'");
		////Adiciona "Projeção" a barra
		//TwAddVarRW(g_pToolBar, "Projecao", TW_TYPE_DIR3F, &projecao, " label='Projecao'");
		////Adiciona "Modelo Atual" a barra
		//TwAddVarRW(g_pToolBar, "Modelo", TW_TYPE_INT8, &modelN, " label='Modelo Atual' min = 0.0");
		camera camera1(programID);
		cameras.push_back(camera1);

	}
	void addMesh(const char* obj);
	void addModel(GLuint programID, const char* texture, glm::vec3 posicao, int MeshID);
	void draw(GLFWwindow* g_pWindow, GLuint LightID);
	std::vector<mesh>& getMesh();
	std::vector<model>& getModel();
	void criaCamera();
	void gameControl(GLFWwindow* g_pWindow);
	GLboolean naraujoCheckCollision(glm::vec3 positionA, glm::vec3 sizeA, glm::vec3 positionB, glm::vec3 sizeB);
};

#endif
