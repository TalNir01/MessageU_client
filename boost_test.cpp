/*
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#define MAX_LENGTH 10

using boost::asio::ip::tcp;

void clear(char message[], int length) {
	for (int i = 0; i < length; i++)
		message[i] = '\0';
}

int main(int argc, char* argv[])
{
	const int max_length = 1024;
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
			return 1;
		}
		
		boost::asio::io_context io_context;
		tcp::socket s(io_context);
		tcp::resolver resolver(io_context);
		boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));
		for (; ; ) {
			std::cout << "Enter message: ";
			char request[max_length];
			clear(request, max_length);
			std::cin.getline(request, max_length);
			boost::asio::write(s, boost::asio::buffer(request, max_length));
			char reply[max_length];
			size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, max_length));
			std::cout << "Reply is: " << reply << "\n";
		}
	}
		catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}*/