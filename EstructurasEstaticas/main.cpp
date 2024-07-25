#include <iostream>
#include "estructura.h"

int main(int argc, const char* argv[]) {
	Node nodo1(0, 0);
	Node nodo2(4, 3);
	Node nodo3(8, 0);

	nodo1.addConnection(nodo2);
	nodo1.addConnection(nodo3);
	nodo2.addConnection(nodo3);

	nodo1.setSupp(false, true);
	nodo3.setSupp(true, true);

	nodo2.addForce(Force(0, -11.546, 0));

	Estructura estructura({&nodo1, &nodo2, &nodo3});
	estructura.getSupp();


}