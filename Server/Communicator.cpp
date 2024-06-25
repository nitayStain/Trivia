#include "Communicator.h"

Communicator::Communicator(RequestHandlerFactory& handler) : m_handler(handler)
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - Couldn't init socket.");
}

Communicator::~Communicator()
{
	try {
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(m_serverSocket);

		// clear request handlers memory
		for (auto& c : m_clients) {
			delete c.second;
		}
	}
	catch (...) {}
}

void Communicator::startHandleRequests()
{
	// calling this function so i can bind and create the socket before i accept clients
	bindAndListen();

	// infinite loop to catch clients - i just used the code from class 13
	while (true) {
		SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__ " - Couldn't connect socket");

		std::cout << "[SERVER] Client Accepted." << std::endl;

		// create the client with the first task of authentication
		m_clients[client_socket] = m_handler.createLoginRequestHandler();

		// create a special thread to communicate with him
		std::thread worker(&Communicator::handleNewClient, this, client_socket);
		worker.detach();
	}
}

void Communicator::bindAndListen()
{
	// basically u don't need to understand, i grabbed the code from class 13
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - couldn't bind server to port " + PORT);

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - couldn't listen on port " + PORT);

	std::cout << "[SERVER] Listening on port " << PORT << std::endl;
}

void Communicator::handleNewClient(SOCKET client)
{
	while (1) {
		try {
			auto clientHandler = m_clients.find(client)->second;

			if (!clientHandler || client == INVALID_SOCKET) break;

			auto code = (RequestCode) Utils::getFromSocket<char>(client);
			auto length = Utils::getFromSocket<int>(client);
			auto payload = Utils::getBytes(client, length);

			RequestInfo req = {
				code,
				time_t(NULL),
				payload
			};

			std::cout << "[SERVER] Capturing request [Code: " << (int)code << ", Length: " << length << ", ID:" << client << "]" << std::endl;

			if (code == RequestCode::EXIT)
				break;

			if (clientHandler && clientHandler->isRequestRelevant(req)) {
				auto res = clientHandler->handleRequest(req);
				
				if (res.newHandler != nullptr)
				{
					m_clients.insert_or_assign(client, res.newHandler);
					delete clientHandler;
				}

				if (Utils::SendData(client, res.response)) {
					std::cout << "[SERVER] Sent response to client [ID:" << client << "]" << std::endl;
				}
			}
			else {
				ErrorResponse resp = { "Irrelevant Request." };
				auto payload = JsonResponsePacketSerializer::serializeResponse(resp);

				Utils::SendData(client, payload);
				std::cout << "[SERVER] Sent error response to client [ID:" << client << ", Req: " << (int)code << "]" << std::endl;
			}
		}
		catch (std::exception& e) {
			std::cerr << "[ERROR] " << e.what() << std::endl;
			break;
		}
	}
	std::cout << "[SERVER] Client leaving . . . [ID: " << client << "]" << std::endl;

	m_clients.erase(client);
	closesocket(client);
}

const Buffer Communicator::convertToBuffer(const std::string& str) const
{
	Buffer buff;
	for (int i = 0; i < str.length(); i++)
	{
		buff.push_back(str[i]);
	}
	return buff;
}

const std::string Communicator::convertFromBuffer(const Buffer& buff) const
{
	std::string str;
	for (int i = 0; i < buff.size(); i++)
	{
		str += buff[i];
	}
	return str;
}