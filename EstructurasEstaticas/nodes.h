#pragma once

#include "linearAlgebra.h"
#include "forces.h"

class Node
{
public:
	std::vector<Force> forces;
	std::vector<Node*> connections;

	// Posicion del nodo
	float x = 0;
	float y = 0;

private:
	bool suppX = false;
	bool suppY = false;

public:
	Node(float x, float y);

	void addForce(Force force);
	void addForces(std::vector<Force> forces);
	void addConnection(Node& node);

	void setSupp(bool _suppX, bool _suppY) { suppX = _suppX; suppY = _suppY; };
	bool isSupp() const { return suppX || suppY; }
	bool isSuppX() const { return suppX; }
	bool isSuppY() const { return suppY; }

	bool hasExternal() const { return forces.size() > 0; }


	std::vector<float> distance(const Node& node) const;
};