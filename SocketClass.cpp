#include "SocketClass.h"

SocketClass::SocketClass() 
{
	/*
		Constructor for SocketClass, creating the connection, Constructor for first time log in.
	*/
	std::cout << "[LOG] Hello World From SocketClass Constructor " << std::endl;
	this->ip = "";
	this->port = "";
	this->read_server_info();

	// Init boost connection
	boost::asio::io_context io_context;
	tcp::socket client_sock_conn(io_context);
	tcp::resolver resolver(io_context);

	this->
	// this->client_sock_conn(this->io_context);


}

void SocketClass::printing_Stuff(int i) 
{
	std::cout << i << std::endl;
}

void SocketClass::read_server_info() 
{
	
	FILE *server_info_file;
	server_info_file = fopen("server.info", "r");
	if (!server_info_file) {
		std::cout << "[LOG]Error: server.info opening failed " << std::endl;
	}
	else{
		char tmp;
		while ((tmp = fgetc(server_info_file)) != ':' and tmp != EOF) 
		{
			this->ip += tmp; // add char to ip until reaching ':'
		}
		while ((tmp = fgetc(server_info_file)) != EOF) 
		{
			this->port += tmp; // add char to port until reaching EOF
		}
	}


}