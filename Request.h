#pragma once
#include <iostream>
#include <string>
#include "SocketClass.h"
#include <fstream>
#include "client_class.h"
#include <map>
#define NAME_BUFFER_LEANGTH 256

#define PUBLIC_KEY_BUFFER_LEANGTH 160

class Request
{
private:
	int Payload_size : 32;
	//std::string payload;
	typedef char uuid_t[16];
	std::string str_client_name;

	char Payload_str_size[4];
	char client_name[NAME_BUFFER_LEANGTH];
	char public_key[PUBLIC_KEY_BUFFER_LEANGTH];
	char private_key[PUBLIC_KEY_BUFFER_LEANGTH];
	char Version[2];
	char Code[3];

	std::map<std::string, std::string> client_list;
	
	
	uuid_t Client_ID;
	
	char* get_private_key();

	bool setloginproprties();

	bool getformerloginproprites();

	tcp::socket* req_socket_conn_pointer;

	bool send_header();

	std::string get_decode_payload(const char* response_encrypted_payload);
	
	
	char * ret_client_id(std::string name);


	bool service_51_type_2(); // request getting symmetric key

	bool service_50_type_3();

	bool service_52_type_1();


public:
	Request(tcp::socket* conn_pointer);
	
	bool old_user;

	bool service_10_code_1000();// 

	bool service_20_code_1001();//

	bool service_30_code_1002();//

	bool service_40_code_1004();// ask for message list

	bool service_50_code_1003(int type);






};

