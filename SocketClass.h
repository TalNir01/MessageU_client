#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdio>



#define MAX_LENGTH 10

using boost::asio::ip::tcp;

class SocketClass
{
public:
	SocketClass();

	bool send(const char* data_to_send, const int len);
	std::string read(const int len);



private:
	std::string address;
	std::string port;
	
	tcp::socket * socket;
	
	
	void printing_Stuff(int i);

	void read_server_info();

	
};

