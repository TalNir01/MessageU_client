#pragma once
#include <iostream>
#include <boost/asio.hpp>

#define MAX_LENGTH 10

using boost::asio::ip::tcp;

class SocketClass
{
public:
	SocketClass();

private:
	void printing_Stuff(int i);
	
};

