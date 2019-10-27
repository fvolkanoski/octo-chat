#ifndef ASIOINIT_H
#define ASIOINIT_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"

#include "common.h"

const short multicast_port = 30001;

class receiver
{
public:
	receiver(boost::asio::io_service& io_service, const boost::asio::ip::address& listen_address, const boost::asio::ip::address& multicast_address) : socket_(io_service)
	{
		// Create the socket so that multiple may be bound to the same address.
		boost::asio::ip::udp::endpoint listen_endpoint(
			listen_address, multicast_port);
		socket_.open(listen_endpoint.protocol());
		socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
		socket_.bind(listen_endpoint);

		// Join the multicast group.
		socket_.set_option(
			boost::asio::ip::multicast::join_group(multicast_address));

		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length), sender_endpoint_,
			boost::bind(&receiver::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void handle_receive_from(const boost::system::error_code& error,
		size_t bytes_recvd)
	{
		if (!error)
		{
			std::cout << "new transmission from houston: " << std::endl;
			std::cout.write(data_, bytes_recvd);
			std::cout << std::endl;
			newMessageReceived(std::string(data_));

			socket_.async_receive_from(
				boost::asio::buffer(data_, max_length), sender_endpoint_,
				boost::bind(&receiver::handle_receive_from, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
	}

private:
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	enum { max_length = 1024 };
	char data_[max_length];
};

void runOctoClient(char* clientIp[])
{
	boost::asio::io_service io_service;
	receiver r(io_service,
		boost::asio::ip::address::from_string(clientIp[1]),
		boost::asio::ip::address::from_string(clientIp[2]));
	io_service.run();
}

#endif