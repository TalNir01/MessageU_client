#include <boost/asio.hpp> 
#include <iostream> 
using boost::asio::ip::tcp;

int main() 
{
	int port = 1234;
	boost::asio::io_context io_context;
	tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
	tcp::socket sock = a.accept();

	char data[max_Length];
	size_t length = boost::asio::read(sock, boost::asio::buffer(data, max_length));


	return 0;
}
