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

//Inclui classes novas
#include <mesh.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* g_pWindow;
unsigned int g_nWidth = 1024, g_nHeight = 768;

// Include AntTweakBar
#include <AntTweakBar.h>
TwBar *g_pToolBar;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>
#include <meshSimplification.hpp>
#include <modelManager.hpp>


void WindowSizeCallBack(GLFWwindow *pWindow, int nWidth, int nHeight) {

	g_nWidth = nWidth;
	g_nHeight = nHeight;
	glViewport(0, 0, g_nWidth, g_nHeight);
	TwWindowSize(g_nWidth, g_nHeight);
}

int main(void)
{
	int nUseMouse = 0;
	

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	g_pWindow = glfwCreateWindow(g_nWidth, g_nHeight, "CG UFPel", NULL, NULL);
	if (g_pWindow == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(g_pWindow);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	check_gl_error();//OpenGL error from GLEW

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(g_nWidth, g_nHeight);

	// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	glfwSetMouseButtonCallback(g_pWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetCursorPosCallback(g_pWindow, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetScrollCallback(g_pWindow, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetKeyCallback(g_pWindow, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
	glfwSetCharCallback(g_pWindow, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar
	glfwSetWindowSizeCallback(g_pWindow, WindowSizeCallBack);

	//create the toolbar
	g_pToolBar = TwNewBar("CG UFPel ToolBar");


	//Adiciona novo model
	//glm::vec3 posicaoModel;
	//TwAddVarRW(g_pToolBar, "Model", TW_TYPE_DIR3F, &posicaoModel, "label = 'Novo Model'");
	//int meshID = 0;
	//TwAddVarRW(g_pToolBar, "MeshID", TW_TYPE_INT8, &meshID, "label = 'MehsID'");

	////Adiciona "cor" a barra
	//float corR = 0.9;
	//TwAddVarRW(g_pToolBar, "CorR", TW_TYPE_FLOAT, &corR, "min=0 max=1 step=0.1 label = 'Cor R'");
	//float corG = 0.1;
	//TwAddVarRW(g_pToolBar, "CorG", TW_TYPE_FLOAT, &corG, "min=0 max=1 step=0.1 label = 'Cor G'");
	//float corB = 0.1;
	//TwAddVarRW(g_pToolBar, "CorB", TW_TYPE_FLOAT, &corB, "min=0 max=1 step=0.1 label = 'Cor B'");

	////Adicona "roughness" a barra
	//float roughness = 0.3;
	//TwAddVarRW(g_pToolBar, "roughness", TW_TYPE_FLOAT, &roughness, "min=0.1 max=1 step=0.1 label = 'Roughness value'");

	////Adiciona "Fresnel" a barra
	//float fresnel = 0.8;
	//TwAddVarRW(g_pToolBar, "fresnel", TW_TYPE_FLOAT, &fresnel, "min=0.1 max=1 step=0.1 label = 'Fresnel reflectance'");

	////Adiciona "Diffuse" a barra
	//float diffuse = 0.2;
	//TwAddVarRW(g_pToolBar, "diffuse", TW_TYPE_FLOAT, &diffuse, "min=0.1 max=1 step=0.1 label = 'Diffuse reflection'");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(g_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(g_pWindow, g_nWidth / 2, g_nHeight / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/CookTorrance.fragmentshader");

	std::vector<std::pair<int, int>> contagemVizinhos;  //Vector onde fica a contagem dos vizinhos
	std::vector<std::set<unsigned short> > vizinhos; //Vector que contém os vizinhos dos vértices
	std::stack <std::vector<glm::vec3> > verticesRemovidos; //Vetor que armazena os vértices removidos
	std::stack <std::vector<unsigned short> > indicesAntigos; //Vetor que armazena os índices antigos

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames    = 0;

	int simplificar=0, desfazer = 0, stepS = 0, stepD = 0, MeshN = 0; //Flags para controle das teclas

	/*std::cout << "Pressione S para simplificar a malha; \n D para desfazer a simplificação; \n Q para dar 1 passo de simplificação; \n W para desfazer 1 passo de simplificação; \n Espaço para pausar o que estiver fazendo; \n D para Translação \n E para Escala \n R para rotação \n X para ShearX, Y para ShearY, Z para ShearZ \n C para Composta \n O para projeção \n T para ativar a transformação \n B para adicionar uma nova mesh \n N para adicionar um novo model \n";
	std::cout << "\n------Câmera------\n";
	std::cout << "\nColoque a posição de uma nova câmera na barra Translação e enão aperte K para criá-la";
	std::cout << "\nColoque os valores da translação na barra e o tempo de transformação e pressione H para coloca-la na fila";
	std::cout << "\nColoque o eixo de rotação desejado na barra re rotação, o ângulo e o tempo da rotação e pressione J para colocá-la na fila";
	std::cout << "\nColoque a posição do lookAt na barra, o centro da camera e o tempo de transformação e pression F para coloca-la na fila";
	std::cout << "\nColoque o ponto que deseja rotacionar em volta na barra Rotação, o angulo e o tempo da transformação e pressione M para coloca-la na fila";
	std::cout << "\nColoque um ponto da curva linear na barra Ponto e o tempo (total) e pressione P para adiciona-lo na fila, insira quantos pontos quiser";
	std::cout << "\nColoque o p0 da B-Spline na barra Ponto e o tempo (total) e pressione L para adiciona-lo a fila, repita até p3 (não mais do que 4 pontos)";
	std::cout << "\nColoque o p0 da Bézier na barra Ponto e o tempo (total) e pressione A para adicioná-lo na fila, repita até p3 (não mais do que 4 pontos)";
	std::cout << "\nPressione G para executar as transformações contidas na fila";
	std::cout << "\nOBS: Para as curvas, a fila deve possuir apenas os pontos dela, sem nenhum outro tipo de transformação";*/

	int meshCount = 0;
	modelManager ModelManager(g_pToolBar, programID);
	//Adiciona bola
	ModelManager.addMesh("mesh/sphere.obj");
	glm::vec3 posicao(0.0f, -6.0f, 0.0f);
	ModelManager.addModel(programID, "mesh/uvmap.DDS", posicao, 0);
	ModelManager.getModel().at(0).setShaderParameters(0.9, 0.1, 0.1, 0.8, 0.8, 0.2);
	std::cout << "Posicao inicial: " << ModelManager.getModel().at(0).getModelMatrix()[3][0] << " | " << ModelManager.getModel().at(0).getModelMatrix()[3][1] << " | " << ModelManager.getModel().at(0).getModelMatrix()[3][2] << std::endl;
	
	//Adiciona cubo
	ModelManager.addMesh("mesh/cube.obj");
	posicao.y =  -8.0f;
	ModelManager.addModel(programID, "mesh/goose.dds", posicao, 1);
	ModelManager.getModel().at(1).setShaderParameters(0.5, 0.3, 0.8, 0.5, 0.6, 0.5);
	//std::cout << "Posicao inicial: " << ModelManager.getModel().at(1).getPosition().x << " | " << ModelManager.getModel().at(1).getPosition().y << " | " << ModelManager.getModel().at(1).getPosition().z << std::endl;

	//posicao.x = -12.0f;

	//for (int i = 2; i < 25; i++) {
	//	//Adiciona cubo
	//	ModelManager.addMesh("mesh/cube.obj");
	//	posicao.y = 8.0f;
	//	posicao.x += 1.0f;
	//	ModelManager.addModel(programID, "mesh/goose.dds", posicao, 1);
	//	ModelManager.getModel().at(i).setShaderParameters(0.4, 0.8, 0.3, 0.1, 1.0, 0.2);
	//}

	//posicao.x = -11.0f;

	//for (int i = 25; i < 46; i++) {
	//	//Adiciona cubo
	//	ModelManager.addMesh("mesh/cube.obj");
	//	posicao.y = 6.0f;
	//	posicao.x += 1.0f;
	//	ModelManager.addModel(programID, "mesh/goose.dds", posicao, 1);
	//	ModelManager.getModel().at(i).setShaderParameters(0.9, 0.8, 0.5, 0.1, 1.0, 0.2);
	//}

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs(nUseMouse, g_nWidth, g_nHeight);

	do{
        //check_gl_error();

		/*if (glfwGetKey(g_pWindow, GLFW_KEY_L) == GLFW_PRESS) //Se o usuário pressionar L, muda o para o modo de "linhas"
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (glfwGetKey(g_pWindow, GLFW_KEY_P) == GLFW_PRESS) //Se o usuário pressionar P, muda o para o modo de "pontos"
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		if (glfwGetKey(g_pWindow, GLFW_KEY_M) == GLFW_PRESS) //Se o usuário pressionar M, muda o para o modo "cheio" (normal)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		*/
		//if (glfwGetKey(g_pWindow, GLFW_KEY_S) == GLFW_PRESS) //Se o usuário pressionar S, inicia a simplificação da malha
		//	simplificar = 1;

		//if (glfwGetKey(g_pWindow, GLFW_KEY_D) == GLFW_PRESS) //Se o usuário pressionar D, desfaz a simplificação da malha
		//	desfazer = 1;

		//if (glfwGetKey(g_pWindow, GLFW_KEY_Q) == GLFW_PRESS) //Se o usuário pressionar Q, realiza 1 passo de simplificação da malha
		//	stepS = 1;

		//if (glfwGetKey(g_pWindow, GLFW_KEY_W) == GLFW_PRESS) //Se o usuário pressionar W, realiza 1 passo da operação de desfazer a simplificação
		//	stepD = 1;

		//if (glfwGetKey(g_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS) { //Se o usuário pressionar Espaço, para o que estiver fazendo
		//	simplificar = 0;
		//	desfazer = 0;
		//	stepS = 0;
		//	stepD = 0;
		//}

		if (glfwGetKey(g_pWindow, GLFW_KEY_D) == GLFW_PRESS) {
			std::cout << "addBricks chamado\n";
			ModelManager.addBricks();
		}

		
		//Faz a simplificação, contanto que as teclas certas sejam pressionadas, e que ainda seja possível simplificar a malha
		if ((ModelManager.getMesh().at(MeshN).getVertices().size() > 3 && ModelManager.getMesh().at(MeshN).getIndices().size() > 3  && simplificar == 1) || (ModelManager.getMesh().at(MeshN).getVertices().size() > 3 && ModelManager.getMesh().at(MeshN).getIndices().size() > 3 && stepS == 1)) {
			CalculaVizinhos(ModelManager.getMesh().at(MeshN).getVertices(), ModelManager.getMesh().at(MeshN).getIndices(), contagemVizinhos, vizinhos);
			TriangleCollapse(ModelManager.getMesh().at(MeshN).getVertices(), ModelManager.getMesh().at(MeshN).getIndices(), contagemVizinhos, vizinhos, verticesRemovidos, indicesAntigos);
			contagemVizinhos.clear();
			vizinhos.clear();
			stepS = 0;
		}
		else {
			simplificar = 0;
			stepS = 0;
		}

		//Desfaz a simplificação, contanto que as teclas certas sejam pressionadas, e que ainda haja algo para desfazer
		if ((desfazer == 1 && verticesRemovidos.size() != 0) || (stepD == 1 && verticesRemovidos.size() != 0)) {
			refazer(verticesRemovidos, indicesAntigos, ModelManager.getMesh().at(MeshN).getIndices(), ModelManager.getMesh().at(MeshN).getVertices());
			stepD = 0;
		}
		else {
			desfazer = 0;
			stepD = 0;
		}

		double currentTime = glfwGetTime();
		if (currentTime - lastTime >= 0.8) {
			if (glfwGetKey(g_pWindow, GLFW_KEY_B)) { //Adiciona mesh
				std::string meshName;
				std::cout << "\nInsira o caminho da nova mesh:\n";
				std::cin >> meshName;
				const char* meshNameC = meshName.c_str();
				meshCount += 1;
				ModelManager.addMesh(meshNameC);
				std::cout << "Concluído, o ID desta mesh é: " << meshCount << "\n";
			}

			if (glfwGetKey(g_pWindow, GLFW_KEY_N)) { //Adiciona model
				std::string texture;
				std::cout << "\nInsira o caminho da textura do novo model:\n";
				std::cin >> texture;
				const char* textureC = texture.c_str();
				//ModelManager.addModel(programID, textureC, posicaoModel, meshID);
			}
		}


        //use the control key to free the mouse
		if (glfwGetKey(g_pWindow, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
			nUseMouse = 0;
		else
			nUseMouse = 1;

		// Measure speed
		
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			// printf and reset
			//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames  = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		ModelManager.draw(g_pWindow, LightID);

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(g_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(g_pWindow) == 0);

	for(int i = 0; i < ModelManager.getMesh().size(); i++)
		ModelManager.getMesh().at(i).clean();
	glDeleteProgram(programID);

	for (int i = 0; i < ModelManager.getModel().size(); i++)
		ModelManager.getModel().at(i).clean();
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Terminate AntTweakBar and GLFW
	TwTerminate();
	glfwTerminate();

	return 0;
}




