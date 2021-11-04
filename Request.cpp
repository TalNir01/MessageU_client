#include "Request.h"

Request::Request(tcp::socket* conn_pointer)
{
	this->req_socket_conn_pointer = conn_pointer;
	strcpy_s(this->req_header.Version, 1, "1");
	strcpy_s(this->Version, 1, "1");

	this->old_user = false;

	this->connection = new SocketClass();

	if (!this->getformerloginproprites()) 
	{
		// meaning that the client is new and must login
		std::cout << "[LOG] the user is new. " << std::endl;
		// generate keys

	}
	else 
	{
		std::cout << "[LOG] the user is OLD. We have your data. :)" << std::endl;
	}
	
}


std::string Request::get_name_by_id(uuid_t id) 
{
	for (auto i = this->client_list.begin(); i != this->client_list.end(); i++) 
	{
		if ((*i).Client_ID == id) 
		{
			return (*i).Client_Name;
		}
	}

}

/*	Sending the Header */
bool Request::send_header()
{
	try 
	{
		this->connection->send(this->req_header.Client_ID, UUID_LEANGTH);
		this->connection->send(this->req_header.Version, 1);
		this->connection->send(this->req_header.Code, CODE_LEANGTH_BUFFER);
		this->connection->send(this->req_header.payload_size, PAYLOAD_SIZE_BUFFER);
		
		std::cout << "[LOG] send the header." << std::endl;

		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << "[LOG] Error at sending header -> soket error: " << e.what() << std::endl;
		return false;
	}

	
}


/*	Reading the Header */
response_header_class Request::read_header()
{
	//size_t lang;
	int response_payload_size = 0;
	response_header_class tmp_response_header;

	try
	{
		std::string tmp_header_version = this->connection->read(1);
		strcpy_s(tmp_response_header.Version, 1, tmp_header_version.c_str());
		
		std::string tmp_header_code = this->connection->read(CODE_LEANGTH_BUFFER);
		strcpy_s(tmp_response_header.Code, CODE_LEANGTH_BUFFER, tmp_header_code.c_str());
		
		this->int_code = atoi(tmp_response_header.Code);

		std::string tmp_header_payload_size = this->connection->read(4);
		strcpy_s(tmp_response_header.payload_size, 4, tmp_header_payload_size.c_str());

	
		std::cout << "[LOG] read the header." << std::endl;

		//return true;
	}
	catch (std::exception& e)
	{
		std::cerr << "[LOG] Error at sending header -> soket error: " << e.what() << std::endl;
		//return false;
	}

	return tmp_response_header;

}


char * Request::ret_client_id(std::string name)
{
	const char id[17] = "0000000000000000";
	return NULL;
}


bool Request::getformerloginproprites()
{
	/*
	  cheking in me.info if users was already defined.
	*/
	std::string client_name_tmp;
	std::string str_uuid;
	std::string private_key_str;


	std::ifstream inf{ "me.info" };
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

	strcpy_s(this->req_header.Client_ID, UUID_LEANGTH, this->Client_ID);

	


	this->old_user = true;
	return true;

}




