#pragma once

#include "linearAlgebra.h"
#include "nodes.h"


// Dise�ado para resolver estructuras por nodos
class Estructura
{
public:
	std::vector<Node*> nodes;



public:
	Estructura(const std::vector<Node*>& nodos);


	void getSupp() const;

};
