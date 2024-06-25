#pragma once
#include "RoomMemberRequestHandler.h"
#include <memory>

class RequestHandlerFactory;
class RoomMemberRequestHandler;

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	// constructor
	RoomAdminRequestHandler(const std::string& roomId, RequestHandlerFactory& handler, const LoggedUser& user, RoomManager& roomManager);
	
	// methods
	bool isRequestRelevant(const RequestInfo& reqInfo) const override; // checks if the request is for closing a room, starting a game or getting a room's state
	const RequestResult handleRequest(const RequestInfo& reqInfo) override; // handles relevant request

private:
	// fields
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// methods
	const RequestResult closeRoom(const RequestInfo& reqInfo); // closes the room
	const RequestResult startGame(const RequestInfo& reqInfo); // starts the game in the room
	const RequestResult getRoomState(const RequestInfo& reqInfo); // returns the state of the room
};