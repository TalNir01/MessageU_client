#pragma once
#include <iostream>
#include <string>
#include "SocketClass.h"
#include <fstream>



#define NAME_BUFFER_LEANGTH 256

#define PUBLIC_KEY_BUFFER_LEANGTH 160


class client_class
{
	typedef char uuid_t[16];

	char client_name[NAME_BUFFER_LEANGTH];
	char public_key[PUBLIC_KEY_BUFFER_LEANGTH];
	char private_key[PUBLIC_KEY_BUFFER_LEANGTH];
	char Version[2];
	char Code[3];


	uuid_t Client_ID;


};