bool Request::service_10_code_1000() // service 10 regestration
{
	
	payload_1000 service_10;
	payload_2000 response_2000;
	response_header tmp_response_header;

	if (this->old_user) 
	{
		std::cout << "User already exsits, can't register again. " << std::endl;
		return false;
	}

	
	try 
	{

		strcpy_s(this->req_header.Code, CODE_LEANGTH_BUFFER, "1000");

		strcpy_s(this->req_header.Client_ID, "0");

		strcpy_s(service_10.Public_key, KEY_BUFFER_LEANGTH, this->get_private_key());

		std::cout << "Enter your user-name: ";

		std::cin.getline(this->client_name, NAME_BUFFER_LEANGTH);
		std::cout << "Got it, trying to register..." << std::endl;

		std::cout << "registering... " << std::endl;

		strcpy_s(service_10.Name, NAME_BUFFER_LEANGTH, this->client_name);

		// Payload size
		int req_size_tmp = KEY_BUFFER_LEANGTH + NAME_BUFFER_LEANGTH;
		std::string load_size_tmp = std::to_string(req_size_tmp);
		
		strcpy_s(this->req_header.payload_size, 5, load_size_tmp.c_str());
		
		// sending the header(req_header)
		if (!this->send_header())
		{
			return false;
		}
		
		
		this->connection->send(service_10.Name, NAME_BUFFER_LEANGTH);
		this->connection->send(service_10.Public_key, KEY_BUFFER_LEANGTH);
	
	}
	catch (std::exception& e) 
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << e.what() << std::endl;
		return false;
	}

	//	payload_2000 response_2000;


	// reciveing response from the server.
	try // 2000 response code
	{
		int length_of_payload = 0;
		//size_t leng;
		std::string tmp_client_id;

		tmp_response_header = this->read_header();
		if (tmp_response_header.Code != "2000")
		{
			std::cerr << "[LOG] Error, got worng response code. " << std::endl;
			return false;
		}

		tmp_client_id = this->connection->read(16);
		strcpy_s(response_2000.Client_ID, 16, tmp_client_id.c_str());
		

		strcpy_s(this->req_header.Client_ID, UUID_LEANGTH, response_2000.Client_ID);
		

		std::ofstream outf{ "me.info" };

		// If we couldn't open the output file stream for writing
		if (!outf)
		{
			// Print an error and exit
			std::cerr << " [LOG] Error: error acured" << std::endl;
			return 1;
		}

		// We'll write two lines into this file
		outf << this->client_name << '\n';
		outf << this->Client_ID << '\n';
		outf << this->private_key << '\n';
		
		outf.close();

		this->old_user = true;

		//delete tmp_response_header;

	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Server responded with an error. " << e.what() << std::endl;
		return true;
	}


	return true;

}


bool Request::service_20_code_1001() // ask for client list
{
	response_header_class tmp_response_header;
	//payload_2001 response_payload; // we dont need it becuse we used a better kind of structure "another_client_info***"
	//size_t leng;
	std::string name;
	std::string id;
	if (!this->old_user) 
	{
		std::cout << "Error, no registration " << std::endl;
		return false;
	}

	try 
	{
		// init the header 
		strcpy_s(this->req_header.Code, CODE_LEANGTH_BUFFER, "1001");
		strcpy_s(this->req_header.payload_size, PAYLOAD_SIZE_BUFFER, "0");
		
		if (!this->send_header()) // sending the Request header
		{
			std::cout << "Server responded with an error. " << std::endl;
			return false;
		}
		
	}
	catch (std::exception& e) 
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;
	
	}

	// read the response
	try 
	{
		tmp_response_header = this->read_header();

		if (tmp_response_header.Code != "2001")
		{
			std::cout << "server responded with an error " << std::endl;
			return false;

		}


		const int total_payload_size = atoi(tmp_response_header.payload_size);
		if (total_payload_size == 0)
		{
			if (tmp_response_header.payload_size != "0")
				return false;
		}

		int num_of_clients = total_payload_size / (16 + 255);

		for (int i = 0; i < num_of_clients; i++) 
		{
			another_client_info tmp_info;
			id = this->connection->read(UUID_LEANGTH);
			strcpy_s(tmp_info.Client_ID, UUID_LEANGTH, id.c_str());
			
			id = this->connection->read(NAME_BUFFER_LEANGTH);
			strcpy_s(tmp_info.Client_Name, NAME_BUFFER_LEANGTH, id.c_str());
			
			this->client_list.push_back(tmp_info);

			std::cout << " [" << tmp_info.Client_ID << " : " << tmp_info.Client_Name << "] " << std::endl;

		}

		return true;

	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return true;

	}
	return true;
}




