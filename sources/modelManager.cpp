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
#include <math.h>

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
#include <modelManager.hpp>

//Adiciona uma nova mesh 
void modelManager::addMesh(const char* obj) {

	mesh mesh(obj);
	meshes.push_back(mesh);
}

//Retorna o vector de meshes
std::vector<mesh>& modelManager::getMesh() {

	return meshes;
}

//Adiciona um novo model
void modelManager::addModel(GLuint programID, const char* texture, glm::vec3 posicao, int MeshID) {

	model model(programID, texture, posicao, MeshID);
	models.push_back(model);
	meshes.at(models.back().getMeshID()).bind();
}

//Retorna o vector de models
std::vector<model>& modelManager::getModel(){

	return models;
}

//Faz o desenho
void modelManager::draw(GLFWwindow* g_pWindow, GLuint LightID) {

	double currentTime = glfwGetTime();

	if (glfwGetKey(g_pWindow, GLFW_KEY_SPACE)) {
		up = true;
		right = true;
		started = true;
	}

	if (started == false) {
		if (glfwGetKey(g_pWindow, GLFW_KEY_R)) {
			resetGame();
		}
	}

	if (started && (glfwGetTime() > lastGameTime + stepTime)) {
		gameControl(g_pWindow);
		lastGameTime = glfwGetTime();
	}

	/*if (transformando == 0 && transformandoCamera == 0) {
		if (currentTime - initialTime >= 0.01) {
			if (glfwGetKey(g_pWindow, GLFW_KEY_D)) {
				transformation.x = translacao.x;
				transformation.y = translacao.y;
				transformation.z = translacao.z;
				transformation.transformationID = 0;
				models.at(modelN).addTransformation(transformation);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_E)) {
				transformation.x = escala.x;
				transformation.y = escala.y;
				transformation.z = escala.z;
				transformation.transformationID = 1;
				models.at(modelN).addTransformation(transformation);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_R)) {
				transformation.x = rotacao.x;
				transformation.y = rotacao.y;
				transformation.z = rotacao.z;
				transformation.angle = angulo;
				transformation.transformationID = 2;
				models.at(modelN).addTransformation(transformation);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_X)) {
				transformation.x = shearX.x;
				transformation.y = shearX.y;
				transformation.z = shearX.z;
				transformation.transformationID = 3;
				models.at(modelN).addTransformation(transformation);
			}

			if (glfwGetKey(g_pWindow, GLFW_KEY_Y)) {
				transformation.x = shearY.x;
				transformation.y = shearY.y;
				transformation.z = shearY.z;
				transformation.transformationID = 4;
				models.at(modelN).addTransformation(transformation);
			}

			if (glfwGetKey(g_pWindow, GLFW_KEY_Z)) {
				transformation.x = shearZ.x;
				transformation.y = shearZ.y;
				transformation.z = shearZ.z;
				transformation.transformationID = 5;
				models.at(modelN).addTransformation(transformation);
			}

			if (glfwGetKey(g_pWindow, GLFW_KEY_C)) {
				transformation.x = translacao.x;
				transformation.y = translacao.y;
				transformation.z = translacao.z;
				transformation.transformationID = 6;
				models.at(modelN).addTransformation(transformation);
				transformation.x = rotacao.x;
				transformation.y = rotacao.y;
				transformation.z = rotacao.z;
				transformation.angle = angulo;
				transformation.transformationID = 6;
				models.at(modelN).addTransformation(transformation);
			}

			if (glfwGetKey(g_pWindow, GLFW_KEY_O)) {
				transformation.x = projecao.x;
				transformation.y = projecao.y;
				transformation.z = projecao.z;
				transformation.transformationID = 7;
				models.at(modelN).addTransformation(transformation);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_K)) {
				criaCamera();
				std::cout << "Câmera criada\n";
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_H)) {
				transformationC.x = translacao.x;
				transformationC.y = translacao.y;
				transformationC.z = translacao.z;
				transformationC.transformationID = 0;
				cameras.at(cameraAtual).addTransformation(transformationC);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_J)) {
				transformationC.x = rotacao.x;
				transformationC.y = rotacao.y;
				transformationC.z = rotacao.z;
				transformationC.angle = angulo;
				transformationC.transformationID = 2;
				cameras.at(cameraAtual).addTransformation(transformationC);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_F)) {
				transformationC.p1 = lookAt.x;
				transformationC.p2 = lookAt.y;
				transformationC.p3 = lookAt.z;
				transformationC.x = eye.x;
				transformationC.y = eye.y;
				transformationC.z = eye.z;
				transformationC.transformationID = 3;
				cameras.at(cameraAtual).addTransformation(transformationC);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_M)) {
				transformationC.x = rotacao.x;
				transformationC.y = rotacao.y;
				transformationC.z = rotacao.z;
				transformationC.angle = angulo;
				transformationC.transformationID = 4;
				cameras.at(cameraAtual).addTransformation(transformationC);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_P)) {
				transformationC.x = pontos.x;
				transformationC.y = pontos.y;
				transformationC.z = pontos.z;
				transformationC.transformationID = 5;
				cameras.at(cameraAtual).addTransformation(transformationC);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_L)) {
				transformationC.x = pontos.x;
				transformationC.y = pontos.y;
				transformationC.z = pontos.z;
				transformationC.transformationID = 6;
				cameras.at(cameraAtual).addTransformation(transformationC);
			}
			if (glfwGetKey(g_pWindow, GLFW_KEY_A)) {
				transformationC.x = pontos.x;
				transformationC.y = pontos.y;
				transformationC.z = pontos.z;
				transformationC.transformationID = 7;
				cameras.at(cameraAtual).addTransformation(transformationC);
			}

			initialTime += 1.0;
		}

		
		
		if (glfwGetKey(g_pWindow, GLFW_KEY_G)) {
			for (int i = 0; i < cameras.size(); i++) {
				if (cameras.at(i).setTransformation(seconds))
				{
					transformandoCamera += 1;
				}
			}
		}
	}*/

	seconds = 1;

	//Aplica a transformação  
	for (int i = 0; i < models.size(); i++) {
		if (models.at(i).setTransformation(seconds))
		{
			//std::cout << "set transformation" << std::endl;
			transformando += 1;
		}
	}

	//Executa a transformação
	
	int flag = 0 , step = seconds * ANIMSTEP;
	//std::cout << transformando << " | " << currentTime - animationTime << " >= " << seconds / step << " | " << transformation.transformationID << std::endl;
	if (transformando != 0 && ((currentTime - animationTime) >= seconds/step) && transformation.transformationID != 6) {
		//std::cout << "entrou na execução" << std::endl;
		for (int i = 0; i < models.size(); i++) {
			if (!models.at(i).animate()) {
				flag += 1;
			}
		}
		if (flag == models.size())
			transformando = 0;
		animationTime = currentTime;
	}
	else if (transformando != 0 && ((currentTime - animationTime) >= seconds / (step*2)) && transformation.transformationID == 6) {
		for (int i = 0; i < models.size(); i++) {
			if (!models.at(i).animate()) {
				flag += 1;
			}
		}
		if (flag == models.size())
			transformando = 0;
		animationTime = currentTime;
	}

	if (transformandoCamera != 0 && ((currentTime - animationTime) >= seconds / step) && transformation.transformationID != 6) {
		for (int i = 0; i < cameras.size(); i++) {
			if (!cameras.at(i).animate()) {
				flag += 1;
			}
		}
		if (flag == cameras.size()) 
			transformandoCamera = 0;
		animationTime = currentTime;
		
	}
	
	//Dá bind em todos os models
	for (int i = 0; i < models.size(); i++) {


		models.at(i).bind(LightID, cameras.at(cameraAtual));

		//Uniform color
		GLint fragmentColorLocation = glGetUniformLocation(programID, "lightColor");
		glUniform3f(fragmentColorLocation, models.at(i).corR, models.at(i).corG, models.at(i).corB);

		//Uniform roughness
		GLint fragmentRoughnessLocation = glGetUniformLocation(programID, "roughnessValue");
		glUniform1f(fragmentRoughnessLocation, models.at(i).roughness);

		//Uniform Fresnel
		GLint fragmentFresnelLocation = glGetUniformLocation(programID, "F0");
		glUniform1f(fragmentFresnelLocation, models.at(i).fresnel);

		//Uniform diffuse
		GLint fragmentDiffuseLocation = glGetUniformLocation(programID, "k");
		glUniform1f(fragmentDiffuseLocation, models.at(i).diffuse);

		if(transformando == 1)
			meshes.at(models.at(i).getMeshID()).bind();

		meshes.at(models.at(i).getMeshID()).load();

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,        // mode
			meshes.at(models.at(i).getMeshID()).getIndices().size(),      // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0             // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	
	// Draw tweak bars
	TwDraw();

	// Swap buffers
	glfwSwapBuffers(g_pWindow);
	glfwPollEvents();

	lastTime = glfwGetTime();
	drawns = 1;
}

void modelManager::criaCamera() {

	camera cameraNova(programID);
	cameraNova.transformation = glm::translate(glm::mat4(1.0f), glm::vec3(translacao.x, translacao.y, translacao.z) * glm::vec3(-1));
	if (angulo != 0) {
		cameraNova.transformation = glm::rotate(cameraNova.transformation, angulo, glm::vec3(rotacao.x, rotacao.y, rotacao.z));
		cameraNova.flagR = 1;
	}
	cameras.push_back(cameraNova);

	
}

void modelManager::gameControl(GLFWwindow* g_pWindow) {

	if (started) {
		//std::cout << "Side Border: " << sideBorder << "Up Border: " << upBorder << std::endl;
		if (glfwGetKey(g_pWindow, GLFW_KEY_LEFT)) {
			if (limite <= 12) {
				glm::vec3 moviment(-blockMoviment, 0.0f, 0.0f);
				limite += blockMoviment;
				models.at(1).arrowMoviment(moviment);
			}
		}

		if (glfwGetKey(g_pWindow, GLFW_KEY_RIGHT)) {
			if (limite >= -12) {
				glm::vec3 moviment(blockMoviment, 0.0f, 0.0f);
				limite -= blockMoviment;
				models.at(1).arrowMoviment(moviment);
			}
		}

		//Caso base, em que a bola está indo para cima, e não está em nenhuma borda
		if (sideBorder <= 13.0 && sideBorder >= -13.0 && upBorder <= 15.0 && upBorder >= -3.6 && up) {
			//std::cout << "Primeiro caso - up: " << upBorder << std::endl;
			//std::cout << "Primeiro caso - side: " << sideBorder << std::endl;
			upBorder += yMoviment;
			if (right) {
				sideBorder += xMoviment;
				models.at(0).ballMoviment(xMoviment, yMoviment);
			}
			else {
				sideBorder -= xMoviment;
				models.at(0).ballMoviment(-xMoviment, yMoviment);
			}
		}
		//Caso em que a bola está indo para baixo, entra também quando detecta que a bola está na borda de cima
		else if ((upBorder >= 15.0 || down) && upBorder >= -3.5 && sideBorder <= 13.0 && sideBorder >= -13.0) {
			//std::cout << "Segundo caso - up: " << upBorder << std::endl;
			//std::cout << "Segundo caso - side: " << sideBorder << std::endl;
			up = false;
			down = true;
			upBorder -= yMoviment;
			if (right) {
				sideBorder += xMoviment;
				models.at(0).ballMoviment(xMoviment, -yMoviment);
			}
			else {
				sideBorder -= xMoviment;
				models.at(0).ballMoviment(-xMoviment, -yMoviment);
			}
		}
		//Caso em que a bola chegou na borda de baixo, perdeu
		else if (upBorder <= 15.0 && upBorder >= -3.9 && sideBorder <= 13.0 && sideBorder >= -13.0) {
			//std::cout << "Terceiro caso - up: " << upBorder << std::endl;
			//std::cout << "Terceiro caso - side: " << sideBorder << std::endl;
			up = true;
			down = false;
			lives--;
			if (lives < 0) {
				started = false;
				std::cout << "\nVoce perdeu o jogo! Aperte R para reiniciar" << std::endl;
			}
			else {
				//std::cout << "Perdeu vida\n";
				glm::mat4 inicialPosition = glm::translate(inicialPosition, 0.0f, -6.0f, 0.0f);
				models.at(0).setModelMatrix(models.at(0).firstMatrix);
				models.at(0).setTransformation(models.at(0).firstTransformation);
				sideBorder = 0;
				upBorder = 0;
				up = true;
				down = false;
				left = false;
				right = true;
				return;
			}
		}
		//Caso em que a bola chegou na borda direita
		else if (sideBorder <= 13.3 && sideBorder >= -13.3 && right) {
			//std::cout << "Quarto caso - up: " << upBorder << std::endl;
			//std::cout << "Quarto caso - side: " << sideBorder << std::endl;
			right = false;
			left = true;
			sideBorder -= xMoviment;
			if (up) {
				upBorder += yMoviment;
				models.at(0).ballMoviment(-xMoviment, yMoviment);
			}
			else {
				upBorder -= yMoviment;
				models.at(0).ballMoviment(-xMoviment, -yMoviment);
			}
		}
		//Caso em que a bola chegou na borda esquerda
		else if (sideBorder <= 13.0 && sideBorder <= -13.0 && left) {
			//std::cout << "Quinto caso - up: " << upBorder << std::endl;
			//std::cout << "Quinto caso - side: " << sideBorder << std::endl;
			right = true;
			left = false;
			sideBorder += xMoviment;
			if (up) {
				upBorder += yMoviment;
				models.at(0).ballMoviment(xMoviment, yMoviment);
			}
			else {
				upBorder -= yMoviment;
				models.at(0).ballMoviment(xMoviment, -yMoviment);
			}
		}
		//std::cout << "Fora do caso - side: " << sideBorder << "up: " << upBorder << std::endl;

	}

	if (naraujoCheckCollision(models.at(0).getPosition(), meshes.at(0).getSize(), models.at(1).getPosition(), meshes.at(1).getSize())) {
		//std::cout << "Colisão!" << std::endl;
		models.at(0).ballMoviment(xMoviment, yMoviment);
		up = true;
		down = false;
		upBorder += yMoviment;
		sideBorder += xMoviment;
	}

	int i = 2;
	if (models.size() > 2) {
		for (auto it = models.begin() + 2; it != models.end(); ++it) {
			if (models.at(i).destroyed == 0 && naraujoCheckCollision(models.at(0).getPosition(), meshes.at(0).getSize(), models.at(i).getPosition(), meshes.at(i).getSize())) {
				//std::cout << "Colisão 2" << std::endl;
				transformation.x = 0.0;
				transformation.y = 0.0;
				transformation.z = 0.0;
				transformation.transformationID = 1;
				models.at(i).addTransformation(transformation);
				models.at(i).destroyed = 1;
				points++; //Atualiza pontos
				if (xMoviment > 0) {
					sideBorder -= xMoviment;
					if (yMoviment > 0) {
						models.at(0).ballMoviment(-xMoviment, -yMoviment);
						upBorder -= yMoviment;
						up = false;
						down = true;
						left = true;
						right = false;
					}
					else {
						models.at(0).ballMoviment(-xMoviment, yMoviment);
						upBorder += yMoviment;
						up = true;
						down = false;
						left = true;
						right = false;
					}
				}
				else {
					sideBorder += xMoviment;
					if (yMoviment > 0) {
						models.at(0).ballMoviment(xMoviment, -yMoviment);
						upBorder -= yMoviment;
						up = false;
						down = true;
						left = false;
						right = true;
					}
					else {
						models.at(0).ballMoviment(xMoviment, yMoviment);
						upBorder += yMoviment;
						up = true;
						down = false;
						left = false;
						right = true;
					}
				}
			}
			i++;
			//models.erase(models.begin() +2);
		}
	}
	if (points == models.size() - 2) {
		started = false; //Ganhou o jogo
		std::cout << "\nParabens, voce venceu o jogo! Aperte R para reiniciar \n";
	}
}

/* -- Colisões -- */
//Verifica colisão entre dois modelos
GLboolean modelManager::naraujoCheckCollision(glm::vec3 positionA, glm::vec3 sizeA, glm::vec3 positionB, glm::vec3 sizeB)
{

	bool collisionX = false;
	bool collisionY = false;
	bool collisionZ = false;

	if ((positionA.x + (sizeA.x / 2)) >= positionB.x && (((positionB.x + (sizeB.x / 2)) >= positionA.x)))
	{
		collisionX = true;
	}
	if ((positionA.y + (sizeA.y / 2)) >= positionB.y && (((positionB.y + (sizeB.y / 2)) >= positionA.y)))
	{
		collisionY = true;
	}
	if ((positionA.z + (sizeA.z / 2)) >= positionB.z && (((positionB.z + (sizeB.z / 2)) >= positionA.z)))
	{
		collisionZ = true;
	}

	//std::cout <<std::endl<< "Model A position: (" << positionA.x << ", " << positionA.y << ", " << positionA.z << ")";
	//std::cout << " | Model B posiition (" << positionB.x << ", " << positionB.y << ", " << positionB.z << ")" << std::endl;

	//std::cout << "Collision X = " << collisionX << " | Collision Y = " << collisionY << std::endl;
	return collisionX && collisionY && collisionZ;
}

void modelManager::resetGame() {

	models.clear();
	points = 0;
	lives = 2;
	upBorder = 0;
	sideBorder = 0;
	limite = 0;

	//Adiciona bola
	addMesh("mesh/sphere.obj");
	glm::vec3 posicao(0.0f, -6.0f, 0.0f);
	addModel(programID, "mesh/uvmap.DDS", posicao, 0);
	getModel().at(0).setShaderParameters(0.9, 0.1, 0.1, 0.8, 0.8, 0.2);

	//Adiciona cubo
	addMesh("mesh/cube.obj");
	posicao.y = -8.0f;
	addModel(programID, "mesh/goose.dds", posicao, 1);
	getModel().at(1).setShaderParameters(0.5, 0.3, 0.8, 0.5, 0.6, 0.5);

	for(int i = 0; i < 6; i++)
		level[i] = false ;

	addBricks();
}

void modelManager::addBricks() {
	glm::vec3 posicao;
	if (dificuldade > 0 && level[0] == false) {
		//std::cout << "Dificuldade\n";
		//Adiciona cubo
		addMesh("mesh/cube.obj");
		posicao.y = -1.0f;
		addModel(programID, "mesh/goose.dds", posicao, 1);
		getModel().at(2).setShaderParameters(0.1, 0.1, 0.4, 0.5, 0.6, 0.5);
		level[0] = true;
	}
	if (dificuldade > 1 && level[0] == true && level[1] == false) {
		posicao.x = -2.0f;
		for (int i = 3; i < 6; i++) {
			//std::cout << "Dificuldade\n";
			//Adiciona cubo
			addMesh("mesh/cube.obj");
			posicao.y = 1.0f;
			posicao.x += 1.0f;
			addModel(programID, "mesh/goose.dds", posicao, 1);
			if (i % 2 == 0)
				getModel().at(i).setShaderParameters(0.3, 0.1, 0.4, 0.5, 0.6, 0.5);
			else
				getModel().at(i).setShaderParameters(0.4, 0.1, 0.3, 0.5, 0.6, 0.5);
			level[1] = true;
		}
	}
	if (dificuldade > 2 && level[0] == true && level[1] == true && level[2] == false) {
		posicao.x = -4.0f;
		for (int i = 6; i < 13; i++) {
			//std::cout << "Dificuldade\n";
			//Adiciona cubo
			addMesh("mesh/cube.obj");
			posicao.y = 3.0f;
			posicao.x += 1.0f;
			addModel(programID, "mesh/goose.dds", posicao, 1);
			if (i % 2 == 0)
				getModel().at(i).setShaderParameters(0.5, 0.2, 0.4, 0.5, 0.6, 0.5);
			else
				getModel().at(i).setShaderParameters(0.4, 0.2, 0.5, 0.5, 0.6, 0.5);
			level[2] = true;
		}
	}
	if (dificuldade > 3 && level[0] == true && level[1] == true && level[2] == true && level[3] == false) {
		posicao.x = -7.0f;
		for (int i = 13; i < 26; i++) {
			//std::cout << "Dificuldade\n";
			//Adiciona cubo
			addMesh("mesh/cube.obj");
			posicao.y = 5.0f;
			posicao.x += 1.0f;
			addModel(programID, "mesh/goose.dds", posicao, 1);
			if (i % 2 == 0)
				getModel().at(i).setShaderParameters(0.7, 0.2, 0.2, 0.5, 0.6, 0.5);
			else
				getModel().at(i).setShaderParameters(0.2, 0.2, 0.7, 0.5, 0.6, 0.5);
			level[3] = true;
		}
	}
	if (dificuldade > 4 && level[0] == true && level[1] == true && level[2] == true && level[3] == true && level[4] == false) {
		posicao.x = -11.0f;
		for (int i = 26; i < 47; i++) {
			//std::cout << "Dificuldade\n";
			//Adiciona cubo
			addMesh("mesh/cube.obj");
			posicao.y = 7.0f;
			posicao.x += 1.0f;
			addModel(programID, "mesh/goose.dds", posicao, 1);
			if (i % 2 == 0)
				getModel().at(i).setShaderParameters(0.8, 0.1, 0.1, 0.5, 0.6, 0.5);
			else
				getModel().at(i).setShaderParameters(0.1, 0.1, 0.8, 0.5, 0.6, 0.5);
			level[4] = true;
		}
	}
	if (dificuldade > 5 && level[0] == true && level[1] == true && level[2] == true && level[3] == true && level[4] == true && level[5] == false) {
		posicao.x = -13.0f;
		for (int i = 47; i < 73; i++) {
			//std::cout << "Dificuldade\n";
			//Adiciona cubo
			addMesh("mesh/cube.obj");
			posicao.y = 9.0f;
			posicao.x += 1.0f;
			addModel(programID, "mesh/goose.dds", posicao, 1);
			if (i % 2 == 0)
				getModel().at(i).setShaderParameters(1.0, 0.0, 0.0, 0.5, 0.6, 0.5);
			else
				getModel().at(i).setShaderParameters(0.0, 0.0, 1.0, 0.5, 0.6, 0.5);
			level[5] = true;
		}
	}
}