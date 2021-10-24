#include "SocketClass.h"
#include "handleUI.h"

handleUI::handleUI()
{

}

void handleUI::printUI() 
{
	std::cout << "MessageU client all your service." << std::endl;
	std::cout << "\n" << std::endl;
	std::cout << "10) Register " << std::endl;
	std::cout << "20) Request for clients list " << std::endl;
	std::cout << "30) Request for public key " << std::endl;
	std::cout << "40) Request for waiting messages " << std::endl;
	std::cout << "50) Send a text message " << std::endl;
	std::cout << "51) Send a request for symmetric key " << std::endl;
	std::cout << "52) Send your symmetric key " << std::endl;
	std::cout << "0) Exit client " << std::endl;
	std::cout << "? " << std::endl;

}