bool Request::service_30_code_1002() // ask for client public key
{
	/*
		Asking for client public key, reading the response adding it to the client list
	
	*/
	payload_2002 response_payload;
	payload_1002 request_payload;
	std::string name_str;
	std::string publickey_str;
	std::string res_pay_size_str;
	response_header_class response_head;
	uuid_t tmp_id;

	if (!this->old_user)
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << std::endl;
		return false;
	}

	try 
	{
		strcpy_s(this->req_header.Code, CODE_LEANGTH_BUFFER, "1002");
		strcpy_s(this->req_header.payload_size, PAYLOAD_SIZE_BUFFER, "16");


		if (!this->send_header()) 
		{
			std::cerr << "[LOG] Error at sending section -> soket error: " << std::endl;
			return false;
		}
		
		std::cout << "enter name of client: ";
		std::cin >> name_str;

		if (!strcmp(this->client_name, name_str.c_str())) // it's us
		{
			strcpy_s(tmp_id, UUID_LEANGTH, this->Client_ID);
		
		}
		else 
		{
			strcpy_s(tmp_id, UUID_LEANGTH, "0");
			for (auto i = this->client_list.begin(); i != this->client_list.end(); ++i) 
			{
				if (!strcmp(this->client_name, name_str.c_str()))
				{
					strcpy_s(tmp_id, UUID_LEANGTH, this->Client_ID);
				}
			
			}
		}

		
		strcpy_s(request_payload.Client_ID, UUID_LEANGTH, tmp_id);
		this->connection->send(request_payload.Client_ID, 16);

		// read response
		try 
		{
			char serverV[2];
			char response_code[3];
			char response_payload_size[5];
			std::string tmp_id;
			//size_t leng;

			response_head = this->read_header();

			if (response_head.Code != "2002") 
			{
				std::cout << "Server Responded with an error" << std::endl;
				return false;
			}

			res_pay_size_str = this->connection->read(PAYLOAD_SIZE_BUFFER);
			strcpy_s(response_head.payload_size, PAYLOAD_SIZE_BUFFER, res_pay_size_str.c_str());

			const int resleng = atoi(response_head.payload_size);
			if (resleng == 0)
			{
				if (response_head.payload_size != "0")
					return false;
			}
			
			tmp_id = this->connection->read(UUID_LEANGTH);
			
			if (tmp_id.empty()) 
			{
				std::cout << "Server Responded with an error" << std::endl;
				return false;
			}

			strcpy_s(response_payload.Client_ID, UUID_LEANGTH, tmp_id.c_str());

			publickey_str = this->connection->read(KEY_BUFFER_LEANGTH);

			if (publickey_str.empty())
			{
				std::cout << "Server Responded with an error" << std::endl;
				return false;
			}

			strcpy_s(response_payload.Public_Key, KEY_BUFFER_LEANGTH, publickey_str.c_str());
			
			if (response_payload.Client_ID == this->req_header.Client_ID) // asking for our own public key.
			{
				strcpy_s(this->public_key, KEY_BUFFER_LEANGTH, response_payload.Public_Key);
				std::cout << "Your public key is :" << this->public_key << std::endl;
			}
			else 
			{
				for (auto i = this->client_list.begin(); i != this->client_list.end(); i++) 
				{
					if ((*i).Client_ID == response_payload.Client_ID) // add public key to client_list
					{
						strcpy_s((*i).Public_Key, KEY_BUFFER_LEANGTH, response_payload.Public_Key);
						std::cout << (*i).Client_Name << " public key is: " << (*i).Public_Key << std::endl;
					}
				
				}
			}

			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error : Error is " << e.what() << std::endl;
			return false;

		}



	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

}

// ################################################################################################ reading messages for message list


