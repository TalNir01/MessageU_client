#include "Request.h"

Request::Request(tcp::socket* conn_pointer)
{
	this->req_socket_conn_pointer = conn_pointer;
	strcpy(this->Version, "1");


	std::string reqCode;
	std::cin >> reqCode;
	char tmp[NAME_BUFFER_LEANGTH];

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
	/*
	* if (!this->getformerloginproprites()) 
	{
		//new user
		get 
	}
	* 
	*/
	


}


bool Request::getformerloginproprites()
{
	std::string str_uuid;
	std::string private_key;
	std::ifstream inf{ "me.info" };
	if (!inf) 
	{
		std::cout << "[LOG] User dont exsits [couldn't find me.info] " << std::endl;
		this->old_user = false;
		return false;
	}
	
	std::getline(inf, this->str_client_name);
	std::getline(inf, str_uuid);
	std::getline(inf, private_key);

	this->old_user = true;
	return true;

}


bool Request::setloginproprties() 
{



}


bool Request::service_10_code_1000() // service 10
{
	if (this->old_user) 
	{
		std::cout << "User already exsits, cant register. " << std::endl;
		return false;
	}

	strcpy(this->Code, "1000");

	strcpy(this->Client_ID, "0000000000000000");

	strcpy(this->private_key, this->get_private_key());

	std::cout << "Enter your user name: ";

	std::cin.getline(this->client_name, NAME_BUFFER_LEANGTH);
	std::cout << "" << std::endl;

	std::cout << "Send the request" << std::endl;

	try 
	{
		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(this->Client_ID, 16));
		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(this->Version, 1));
		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(this->Code, 2));
		// Payload size
		this->Payload_size = 256 + 160;
		std::string load_size_tmp = std::to_string(this->Payload_size);
		char Payload_str_size[4];
		strcpy(Payload_str_size, load_size_tmp.c_str());

		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(Payload_str_size, 4));
		char payload[416];
		strcat(payload, this->client_name);
		strcat(payload, this->public_key);

		boost::asio::write(*(this->req_socket_conn_pointer), boost::asio::buffer(payload, 416));

	
	}
	catch (std::exception& e) 
	{
		std::cerr << "[LOG] Error at sending section -> soket error: " << e.what() << std::endl;
		return false;
	}


	try 
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
		strcpy(this->Client_ID, res_payload);

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


	}
	catch (std::exception& e)
	{
		std::cerr << "Error : Erro is " << e.what() << std::endl;
	}
	
	return true;

}