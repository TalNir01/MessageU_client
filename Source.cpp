//#include <boost/asio.hpp> 
#include <iostream> 
// using boost::asio::ip::tcp;
#include <math.h>
#include "SocketClass.h"

int main()
{
	SocketClass* c1 = new SocketClass;
	std::cout << "Hello World \n" << std::endl;
	std::cout << c1 << std::endl;

	return 0;
}
