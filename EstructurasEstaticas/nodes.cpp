#include "nodes.h"



Node::Node(float x, float y)
{
	this->x = x;
	this->y = y;
}


void Node::addForce(Force force)
{
	forces.push_back(force);
}
void Node::addForces(std::vector<Force> forces)
{
	for (auto& f : forces)
	{
		forces.push_back(f);
	}
}

void Node::addConnection(Node& node)
{
	connections.push_back(&node);
	node.connections.push_back(this);
}




std::vector<float> Node::distance(const Node& node) const
{
	return std::vector<float>({ node.x - x, node.y - y });
}