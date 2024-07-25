#include "estructura.h"


Estructura::Estructura(const std::vector<Node*>& nodes)
{
	this->nodes = nodes;
}






void Estructura::getSupp() const
{
	int m = 0;
	std::vector<Node*> external; // nodos que reaccionan con fuerzas externas, pueden repetirse si tienen restricciones y reciben fuerzas
	for (Node* node : nodes)
	{
		if (node->hasExternal()) {
			m++;
			external.push_back(node);
		}
		if (node->isSupp()) {
			m++;
			external.push_back(node);
		}
	}

	m *= 2; // cada nodo tiene 2 fuerzas
	Matrix matrix(m, m);
	Matrix vector(m, 1);

	for (int i = 0; i < m / 2; i++)
	{
		// momento
		std::vector<float> distance = external[0]->distance(*external[i]);
		matrix[0][i * 2] = distance[1];
		matrix[0][i * 2 + 1] = distance[0];

		// fuerzas en x
		matrix[1][i * 2] = 1;
		// fuerzas en y
		matrix[2][i * 2 + 1] = 1;
	}

	// fuerzas externas conocidas
	int row = 3;
	for (int i = 0; i < m / 2; i++)
	{
		if (external[i]->isSupp() && external[i]->hasExternal())
		{
			if (!external[i]->isSuppX())
			{
				// Fuerza en x del soporte es 0 porque no tiene
				matrix[row][i * 2] = 1;
				vector[row][0] = 0;
				row++;
			}
			else
			{
				// Fuerza en y del soporte es 0 porque no tiene
				matrix[row][i * 2 + 1] = 1;
				vector[row][0] = 0;
				row++;
			}

			i++; // se pasa al siguiente nodo para agregar las fuerzas externas conocidas

			// Fuerza en x
			matrix[row][i * 2] = 1;
			vector[row][0] = external[i]->forces[0].vector[0];
			//Fuerza en y
			matrix[row + 1][i * 2 + 1] = 1;
			vector[row + 1][0] = external[i]->forces[0].vector[1];
			row += 2;

		}
		else if (external[i]->hasExternal())
		{
			// Fuerza en x
			matrix[row][i * 2] = 1; 
			vector[row][0] = external[i]->forces[0].vector[0];
			//Fuerza en y
			matrix[row + 1][i * 2 + 1] = 1;
			vector[row + 1][0] = external[i]->forces[0].vector[1];
			row += 2;
		}
		else
		{
			if (!external[i]->isSuppX())
			{
				// Fuerza en x del soporte es 0 porque no tiene
				matrix[row][i * 2] = 1;
				vector[row][0] = 0;
				row++;
			}
			else if (!external[i]->isSuppY())
			{
				// Fuerza en y del soporte es 0 porque no tiene
				matrix[row][i * 2 + 1] = 1;
				vector[row][0] = 0;
				row++;
			}
		}
	}
	std::cout << matrix.solve(vector) << std::endl;
}