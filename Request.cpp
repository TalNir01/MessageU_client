#include "Request.h"

Request::Request()
{
	std::string reqCode;
	std::cin >> reqCode;
	char tmp[BUFFER_LEANGTH];

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



}


bool Request::getformerloginproprites()
{
	std::string str_uuid;
	std::string private_key;
	std::ifstream inf{ "me.info" };
	if (!inf) 
	{
		std::cout << "[LOG] already exsits user " << std::endl;
		this->old_user = false;
		return false;
	}
	
	std::getline(inf, this->str_client_name);
	std::getline(inf, str_uuid);
	std::getline(inf, private_key);

	this->old_user = true;
	return false;

}