#ifndef COMMON_H
#define COMMON_H

std::string messageReceived;

void newMessageReceived(std::string message)
{
	messageReceived = message;
}

#endif
