#ifndef ASIOINIT_H
#define ASIOINIT_H

#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>

constexpr short multicast_port = 30001;
constexpr int max_message_count = 90;

class sender
{
public:
	sender(boost::asio::io_context& io_context, const boost::asio::ip::address& multicast_address) : endpoint_(multicast_address, multicast_port), socket_(io_context, endpoint_.protocol()) {};

	void send_message(std::string msg)
	{
		this->do_send(msg);
	}

private:
	void do_send(std::string message)
	{
		std::ostringstream os;
		os << message;
		message_ = os.str();

		socket_.async_send_to(boost::asio::buffer(message_), endpoint_, [this](boost::system::error_code ec, std::size_t /*length*/) {});
	}

private:
	boost::asio::ip::udp::endpoint endpoint_;
	boost::asio::ip::udp::socket socket_;
	std::string message_;
};

void runOctoServer(char* serverIp[])
{
	boost::asio::io_context io_context;
	sender s(io_context, boost::asio::ip::make_address(serverIp[1]));
	io_context.run();
}

#endif