#include "SocketClass.h"

SocketClass::SocketClass() 
{
	/*
		Constructor for SocketClass, creating the connection, Constructor for first time log in.
	*/
	std::cout << "[LOG] Hello World From SocketClass Constructor " << std::endl;
	this->address = "";
	this->port = "";
	this->read_server_info();

	// Init boost connection
	boost::asio::io_context io_context;
	tcp::socket client_sock_conn(io_context);
	tcp::resolver resolver(io_context);
	
	// Connect to server.
	boost::asio::connect(client_sock_conn, resolver.resolve(this->address, this->port));
	
	// make the connection an Object proprtie
	this->client_sock_conn_pointer = &client_sock_conn;
	
	//

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
			this->address += tmp; // add char to ip until reaching ':'
		}
		while ((tmp = fgetc(server_info_file)) != EOF) 
		{
			this->port += tmp; // add char to port until reaching EOF
		}
	}


}