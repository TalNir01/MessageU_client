#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <cstring>

#include "client_class.h"
#include "SocketClass.h"



#define NAME_BUFFER_LEANGTH 255
#define UUID_LEANGTH 16
#define KEY_BUFFER_LEANGTH 160

class Request
{
private:
	int Payload_size : 32;
	//std::string payload;
	typedef char uuid_t[16];
	typedef char msg_uuid_t[4];
	unsigned short tmp_code;


	char Payload_str_size[4];
	char client_name[NAME_BUFFER_LEANGTH];
	char public_key[KEY_BUFFER_LEANGTH];
	char private_key[KEY_BUFFER_LEANGTH];
	char Version[2];
	char Code[3];

	std::map<std::string, std::string> client_list;


	uuid_t Client_ID;

	char* get_private_key();

	bool setloginproprties();

	bool getformerloginproprites();

	tcp::socket* req_socket_conn_pointer;

	bool send_header();

	int read_header();



	std::string get_decode_payload(const char* response_encrypted_payload);

	char* ret_client_id(std::string name);


	bool service_51_type_2(); // request getting symmetric key

	bool service_50_type_3();

	bool service_52_type_1();


	// request

	struct reqest_header {
		uuid_t Client_ID;
		char Version[2];
		char Code[3];
		char payload_size[5];
	};

	struct payload_1000  // regestration payload
	{
		char Name[NAME_BUFFER_LEANGTH];
		char Public_key[PUBLIC_KEY_BUFFER_LEANGTH]; // client(self) public key
	};

	struct paypayload_1001 { // client list payload
		// should be empty, NULL
	};

	struct payload_1002 // asking for another client public key.
	{
		uuid_t Client_ID;
	};

	struct payload_1003 // sending a message to client[3 types]
	{
		uuid_t Client_ID; // id of dest client
		char Message_Type; // type of msg 1 - ask for symetric key; 2 - send symmetric key
		char Content_Size[4]; // !!!!!!!!!!!!!!!! think again of size
		std::string Message_Content_str;
	};

	struct paypayload_1004 { // pulling messages from server
		// should be empty, NULL
	};


	// response

	struct response_header {
		char Version[2];
		char Code[3];
		char payload_size[5];
	};

	struct payload_2000  // regestration success response
	{
		uuid_t Client_ID; // client new id
	};

	struct payload_2001 // client list
	{
		uuid_t Client_ID; // client id
		char Client_Name[NAME_BUFFER_LEANGTH]; // client name length
	};
	
	struct payload_2002 // returning a public key
	{
		uuid_t Client_ID;
		char Public_Key[PUBLIC_KEY_BUFFER_LEANGTH];
	};

	struct payload_2003 // message was sent to client(saved at server)
	{
		uuid_t Client_ID; // dest client id
		msg_uuid_t Message_ID;
	};

	struct payload_2004 // recieve a pulled msg
	{
		uuid_t Client_ID; // sender
		msg_uuid_t Message_ID;
		char Message_Type;
		char Message_Size[4];
		std::string Content_str;
	};

	reqest_header req_header;

	response_header header_response;


public:
	Request(tcp::socket* conn_pointer);
	
	bool old_user;

	bool service_10_code_1000();// 

	bool service_20_code_1001();//

	bool service_30_code_1002();//

	bool service_40_code_1004();// ask for message list

	bool service_50_code_1003(int type);

};

