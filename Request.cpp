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
		std::cout << "[LOG] the user is OLD. We have your data." << std::endl;
	}
	
}

/*	Sending the Header */
bool Request::send_header()
{
	try 
	{
		this->connection->send(this->req_header.Client_ID, 16);
		this->connection->send(this->req_header.Version, 1);
		this->connection->send(this->req_header.Code, 2);
		this->connection->send(this->req_header.payload_size, 4);
		
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
	size_t lang;
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
		size_t leng;
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
	payload_2001 response_payload;
	size_t leng;
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
	payload_2002 response_payload;
	payload_1002 request_payload;
	std::string name_str;
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
				if(*i.Client_Name == name_str)
			
			}
		}

		char payload[16];
		strcpy_s(request_payload.Client_ID, UUID_LEANGTH, tmp_id);

		this->connection->send(request_payload.Client_ID, 16);

		// read response
		try 
		{
			char serverV[2];
			char response_code[3];
			char response_payload_size[5];
			size_t leng;
			leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(serverV, 1));


			leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(response_code, 2));

			if (response_code != "2002")
			{
				std::cout << "server responded with an error " << std::endl;
				return false;

			}

			leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(response_payload_size, 4));

			const int resleng = atoi(response_payload_size);
			if (resleng == 0)
			{
				if (response_payload_size != "0")
					return false;
			}
		
			char* res_payload = (char*)malloc(resleng * sizeof(char));

			leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(res_payload, resleng));

			if (res_payload == NULL) 
			{
				std::cout << "server responded with an error " << std::endl;
				return false;
			}

			for (int i = 16; i < PUBLIC_KEY_BUFFER_LEANGTH; i++) 
			{
				this->public_key[i - 16] = res_payload[i];
			}

			free(res_payload);
			std::cout << "private key is: " << this->private_key << std::endl;

			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error : Error is " << e.what() << std::endl;
			return true;

		}



	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return true;

	}

}




bool Request::service_40_code_1004() // ask for message list
{
	if (!this->old_user)
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << std::endl;
		return false;
	}

	try
	{
		std::strcpy(this->Code, "1004");

		std::strcpy(this->Payload_str_size, "0");

		if (!this->send_header())
		{
			std::cerr << "[LOG] Error at sending section -> soket error: " << std::endl;
			return false;
		}

		
		// read response (code: 2004)
		char serverV[2];
		char response_code[3];
		char response_payload_size[5];
		size_t leng;
		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(serverV, 1));


		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(response_code, 2));

		if (response_code != "2004")
		{
			std::cout << "server responded with an error " << std::endl;
			return false;

		}

		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(response_payload_size, 4));

		const int resleng = atoi(response_payload_size);
		if (resleng == 0)
		{
			if (response_payload_size != "0")
				return false;
		}

		int num_of_msg = resleng / (16 + 255);

		char* res_payload;

		res_payload = (char*)malloc(resleng * sizeof(char));

		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(res_payload, resleng));
		std::string decoded_payload = get_decode_payload(res_payload);

		// print message and stuff.
		// ##########################################################################
		// ##########################################################################



		/*
		for (int i = 0; i < num_of_clients; i++)
		{
			std::cout << decoded_payload.substr(i, i + 16) << ":" << decoded_payload.substr(i + 16, i + 255) << std::endl;
			i += (255 + 16);
		}
		*/
		free(res_payload);

		return true;



	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return true;

	}

}



bool Request::service_50_code_1003(int type) // send message. 50 - regular msg; 51 - ask for symmetric key; 52 - send symmetric key
{

}


bool Request::service_50_type_3() // send message to another client
{

}


bool Request::service_51_type_2() // request getting symmetric key
{

}


bool Request::service_52_type_1() // request sending symmetric key
{

}
