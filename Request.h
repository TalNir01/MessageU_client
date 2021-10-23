#pragma once
#include <iostream>
#include <string>
#include "SocketClass.h"
#include <fstream>

typedef char uuid_t[16];

#define BUFFER_LEANGTH 256

class Request
{
private:
	int Code : 16, Version : 8, Payload_size : 32;
	std::string payload;
	std::string str_client_name;
	char client_name[BUFFER_LEANGTH];

	


	bool getformerloginproprites();


public:
	Request();
	
	bool old_user;



};

