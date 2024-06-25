#pragma once

#include "Communicator.h"
#include "IDatabase.h"

class Server
{
public:
	Server(IDatabase* db);
	
	void run();
private:
	IDatabase* m_database;
	
	RequestHandlerFactory m_handler;
	Communicator* m_communicator;
};

