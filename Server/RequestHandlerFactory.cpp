#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) : m_database(db),
m_loginManager(LoginManager(m_database)),
m_statManager(StatisticsManager(m_database)),
m_roomManager(RoomManager()),
m_gameManager(GameManager(m_database))
{
}

RequestHandlerFactory::~RequestHandlerFactory()
{
    delete m_database;
    m_database = nullptr;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return m_roomManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return m_statManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return m_gameManager;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(*this, user);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, const std::string& roomId)
{
    return new RoomAdminRequestHandler(roomId, *this, user, m_roomManager);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, const std::string& roomId)
{
    return new RoomMemberRequestHandler(roomId, user, *this, m_roomManager);
}