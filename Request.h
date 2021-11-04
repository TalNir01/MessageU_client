#pragma once

#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include <fstream>
#include <cstring>
#include <vector>

#include "client_class.h"
#include "SocketClass.h"


#define CODE_LEANGTH_BUFFER 2
#define NAME_BUFFER_LEANGTH 255
#define UUID_LEANGTH 16
#define MSG_ID_LEANGTH 4
#define TYPE_MSG_BUFFER_LEANGTH 2
#define KEY_BUFFER_LEANGTH 160
#define PAYLOAD_SIZE_BUFFER 5
#define MSG_SISE_BUFFER 5

typedef char uuid_t[UUID_LEANGTH];
typedef char msg_uuid_t[MSG_ID_LEANGTH];


typedef struct response_header {
	char Version[2];
	char Code[3];
	char payload_size[5];
} response_header_class;




class Request
{
private:
	int Payload_size : 32;
	//std::string payload;
	
	SocketClass* connection;


	unsigned short int_code;

	//std::map <uuid_t, std::string> client_list;

	char Payload_str_size[4];
	char client_name[NAME_BUFFER_LEANGTH];
	char public_key[KEY_BUFFER_LEANGTH];
	char private_key[KEY_BUFFER_LEANGTH];
	char Version[2];
	char Code[3];

	//std::map<std::string, std::string> client_list;
	//start
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
		char Public_key[KEY_BUFFER_LEANGTH]; // client(self) public key
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
		char Message_Type[TYPE_MSG_BUFFER_LEANGTH];
		char Message_Size[MSG_SISE_BUFFER];
		std::string Message_Content_str;
	};

	struct paypayload_1004 { // pulling messages from server
		// should be empty, NULL
	};


	// response

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
		char Public_Key[KEY_BUFFER_LEANGTH];
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
		char Message_Type[TYPE_MSG_BUFFER_LEANGTH];
		char Message_Size[MSG_SISE_BUFFER];
		std::string Content_str;
	};

	struct another_client_info // recieve a pulled msg
	{
		uuid_t Client_ID; // sender
		char Public_Key[KEY_BUFFER_LEANGTH];
		char Client_Name[NAME_BUFFER_LEANGTH]; // client name length
		char symmetric_key[KEY_BUFFER_LEANGTH];
	};

	// end
	uuid_t Client_ID;

	char* get_private_key();

	bool setloginproprties();

	bool getformerloginproprites();

	tcp::socket* req_socket_conn_pointer;

	std::string get_decode_payload(std::string encoded_data, uuid_t id);

	char* ret_client_id(std::string name);

	reqest_header req_header;

	response_header_class header_response;

	bool send_header();

	response_header_class read_header();

	std::vector<another_client_info> client_list; // a client list that holds all the data

	std::string get_name_by_id(uuid_t id);

	std::string get_symmetric_key();

	std::string get_public_key();

	std::string encode(std::string data, uuid_t id);//symetric

	std::string decode(std::string data, uuid_t id);//symetric

	std::string encode_by_public_key(std::string data, uuid_t id);

	std::string decode_by_private_key(std::string data, uuid_t id);








public:
	Request(tcp::socket* conn_pointer);
	
	bool old_user;

	bool service_10_code_1000();// 

	bool service_20_code_1001();//

	bool service_30_code_1002();//

	bool service_40_code_1004();// ask for message list

	bool service_50_code_1003(int type);

	bool service_51_type_1_code_1003(); // request getting symmetric key

	bool service_50_type_3_code_1003();

	bool service_52_type_2_code_1003();

};