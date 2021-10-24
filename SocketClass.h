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

private:
	std::string address;
	std::string port;
	
	/*
	boost::asio::io_context io_context;
	tcp::socket client_sock_conn(io_context);
	tcp::resolver resolver(io_context);
	*/

	tcp::socket* client_sock_conn_pointer;

	void printing_Stuff(int i);

	void read_server_info();

	
};

