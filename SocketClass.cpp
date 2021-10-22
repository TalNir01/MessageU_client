#include "SocketClass.h"

SocketClass::SocketClass() 
{
	std::cout << "Hello From Socket Class" << std::endl;
	this->printing_Stuff(5);
}

void SocketClass::printing_Stuff(int i) 
{
	std::cout << i << std::endl;
}