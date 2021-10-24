#pragma once
#include <iostream>
#include <string>
#include "SocketClass.h"
#include <fstream>


#define NAME_BUFFER_LEANGTH 256

#define PUBLIC_KEY_BUFFER_LEANGTH 255

class Request
{
private:
	int Code : 16, Version : 8, Payload_size : 32;
	//std::string payload;
	std::string str_client_name;
	char client_name[NAME_BUFFER_LEANGTH];
	char public_key[PUBLIC_KEY_BUFFER_LEANGTH];

	char private_key[PUBLIC_KEY_BUFFER_LEANGTH];
	char Version[2];
	char Code[3];

	typedef char uuid_t[16];
	
	uuid_t Client_ID;
	
	char* get_private_key();

	bool setloginproprties();

	bool getformerloginproprites();

	tcp::socket* req_socket_conn_pointer;

public:
	Request(tcp::socket* conn_pointer);
	
	bool old_user;

	bool service_10_code_1000();

};

