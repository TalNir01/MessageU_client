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
	this->socket = &client_sock_conn;

	// Connect to server.
	boost::asio::connect(*this->socket, resolver.resolve(this->address, this->port));
	
	
}

bool SocketClass::send(const char* data_to_send, const int len)
{
	try {
		boost::asio::write(*this->socket, boost::asio::buffer(data_to_send, len));
		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << e.what() << std::endl;
		return false;
	}

}

std::string SocketClass::read(const int len)
{
	
	//char recive_buffer[len];
	std::string data;
	size_t length_of_read;
	try {
		length_of_read = boost::asio::read(*this->socket, boost::asio::buffer(data, len));
		return data;
	}
	catch (std::exception& e)
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << e.what() << std::endl;
		return data;
	}

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

/*std::ifstream inf{ "me.info" };
	if (!inf) 
	{
		std::cout << "[LOG] User dont exsits [couldn't find me.info therefore it's empty] " << std::endl;
		this->old_user = false;
		return false;
	}
	

	// add tests to make sure all data is intact
	std::getline(inf, client_name_tmp);
	strcpy_s(this->client_name, NAME_BUFFER_LEANGTH, client_name_tmp.c_str());
	
	std::getline(inf, str_uuid);
	strcpy_s(this->Client_ID, UUID_LEANGTH, str_uuid.c_str());

	std::getline(inf, private_key_str);
	strcpy_s(this->private_key, KEY_BUFFER_LEANGTH, private_key_str.c_str());

	strcpy_s(this->req_header.Client_ID, UUID_LEANGTH, this->Client_ID);*/