#pragma once
#include "RequestHandlerFactory.h"
#include "Responses.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	// constructor
	RoomMemberRequestHandler(const std::string& roomId, const LoggedUser& user, RequestHandlerFactory& handler, RoomManager& roomManager);

	// methods
	bool isRequestRelevant(const RequestInfo& reqInfo) const override; // checks if the request is to leave the room or get its state
	const RequestResult handleRequest(const RequestInfo& reqInfo) override; // handles the relevant request

private:
	// fields
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handler;

	// methods
	const RequestResult leaveRoom(const RequestInfo& reqInfo);
	const RequestResult getRoomState(const RequestInfo& reqInfo);
};