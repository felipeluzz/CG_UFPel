#include <meshSimplification.hpp>

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
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Fun��o que calcula os vizinhos de um v�rtice
void CalculaVizinhos(std::vector<glm::vec3> indexed_vertices, std::vector<unsigned short> indices, std::vector<std::pair<int, int>> &contagemVizinhos, std::vector<std::set<unsigned short> > &vizinhos) {

	int pos;
	std::set<unsigned short> set; //Set auxiliar

								  //Loop que itera todos os v�rtices
	for (int i = 0; i < indexed_vertices.size(); i++) {
		set.clear();
		//Loop que itera todos os �ndices
		for (int j = 0; j < indices.size(); j++) {
			if (i == indices[j]) {  //Verifica se encontrou o v�rtice em an�lise no vetor de �ndices
				pos = j % 3; //Verifica em qual posi��o da tripla est�
				if (pos == 0) { //Est� na primeira posi��o da tripla
					set.insert(indices[j + 1]);   //Coloca os vizinhos no set de analisados
					set.insert(indices[j + 2]);
				}

				else if (pos == 1) { //Est� na segunda posi��o da tripla
					set.insert(indices[j - 1]);   //Coloca os vizinhos no set de analisados
					set.insert(indices[j + 1]);
				}

				else if (pos == 2) { //Est� na terceira posi��o da tripla
					set.insert(indices[j - 1]);   //Coloca os vizinhos no set de analisados
					set.insert(indices[j - 2]);
				}
			}
		}
		//Coloca o set de vizinhos no vector
		vizinhos.push_back(set);

		//Coloca contagem dos vizinhos na futura heap
		contagemVizinhos.push_back(std::make_pair(i, vizinhos[i].size()));
	}

	//Cria uma heap com o contador de vizinhos
	std::make_heap(contagemVizinhos.begin(), contagemVizinhos.end(), greaterPair());
}

