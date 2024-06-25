#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "GameManager.h"
#include "StatisticsManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* db);
	~RequestHandlerFactory();

	// returns m_loginManager
	LoginManager& getLoginManager();

	// returns the room manager
	RoomManager& getRoomManager();

	// returns the stats manager
	StatisticsManager& getStatisticsManager();

	// returns the game manager
	GameManager& getGameManager();

	// creates a new pointer to a login request handler
	LoginRequestHandler* createLoginRequestHandler();

	// creates a new menu request handler
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);

	// creates a new room admin request handler
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, const std::string& roomId);

	// 
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, const std::string& roomId);

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_statManager;
	GameManager m_gameManager;
};
