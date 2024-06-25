#include "Server.h"
#include "uuid.h"

#define CMD_PREFIX '!'

Server::Server(IDatabase* db) : m_database(db), m_handler(RequestHandlerFactory(db)), m_communicator(new Communicator(m_handler))
{
}


void Server::run() {
	// run communicator thread - the function that handles the server system
	std::thread communicate(&Communicator::startHandleRequests, m_communicator);
	communicate.detach();
	
	LoggedUser Owner;
	Owner.username = "Admin";
	RoomManager& rm = m_handler.getRoomManager();

	std::string cmd = "";
	while (cmd != "EXIT") {
		std::getline(std::cin, cmd);
		
		if (cmd[0] != CMD_PREFIX) continue;
		std::string command = cmd.substr(1, cmd.find(" ") - 1);
		std::string args = cmd.substr(command.size() + 2, cmd.size()); // + 2 to clear spaces

		if (command == "addquestion")
		{
			std::stringstream ss(args);
			std::string question, correctAnswer, answer_1, answer_2, answer_3;

			std::getline(ss, question, '|');
			std::getline(ss, correctAnswer, '|');
			std::getline(ss, answer_1, '|');
			std::getline(ss, answer_2, '|');
			std::getline(ss, answer_3, '|');

			m_database->addQuestion(question, correctAnswer, answer_1, answer_2, answer_3);
		}
		// for debugging
		if (command == "createroom")
		{
			std::stringstream ss(args);
			std::string name;
			std::getline(ss, name, '|');

			RoomData room;
			room.id = uuidV4::UUID::New().String();
			room.isActive = true;
			room.maxPlayers = 12;
			room.name = name;
			room.numOfQuestionsInGame = 2;
			room.timePerQuestion = 10;

			rm.createRoom(Owner, room);
			rm.getRooms();
		}
	}

	std::cout << "[SERVER] Exiting . . ." << std::endl;
}