//Fun��o que remove os tri�ngulos
void TriangleCollapse(std::vector<glm::vec3> &indexed_vertices, std::vector<unsigned short> &indices, std::vector<std::pair<int, int>> &contagemVizinhos, std::vector<std::set<unsigned short> > &vizinhos, std::stack <std::vector<glm::vec3> > &verticesRemovidos, std::stack <std::vector<unsigned short> > &indicesAntigos) {

	int removidos[3]; //Vetor contendo os v�rtices a serem removidos
	std::pair<int, int> verticeSelecionado = { 0,999999 }; //Par auxiliar, para compara��es
	removidos[0] = contagemVizinhos.front().first; //Seleciona o v�rtice no topo da heap como o primeiro a ser removido

												   //Agora, pesquisamos os vizinhos do v�rtice selecionado para remo��o
	std::set<unsigned short> candidatos = vizinhos[removidos[0]]; //Set auxiliar contendo todos os vizinhos do v�rtice a ser removido
	for (std::set<unsigned short>::iterator it = candidatos.begin(); it != candidatos.end(); ++it) { //Percorre o set de candidatos
		int aux = (*it);
		if (vizinhos[aux].size() <= verticeSelecionado.second && aux != removidos[0]) //Verifica se o n�mero de vizinhos do candidato � o menor poss�vel, e se ele j� n�o foi selecionado anteriormente
			verticeSelecionado = std::make_pair(aux, vizinhos[aux].size());  //Guarda o vertice como selecionado
	}
	removidos[1] = verticeSelecionado.first; //Ao final do loop, coloca o v�rtice selecionado no vetor de removidos

											 //Aqui pesquisamos os v�rtices que s�o vizinhos tanto do 1� quando do 2� v�rtice selecionado para descobrir o 3�
	verticeSelecionado = { 0, 999999 };
	std::set<unsigned short> candidatos3; //Set com os v�rtices que s�o vizinhos tanto do 1� selecionado quanto do 2�
	std::set<unsigned short> candidatos2 = vizinhos[removidos[1]];
	std::set_intersection(candidatos.begin(), candidatos.end(), candidatos2.begin(), candidatos2.end(), std::inserter(candidatos3, candidatos3.end())); //Faz uma intersec��o de sets, para ter apenas os v�rtices vizinhos dos 2 v�rtices selecionados
	for (std::set<unsigned short>::iterator it = candidatos3.begin(); it != candidatos3.end(); ++it) {
		int aux = (*it);
		if (vizinhos[aux].size() <= verticeSelecionado.second && aux != removidos[0] && aux != removidos[1]) //Verifica se o n�mero de vizinhos do candidato � o menor poss�vel, e se ele j� n�o foi selecionado anteriormente
			verticeSelecionado = std::make_pair(aux, vizinhos[aux].size());  //Guarda o vertice como selecionado
	}
	removidos[2] = verticeSelecionado.first; //Ao final do loop, coloca o v�rtice selecionado no vetor de removidos


											 //Cria o novo v�rtice
	float novoX = (indexed_vertices[removidos[0]].x + indexed_vertices[removidos[1]].x + indexed_vertices[removidos[2]].x) / 3;
	float novoY = (indexed_vertices[removidos[0]].y + indexed_vertices[removidos[1]].y + indexed_vertices[removidos[2]].y) / 3;
	float novoZ = (indexed_vertices[removidos[0]].z + indexed_vertices[removidos[1]].z + indexed_vertices[removidos[2]].z) / 3;
	glm::vec3 novoVertice(novoX, novoY, novoZ);


	//Organiza o vetor de removidos
	std::sort(removidos, removidos + sizeof removidos / sizeof removidos[0]);

	//Coloca na pilha os v�rtices removidos
	verticesRemovidos.push(indexed_vertices);

	novoVertice.r = indexed_vertices[removidos[0]].r;
	novoVertice.g = indexed_vertices[removidos[0]].g;
	novoVertice.b = indexed_vertices[removidos[0]].b;

	//Insere o novo v�rtice
	indexed_vertices.push_back(novoVertice);

	//Apaga os v�rtices
	indexed_vertices.erase(indexed_vertices.begin() + removidos[2]);
	indexed_vertices.erase(indexed_vertices.begin() + removidos[1]);
	indexed_vertices.erase(indexed_vertices.begin() + removidos[0]);

	//Armazena o vetor de indices antigo
	indicesAntigos.push(indices);

	//Realiza a atualiza��o dos �ndices
	std::set<unsigned short> candidatosNovos; //Set auxiliar, que cont�m os vizinhos dos �ndices modificados
											  //Percorre o vector de �ndices
	for (int i = 0; i < indices.size(); i++) {
		if (indices[i] == removidos[0] || indices[i] == removidos[1] || indices[i] == removidos[2]) { //Testa se o �ndice atual � igual ao �ndice do v�rtice removido
			if ((i % 3) == 0) {  //Inicio da tripla
								 //Coloca os vizinhos deste �ndice no vector de candidatos novos
				candidatosNovos.insert(i + 1);
				candidatosNovos.insert(i + 2);
				indices[i] = indexed_vertices.size() - 1; //Substitui o �ndice atual pelo �ndice do novo v�rtice
			}
			if ((i % 3) == 1) { //Meio da tripla
								//Coloca os vizinhos deste �ndice no vector de candidatos novos
				candidatosNovos.insert(i - 1);
				candidatosNovos.insert(i + 1);
				indices[i] = indexed_vertices.size() - 1; //Substitui o �ndice atual pelo �ndice do novo v�rtice
			}
			if ((i % 3) == 2) { //Final da tripla
								//Coloca os vizinhos deste �ndice no vector de candidatos novos
				candidatosNovos.insert(i - 1);
				candidatosNovos.insert(i - 2);
				indices[i] = indexed_vertices.size() - 1; //Substitui o �ndice atual pelo �ndice do novo v�rtice
			}
		}
		//Atualizando os n�meros dos indices, para bater com os v�rtices
		if (indices[i] > removidos[0] && indices[i] < removidos[1])
			indices[i]--;
		if (indices[i] > removidos[1] && indices[i] < removidos[2])
			indices[i] -= 2;
		if (indices[i] > removidos[2])
			indices[i] -= 3;
	}

	//Conserta tri�ngulos com v�rtices repetidos
	int substitutoEncontrado = 0, j;
	//Percorre todo o vector de �ndices
	for (int i = 0; i < indices.size(); i += 3) {
		if (indices[i] == indices[i + 1] || indices[i] == indices[i + 2] || indices[i + 1] == indices[i + 2]) { //Se est� em uma tripla com repeti��o de v�rtices
			if (indices[i] == indices[i + 1]) { //Se os repetidos s�o o 1� e o segundo da tripla
				j = indices[i] + 2;
				substitutoEncontrado = 0;
				for (auto it = candidatos.begin(); it != candidatos.end() && substitutoEncontrado != 1; ++it) { //Percorre os vizinhos do primeiro vertice removido (vetor candidatos)
					if (*it != j && vizinhos[j].find(*it) == vizinhos[j].end() || candidatosNovos.find(*it) == candidatosNovos.end()) { //Se encontrou um substituto adequado, utiliza ele
						indices[i + 1] = *it;
						substitutoEncontrado = 1;
					}
				}
				if (substitutoEncontrado == 0) {  //N�o encontrou substituto, remove a tripla
					indices.erase(indices.begin() + i);
					indices.erase(indices.begin() + i);
					indices.erase(indices.begin() + i);
				}
			}
			else if (indices[i] == indices[i + 2] || indices[i + 1] == indices[i + 2]) { //Se o primeiro da tripla for igual ao terceiro, ou o segundo for igual ao terceiro
				if (indices[i] == indices[i + 2])  //Se o primeiro for igual ao terceiro da tripla
					j = indices[i] + 1;
				else
					j = 1;

				substitutoEncontrado = 0;
				for (auto it = candidatos.begin(); it != candidatos.end() && substitutoEncontrado != 1; ++it) { //Percorre o vetor de candidatos, procurando um usbstituto
					if (*it != j && vizinhos[j].find(*it) == vizinhos[j].end() || candidatosNovos.find(*it) == candidatosNovos.end()) { //Se encontrou um substituto adequado, utiliza ele
						indices[i + 2] = *it;
						substitutoEncontrado = 1;
					}
				}
				if (substitutoEncontrado == 0) { //Se n�o encontrou, remove a tripla
					indices.erase(indices.begin() + i);
					indices.erase(indices.begin() + i);
					indices.erase(indices.begin() + i);
				}
			}
			else if (indices[i] == indices[i + 1] && indices[i] == indices[i + 2]) { //Se todos os v�rtices da tripla s�o iguais, remove ela
				indices.erase(indices.begin() + i);
				indices.erase(indices.begin() + i);
				indices.erase(indices.begin() + i);
			}
		}
	}
}

void refazer(std::stack <std::vector<glm::vec3> > &verticesRemovidos, std::stack <std::vector<unsigned short> > &indicesAntigos, std::vector<unsigned short> &indices, std::vector<glm::vec3> &indexed_vertices) {

	//Remove o v�rtice "novo" do vector de vertices
	indexed_vertices.pop_back();

	//Re-insere os v�rtices anteriormente removidos
	indexed_vertices = verticesRemovidos.top();
	verticesRemovidos.pop();

	//Substitui o vetor de indices atual pelo antigo
	indices = indicesAntigos.top();
	indicesAntigos.pop();
}