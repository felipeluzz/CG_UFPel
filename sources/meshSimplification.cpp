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

//Função que calcula os vizinhos de um vértice
void CalculaVizinhos(std::vector<glm::vec3> indexed_vertices, std::vector<unsigned short> indices, std::vector<std::pair<int, int>> &contagemVizinhos, std::vector<std::set<unsigned short> > &vizinhos) {

	int pos;
	std::set<unsigned short> set; //Set auxiliar

								  //Loop que itera todos os vértices
	for (int i = 0; i < indexed_vertices.size(); i++) {
		set.clear();
		//Loop que itera todos os índices
		for (int j = 0; j < indices.size(); j++) {
			if (i == indices[j]) {  //Verifica se encontrou o vértice em análise no vetor de índices
				pos = j % 3; //Verifica em qual posição da tripla está
				if (pos == 0) { //Está na primeira posição da tripla
					set.insert(indices[j + 1]);   //Coloca os vizinhos no set de analisados
					set.insert(indices[j + 2]);
				}

				else if (pos == 1) { //Está na segunda posição da tripla
					set.insert(indices[j - 1]);   //Coloca os vizinhos no set de analisados
					set.insert(indices[j + 1]);
				}

				else if (pos == 2) { //Está na terceira posição da tripla
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

//Função que remove os triângulos
void TriangleCollapse(std::vector<glm::vec3> &indexed_vertices, std::vector<unsigned short> &indices, std::vector<std::pair<int, int>> &contagemVizinhos, std::vector<std::set<unsigned short> > &vizinhos, std::stack <std::vector<glm::vec3> > &verticesRemovidos, std::stack <std::vector<unsigned short> > &indicesAntigos) {

	int removidos[3]; //Vetor contendo os vértices a serem removidos
	std::pair<int, int> verticeSelecionado = { 0,999999 }; //Par auxiliar, para comparações
	removidos[0] = contagemVizinhos.front().first; //Seleciona o vértice no topo da heap como o primeiro a ser removido

												   //Agora, pesquisamos os vizinhos do vértice selecionado para remoção
	std::set<unsigned short> candidatos = vizinhos[removidos[0]]; //Set auxiliar contendo todos os vizinhos do vértice a ser removido
	for (std::set<unsigned short>::iterator it = candidatos.begin(); it != candidatos.end(); ++it) { //Percorre o set de candidatos
		int aux = (*it);
		if (vizinhos[aux].size() <= verticeSelecionado.second && aux != removidos[0]) //Verifica se o número de vizinhos do candidato é o menor possível, e se ele já não foi selecionado anteriormente
			verticeSelecionado = std::make_pair(aux, vizinhos[aux].size());  //Guarda o vertice como selecionado
	}
	removidos[1] = verticeSelecionado.first; //Ao final do loop, coloca o vértice selecionado no vetor de removidos

											 //Aqui pesquisamos os vértices que são vizinhos tanto do 1º quando do 2º vértice selecionado para descobrir o 3º
	verticeSelecionado = { 0, 999999 };
	std::set<unsigned short> candidatos3; //Set com os vértices que são vizinhos tanto do 1º selecionado quanto do 2º
	std::set<unsigned short> candidatos2 = vizinhos[removidos[1]];
	std::set_intersection(candidatos.begin(), candidatos.end(), candidatos2.begin(), candidatos2.end(), std::inserter(candidatos3, candidatos3.end())); //Faz uma intersecção de sets, para ter apenas os vértices vizinhos dos 2 vértices selecionados
	for (std::set<unsigned short>::iterator it = candidatos3.begin(); it != candidatos3.end(); ++it) {
		int aux = (*it);
		if (vizinhos[aux].size() <= verticeSelecionado.second && aux != removidos[0] && aux != removidos[1]) //Verifica se o número de vizinhos do candidato é o menor possível, e se ele já não foi selecionado anteriormente
			verticeSelecionado = std::make_pair(aux, vizinhos[aux].size());  //Guarda o vertice como selecionado
	}
	removidos[2] = verticeSelecionado.first; //Ao final do loop, coloca o vértice selecionado no vetor de removidos


											 //Cria o novo vértice
	float novoX = (indexed_vertices[removidos[0]].x + indexed_vertices[removidos[1]].x + indexed_vertices[removidos[2]].x) / 3;
	float novoY = (indexed_vertices[removidos[0]].y + indexed_vertices[removidos[1]].y + indexed_vertices[removidos[2]].y) / 3;
	float novoZ = (indexed_vertices[removidos[0]].z + indexed_vertices[removidos[1]].z + indexed_vertices[removidos[2]].z) / 3;
	glm::vec3 novoVertice(novoX, novoY, novoZ);


	//Organiza o vetor de removidos
	std::sort(removidos, removidos + sizeof removidos / sizeof removidos[0]);

	//Coloca na pilha os vértices removidos
	verticesRemovidos.push(indexed_vertices);

	novoVertice.r = indexed_vertices[removidos[0]].r;
	novoVertice.g = indexed_vertices[removidos[0]].g;
	novoVertice.b = indexed_vertices[removidos[0]].b;

	//Insere o novo vértice
	indexed_vertices.push_back(novoVertice);

	//Apaga os vértices
	indexed_vertices.erase(indexed_vertices.begin() + removidos[2]);
	indexed_vertices.erase(indexed_vertices.begin() + removidos[1]);
	indexed_vertices.erase(indexed_vertices.begin() + removidos[0]);

	//Armazena o vetor de indices antigo
	indicesAntigos.push(indices);

	//Realiza a atualização dos índices
	std::set<unsigned short> candidatosNovos; //Set auxiliar, que contém os vizinhos dos índices modificados
											  //Percorre o vector de índices
	for (int i = 0; i < indices.size(); i++) {
		if (indices[i] == removidos[0] || indices[i] == removidos[1] || indices[i] == removidos[2]) { //Testa se o índice atual é igual ao índice do vértice removido
			if ((i % 3) == 0) {  //Inicio da tripla
								 //Coloca os vizinhos deste índice no vector de candidatos novos
				candidatosNovos.insert(i + 1);
				candidatosNovos.insert(i + 2);
				indices[i] = indexed_vertices.size() - 1; //Substitui o índice atual pelo índice do novo vértice
			}
			if ((i % 3) == 1) { //Meio da tripla
								//Coloca os vizinhos deste índice no vector de candidatos novos
				candidatosNovos.insert(i - 1);
				candidatosNovos.insert(i + 1);
				indices[i] = indexed_vertices.size() - 1; //Substitui o índice atual pelo índice do novo vértice
			}
			if ((i % 3) == 2) { //Final da tripla
								//Coloca os vizinhos deste índice no vector de candidatos novos
				candidatosNovos.insert(i - 1);
				candidatosNovos.insert(i - 2);
				indices[i] = indexed_vertices.size() - 1; //Substitui o índice atual pelo índice do novo vértice
			}
		}
		//Atualizando os números dos indices, para bater com os vértices
		if (indices[i] > removidos[0] && indices[i] < removidos[1])
			indices[i]--;
		if (indices[i] > removidos[1] && indices[i] < removidos[2])
			indices[i] -= 2;
		if (indices[i] > removidos[2])
			indices[i] -= 3;
	}

	//Conserta triângulos com vértices repetidos
	int substitutoEncontrado = 0, j;
	//Percorre todo o vector de índices
	for (int i = 0; i < indices.size(); i += 3) {
		if (indices[i] == indices[i + 1] || indices[i] == indices[i + 2] || indices[i + 1] == indices[i + 2]) { //Se está em uma tripla com repetição de vértices
			if (indices[i] == indices[i + 1]) { //Se os repetidos são o 1º e o segundo da tripla
				j = indices[i] + 2;
				substitutoEncontrado = 0;
				for (auto it = candidatos.begin(); it != candidatos.end() && substitutoEncontrado != 1; ++it) { //Percorre os vizinhos do primeiro vertice removido (vetor candidatos)
					if (*it != j && vizinhos[j].find(*it) == vizinhos[j].end() || candidatosNovos.find(*it) == candidatosNovos.end()) { //Se encontrou um substituto adequado, utiliza ele
						indices[i + 1] = *it;
						substitutoEncontrado = 1;
					}
				}
				if (substitutoEncontrado == 0) {  //Nâo encontrou substituto, remove a tripla
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
				if (substitutoEncontrado == 0) { //Se não encontrou, remove a tripla
					indices.erase(indices.begin() + i);
					indices.erase(indices.begin() + i);
					indices.erase(indices.begin() + i);
				}
			}
			else if (indices[i] == indices[i + 1] && indices[i] == indices[i + 2]) { //Se todos os vértices da tripla são iguais, remove ela
				indices.erase(indices.begin() + i);
				indices.erase(indices.begin() + i);
				indices.erase(indices.begin() + i);
			}
		}
	}
}

void refazer(std::stack <std::vector<glm::vec3> > &verticesRemovidos, std::stack <std::vector<unsigned short> > &indicesAntigos, std::vector<unsigned short> &indices, std::vector<glm::vec3> &indexed_vertices) {

	//Remove o vértice "novo" do vector de vertices
	indexed_vertices.pop_back();

	//Re-insere os vértices anteriormente removidos
	indexed_vertices = verticesRemovidos.top();
	verticesRemovidos.pop();

	//Substitui o vetor de indices atual pelo antigo
	indices = indicesAntigos.top();
	indicesAntigos.pop();
}