bool Request::service_40_code_1004() // ask for message list
{
	/*
		managing the ask for message list(send and read response)
	*/
	if (!this->old_user)
	{
		std::cerr << "[LOG] New user, need to registar " << std::endl;
		return false;
	}

	try
	{

		strcpy_s(this->req_header.Client_ID, UUID_LEANGTH, this->Client_ID);
		strcpy_s(this->req_header.Version, 2, "1");
		strcpy_s(this->req_header.Code, 3, "1004");
		strcpy_s(this->req_header.payload_size, 5, "0");


		if (! this->send_header())
		{
			std::cout << "Error, Server responded with error " << std::endl;
			return false;
		}

	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	//read pulled msg
	response_header_class resp_head;
	payload_2004 resp_payload;
	unsigned int total_payload_response_size;
	int i;
	std::string tmp_id;
	std::string msg_tmp_id;
	std::string msg_t;
	std::string msg_size;
	std::string msg_encoded_content;
	try
	{
		//reading head;

		resp_head = this->read_header();
		if (resp_head.Code != "2004") 
		{
			std::cout << "Server responded with an error. " << std::endl;
			return false;
		}
		
		total_payload_response_size = atoi(resp_head.payload_size);
		i = total_payload_response_size;

		while (i > 0)
		{
			
			tmp_id = this->connection->read(UUID_LEANGTH);
			strcpy_s(resp_payload.Client_ID, UUID_LEANGTH, tmp_id.c_str());

			msg_tmp_id = this->connection->read(MSG_ID_LEANGTH);
			strcpy_s(resp_payload.Message_ID, MSG_ID_LEANGTH, msg_tmp_id.c_str());

			msg_t = this->connection->read(TYPE_MSG_BUFFER_LEANGTH);
			strcpy_s(resp_payload.Message_Type, TYPE_MSG_BUFFER_LEANGTH, msg_t.c_str());

			msg_size = this->connection->read(MSG_SISE_BUFFER);
			strcpy_s(resp_payload.Message_Size, MSG_SISE_BUFFER, msg_size.c_str());

			i -= (UUID_LEANGTH + MSG_ID_LEANGTH + TYPE_MSG_BUFFER_LEANGTH + atoi(msg_size.c_str()));
			
			resp_payload.Content_str = this->connection->read(atoi(msg_size.c_str()));

			std::cout << "----SOM----" << std::endl;
			std::cout << "From <" << this->get_name_by_id(resp_payload.Client_ID) << ">" << std::endl;
			std::cout << "Content:" << std::endl;

			if (resp_payload.Message_Type == "1") // asked for symmetric key (now we need to send out symmetric key)
			{
				std::cout << "Request for symmetric key" << std::endl;
			}
			else if (resp_payload.Message_Type == "2") // we got someone symmetric key[encoded by public key]
			{
				resp_payload.Content_str = this->connection->read(atoi(msg_size.c_str()));
				resp_payload.Content_str = this->decode_by_private_key(resp_payload.Content_str, resp_payload.Client_ID);// decoding the sym with private key

				for (auto i = this->client_list.begin(); i != this->client_list.end(); i++) 
				{
					if ((*i).Client_ID == resp_payload.Client_ID) 
					{
						strcpy_s((*i).symmetric_key, KEY_BUFFER_LEANGTH, resp_payload.Content_str.c_str());
					}
				}
				std::cout << "Symmertic key received" << std::endl;
			}
			else if (resp_payload.Message_Type == "3") // send txt msg encrepted by my semmetric key
			{
				for (auto i = this->client_list.begin(); i != this->client_list.end(); i++)
				{
					if ((*i).Client_ID == resp_payload.Client_ID)
					{
						if ((*i).symmetric_key == "\0")
						{
							std::cout << "can't decrypt message" << std::endl;
							return false;
						}
					}
				}

				resp_payload.Content_str = this->connection->read(atoi(msg_size.c_str()));
				std::cout << this->decode(resp_payload.Content_str, resp_payload.Client_ID) << std::endl;
			}
			else 
			{
				std::cout << "Error" << std::endl;
				return false;
			}

			std::cout << "----EOM----" << std::endl;
		}

	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	return true;

}
// ######################################################################### section of sending to another client 

bool Request::service_50_type_3_code_1003() // send message to another client
{
	payload_2003 res_payload;
	payload_1003 req_payload;
	std::string msg_content;
	std::string dst_name;
	bool in_list = false;
	int total_payload_size;
	// send msg
	try 
	{
		strcpy_s(this->req_header.Client_ID, UUID_LEANGTH, this->Client_ID);
		strcpy_s(this->req_header.Code, CODE_LEANGTH_BUFFER, "1003");
		strcpy_s(this->req_header.Version, 2, "3");
		//need to add size

		std::cout << "To: ";
		std::cin >> dst_name;

		for (auto i = this->client_list.begin(); i != this->client_list.end(); i++) 
		{
			if ((*i).Client_Name == dst_name.c_str()) 
			{
				in_list = true;
				strcpy_s(req_payload.Client_ID, UUID_LEANGTH, (*i).Client_ID);
			}
		
		}

		if (!in_list) 
		{
			std::cout << " Users doesn;t exists in client list." << std::endl;
			return false;		
		}


		std::cout << "Enter message: ";
		std::cin >> msg_content; // work on it

		msg_content = this->decode(msg_content, req_payload.Client_ID);

		total_payload_size += UUID_LEANGTH + TYPE_MSG_BUFFER_LEANGTH + MSG_SISE_BUFFER + (int)msg_content.length();
		strcpy_s(this->req_header.payload_size, PAYLOAD_SIZE_BUFFER, std::to_string(total_payload_size).c_str());

		if (!this->send_header()) 
		{
			std::cout << "Server responded withan error " << std::endl;
			return false;
		}

		strcpy_s(req_payload.Message_Type, TYPE_MSG_BUFFER_LEANGTH, "3");
		strcpy_s(req_payload.Message_Size, MSG_SISE_BUFFER, std::to_string(msg_content.length()).c_str());
		req_payload.Message_Content_str = msg_content;

		// send the payload(1003)
		this->connection->send(req_payload.Client_ID, UUID_LEANGTH);
		this->connection->send(req_payload.Message_Type, TYPE_MSG_BUFFER_LEANGTH);
		this->connection->send(req_payload.Message_Size, MSG_SISE_BUFFER);
		this->connection->send(req_payload.Message_Content_str.c_str(), (int)msg_content.length());


	
	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	// read response
	response_header_class res_head;
	//payload_2003 res_payload;
	try 
	{
		res_head = this->read_header();

		if (res_head.Code != "2003") 
		{
			std::cout << "Error, server responded with an error." << std::endl;
			return false;
		}

		strcpy_s(res_payload.Client_ID, UUID_LEANGTH, this->connection->read(UUID_LEANGTH).c_str());
		strcpy_s(res_payload.Message_ID, MSG_ID_LEANGTH, this->connection->read(MSG_ID_LEANGTH).c_str());
		std::cout << "Message was sent to <" << res_payload.Client_ID << "> and message id <" << res_payload.Message_ID << ">" << std::endl;


	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	return true;

}


bool Request::service_51_type_1_code_1003() // request getting symmetric key
{
	payload_2003 res_payload;
	payload_1003 req_payload;
	std::string msg_content;
	std::string dst_name;
	bool in_list = false;
	int total_payload_size;
	// send msg
	try
	{
		strcpy_s(this->req_header.Client_ID, UUID_LEANGTH, this->Client_ID);
		strcpy_s(this->req_header.Code, CODE_LEANGTH_BUFFER, "1003");
		strcpy_s(this->req_header.Version, 2, "1");
		//need to add size

		std::cout << "To: ";
		std::cin >> dst_name;

		for (auto i = this->client_list.begin(); i != this->client_list.end(); i++)
		{
			if ((*i).Client_Name == dst_name.c_str())
			{
				in_list = true;
				strcpy_s(req_payload.Client_ID, UUID_LEANGTH, (*i).Client_ID);
			}

		}

		if (!in_list)
		{
			std::cout << " Users doesn;t exists in client list." << std::endl;
			return false;
		}


		total_payload_size += UUID_LEANGTH + TYPE_MSG_BUFFER_LEANGTH + MSG_SISE_BUFFER;
		strcpy_s(this->req_header.payload_size, PAYLOAD_SIZE_BUFFER, std::to_string(total_payload_size).c_str());

		if (!this->send_header())
		{
			std::cout << "Server responded withan error " << std::endl;
			return false;
		}

		strcpy_s(req_payload.Message_Type, TYPE_MSG_BUFFER_LEANGTH, "3");
		strcpy_s(req_payload.Message_Size, MSG_SISE_BUFFER, "0");

		// send the payload(1003)
		this->connection->send(req_payload.Client_ID, UUID_LEANGTH);
		this->connection->send(req_payload.Message_Type, TYPE_MSG_BUFFER_LEANGTH);
		this->connection->send(req_payload.Message_Size, MSG_SISE_BUFFER);


	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	// read response
	response_header_class res_head;
	//payload_2003 res_payload;
	try
	{
		res_head = this->read_header();

		if (res_head.Code != "2003")
		{
			std::cout << "Error, server responded with an error." << std::endl;
			return false;
		}

		strcpy_s(res_payload.Client_ID, UUID_LEANGTH, this->connection->read(UUID_LEANGTH).c_str());
		strcpy_s(res_payload.Message_ID, MSG_ID_LEANGTH, this->connection->read(MSG_ID_LEANGTH).c_str());
		std::cout << "Message was sent to <" << res_payload.Client_ID << "> and message id <" << res_payload.Message_ID << ">" << std::endl;


	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	return true;
}


bool Request::service_52_type_2_code_1003() // sending symmetric key
{
	payload_2003 res_payload;
	payload_1003 req_payload;
	std::string msg_content;
	std::string dst_name;
	bool in_list = false;
	int total_payload_size;
	// send msg
	try
	{
		strcpy_s(this->req_header.Client_ID, UUID_LEANGTH, this->Client_ID);
		strcpy_s(this->req_header.Code, CODE_LEANGTH_BUFFER, "1003");
		strcpy_s(this->req_header.Version, 2, "2");
		//need to add size

		std::cout << "To: ";
		std::cin >> dst_name;

		for (auto i = this->client_list.begin(); i != this->client_list.end(); i++)
		{
			if ((*i).Client_Name == dst_name.c_str())
			{
				in_list = true;
				strcpy_s(req_payload.Client_ID, UUID_LEANGTH, (*i).Client_ID);
				req_payload.Message_Content_str = this->get_symmetric_key();
				strcpy_s((*i).symmetric_key, KEY_BUFFER_LEANGTH, req_payload.Message_Content_str.c_str());

				req_payload.Message_Content_str = this->encode_by_public_key(req_payload.Message_Content_str, req_payload.Client_ID);
			}

		}

		if (!in_list)
		{
			std::cout << " Users doesn;t exists in client list." << std::endl;
			return false;
		}


		
		


		total_payload_size += UUID_LEANGTH + TYPE_MSG_BUFFER_LEANGTH + MSG_SISE_BUFFER + (int)msg_content.length();
		strcpy_s(this->req_header.payload_size, PAYLOAD_SIZE_BUFFER, std::to_string(total_payload_size).c_str());

		if (!this->send_header())
		{
			std::cout << "Server responded withan error " << std::endl;
			return false;
		}

		strcpy_s(req_payload.Message_Type, TYPE_MSG_BUFFER_LEANGTH, "3");
		strcpy_s(req_payload.Message_Size, MSG_SISE_BUFFER, std::to_string(msg_content.length()).c_str());
		req_payload.Message_Content_str = this->encode(msg_content, req_payload.Client_ID);

		// send the payload(1003)
		this->connection->send(req_payload.Client_ID, UUID_LEANGTH);
		this->connection->send(req_payload.Message_Type, TYPE_MSG_BUFFER_LEANGTH);
		this->connection->send(req_payload.Message_Size, MSG_SISE_BUFFER);
		this->connection->send(req_payload.Message_Content_str.c_str(), (int)msg_content.length());



	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	// read response
	response_header_class res_head;
	//payload_2003 res_payload;
	try
	{
		res_head = this->read_header();

		if (res_head.Code != "2003")
		{
			std::cout << "Error, server responded with an error." << std::endl;
			return false;
		}

		strcpy_s(res_payload.Client_ID, UUID_LEANGTH, this->connection->read(UUID_LEANGTH).c_str());
		strcpy_s(res_payload.Message_ID, MSG_ID_LEANGTH, this->connection->read(MSG_ID_LEANGTH).c_str());
		std::cout << "Message was sent to <" << res_payload.Client_ID << "> and message id <" << res_payload.Message_ID << ">" << std::endl;


	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return false;

	}

	return true;
}
