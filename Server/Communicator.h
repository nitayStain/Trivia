#pragma once

#include "pch.hpp"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "Utils.hpp"
#include "json.hpp"
#include <ctime>

using json = nlohmann::json;

#define ERROR -1

class Communicator
{
public:
	Communicator(RequestHandlerFactory& handler);
	~Communicator();

	void startHandleRequests();

private:
	// fields
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handler;


	// private functions
	void bindAndListen();
	void handleNewClient(SOCKET);
	const Buffer convertToBuffer(const std::string& str) const;
	const std::string convertFromBuffer(const Buffer& buff) const;
};

