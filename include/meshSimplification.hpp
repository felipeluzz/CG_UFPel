#ifndef MESHSIMPLIFICATION_H
#define MESHSIMPLIFICATION_H

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


//Protótipos das funções
void CalculaVizinhos(std::vector<glm::vec3> indexed_vertices, std::vector<unsigned short> indices, std::vector<std::pair<int, int>> &contagemVizinhos, std::vector<std::set<unsigned short> > &vizinhos);
void TriangleCollapse(std::vector<glm::vec3> &indexed_vertices, std::vector<unsigned short> &indices, std::vector<std::pair<int, int>> &contagemVizinhos, std::vector<std::set<unsigned short> > &vizinhos, std::stack <std::vector<glm::vec3> > &verticesRemovidos, std::stack <std::vector<unsigned short> > &indicesAntigos);
void refazer(std::stack <std::vector<glm::vec3> > &verticesRemovidos, std::stack <std::vector<unsigned short> > &indicesAntigos, std::vector<unsigned short> &indices, std::vector<glm::vec3> &indexed_vertices);

//Função auxiliar que compara o primeiro valor de um par com um valor informado, retornando Ture ou False
struct FindPair
{
	int aux;
public:
	FindPair(const int& firstValue) {
		aux = firstValue;
	}

	bool operator()(const std::pair<int, int>& val) {
		return val.first == aux;
	}

};

//Função que compara os segundos valores de um par
struct greaterPair {
	bool operator()(std::pair<int, int> const &a, std::pair<int, int> const &b) const {
		return a.second > b.second;
	}
};

#endif
