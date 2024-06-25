#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:

	MenuRequestHandler(RequestHandlerFactory& handler, LoggedUser loggedUser);

	// this function check if a request is either login/register or invalid
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;

	// this function handles the request according to the given data and code
	const RequestResult handleRequest(const RequestInfo& requestInfo) override;
private:
	const RequestResult signout(const RequestInfo& requestInfo);
	const RequestResult joinRoom(const RequestInfo& requestInfo);
	const RequestResult createRoom(const RequestInfo& requestInfo);
	const RequestResult getRooms(const RequestInfo& requestInfo);
	const RequestResult getPlayersInRoom(const RequestInfo& requestInfo);
	const RequestResult getPersonalStats(const RequestInfo& requestInfo);
	const RequestResult getHighScore(const RequestInfo& requestInfo);

	RequestHandlerFactory& m_handler;
	RoomManager& m_roomManager;
	LoggedUser m_user;
};

