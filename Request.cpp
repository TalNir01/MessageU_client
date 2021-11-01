#include "Request.h"

Request::Request(tcp::socket* conn_pointer)
{
	this->req_socket_conn_pointer = conn_pointer;
	strcpy(this->Version, "1");


	std::string reqCode;
	std::cin >> reqCode;
	char tmp[NAME_BUFFER_LEANGTH];


	/*
	// input validity
	if (reqCode.length() > BUFFER_LEANGTH) 
	{
		exit(1);
	}
	for (int i = 0; i < reqCode.length(); i++) 
	{
		if (isdigit(reqCode[i]) == false) 
		{
			exit(1);
		}
	}

	strcpy(tmp, reqCode.c_str());
	this->Code = atoi(tmp);
	*/

	if (!this->getformerloginproprites()) 
	{
		//pass		 
	}
	 
	
	


}


bool Request::send_header()
{
	try 
	{

		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(this->Client_ID, 16));
		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(this->Version, 1));
		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(this->Code, 2));
		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(this->Payload_str_size, 4));

		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << e.what() << std::endl;
		return false;
	}

	
}

char * Request::ret_client_id(std::string name)
{
	const char id[17] = "0000000000000000";


	return NULL;
}

bool Request::getformerloginproprites()
{
	std::string str_uuid;
	std::string private_key_str;
	std::ifstream inf{ "me.info" };
	if (!inf) 
	{
		std::cout << "[LOG] User dont exsits [couldn't find me.info] " << std::endl;
		this->old_user = false;
		return false;
	}
	
	std::getline(inf, this->str_client_name);
	std::strcpy(this->client_name, this->str_client_name.c_str());

	std::getline(inf, str_uuid);
	std::strcpy(this->Client_ID, str_uuid.c_str());

	std::getline(inf, private_key_str);
	std::strcpy(this->private_key, private_key_str.c_str());


	this->old_user = true;
	return true;

}




bool Request::service_10_code_1000() // service 10 regestration
{
	if (this->old_user) 
	{
		std::cout << "User already exsits, can't register again. " << std::endl;
		return false;
	}

	
	try 
	{

		std::strcpy(this->Code, "1000");

		std::strcpy(this->Client_ID, "0000000000000000");

		std::strcpy(this->private_key, this->get_private_key());

		std::cout << "Enter your user name: ";

		std::cin.getline(this->client_name, NAME_BUFFER_LEANGTH);
		std::cout << "" << std::endl;

		std::cout << "Send the request" << std::endl;

		// Payload size
		
		this->Payload_size = 256 + 160;
		std::string load_size_tmp = std::to_string(this->Payload_size);
		//char Payload_str_size[4];
		std::strcpy(this->Payload_str_size, load_size_tmp.c_str());
		
		if (!this->send_header())
		{
			return false;
		}
		
		char payload[416];
		std::strcat(payload, this->client_name);
		std::strcat(payload, this->public_key);

		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(payload, 416));

	
	}
	catch (std::exception& e) 
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << e.what() << std::endl;
		return false;
	}


	try // 2000 response code
	{
		char serverV[2];
		char response_code[3];
		char response_payload_size[5];
		size_t leng;
		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(serverV, 1));
		

		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(response_code, 2));

		if (response_code != "2000") 
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

		char *res_payload;

		res_payload = (char*)malloc(resleng*sizeof(char));

		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(res_payload, resleng));
		std::strcpy(this->Client_ID, res_payload);

		free(res_payload);

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
		
		this->old_user = true;

	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Erro is " << e.what() << std::endl;
		return true;
	}
	
	return true;

}


bool Request::service_20_code_1001() // ask for client list
{
	if (!this->old_user) 
	{
		std::cout << "Error, no registration " << std::endl;
		return false;
	}

	try 
	{
		
		std::strcpy(this->Code, "1001");
		std::strcpy(this->Payload_str_size, "0");
		
		if (!this->send_header()) // sending the Request header
		{
			return false;
		}
		
	}
	catch (std::exception& e) 
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return true;
	
	}
	// read the response
	try 
	{
		char serverV[2];
		char response_code[3];
		char response_payload_size[5];
		size_t leng;
		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(serverV, 1));


		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(response_code, 2));

		if (response_code != "2001")
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

		int num_of_clients = resleng / (16 + 255);

		char* res_payload;

		res_payload = (char*)malloc(resleng * sizeof(char));

		leng = boost::asio::read(*(this->req_socket_conn_pointer), boost::asio::buffer(res_payload, resleng));
		std::string decoded_payload = get_decode_payload(res_payload);
		
		std::cout << "Client list. format [uuid:name]" << std::endl;

		for (int i = 0; i < num_of_clients; i++) 
		{
			std::cout << "[" << decoded_payload.substr(i, i + 16) << ":" << decoded_payload.substr(i + 16, i + 255) << "]" << std::endl;
			
			this->client_list[decoded_payload.substr(i, i + 16)] = decoded_payload.substr(i + 16, i + 255);
			
			//.insert(pair<std::string, std::string>(decoded_payload.substr(i, i + 16), decoded_payload.substr(i + 16, i + 255)));
			i += (255 + 16);
		}

		free(res_payload);

		return true;

	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Error is " << e.what() << std::endl;
		return true;

	}

}




bool Request::service_30_code_1002() // ask for client(myself) public key
{
	if (!this->old_user)
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << std::endl;
		return false;
	}

	try 
	{
		std::strcpy(this->Code, "1002");

		std::strcpy(this->Payload_str_size, "16");

		if (!this->send_header()) 
		{
			std::cerr << "[LOG] Error at sending section -> soket error: " << std::endl;
			return false;
		}

		char payload[16];
		std::strcpy(payload, this->Client_ID);

		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(payload, 16));

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
		std::strcpy_s(this->Code, 5, "1004");

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
