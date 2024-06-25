#include "MenuRequestHandler.h"
#include "JsonResponsePacketserializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "uuid.h"
#include "Exceptions.h"
#include <algorithm>

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handler, LoggedUser loggedUser) : m_handler(handler), m_user(loggedUser), m_roomManager(handler.getRoomManager())
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
    return requestInfo.id >= RequestCode::CREATE_ROOM_REQUEST && requestInfo.id <= RequestCode::LOGOUT_REQUEST;
}

const RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    try
    {
        switch (requestInfo.id)
        {
        case RequestCode::CREATE_ROOM_REQUEST:
            return createRoom(requestInfo);
            break;
        case RequestCode::JOIN_ROOM_REQUEST:
            return joinRoom(requestInfo);
            break;
        case RequestCode::GET_ROOMS_REQUEST:
            return getRooms(requestInfo);
            break;
        case RequestCode::GET_PLAYERS_IN_ROOM_REQUEST:
            return getPlayersInRoom(requestInfo);
            break;
        case RequestCode::GET_HIGH_SCORE_REQUEST:
            return getHighScore(requestInfo);
            break;
        case RequestCode::GET_PERSONAL_STATISTICS_REQUEST:
            return getPersonalStats(requestInfo);
            break;
        case RequestCode::LOGOUT_REQUEST:
            return signout(requestInfo);
            break;
        default:
            break;
        }
    }
    catch (Exceptions::TriviaException& e) {
        ErrorResponse err = { e.what() };
        return {
            JsonResponsePacketSerializer::serializeResponse(err),
            nullptr
        };
    }
    catch (json::exception& e) {
        ErrorResponse err = { "A serialization error occurred." };
        return {
            JsonResponsePacketSerializer::serializeResponse(err),
            nullptr
        };
    }

}

const RequestResult MenuRequestHandler::signout(const RequestInfo& requestInfo)
{
    auto loginManager = m_handler.getLoginManager();
    
    loginManager.logout(m_user.username);

    LogoutResponse resp;
    resp.status = Responses::LOGOUT_RESPONSE;

    return {
        JsonResponsePacketSerializer::serializeResponse(resp),
        nullptr,
    };
}

const RequestResult MenuRequestHandler::joinRoom(const RequestInfo& requestInfo)
{
    auto payload = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.buffer);

    Room& room = m_roomManager.getRoom(payload.roomId);
    if (room.getRoomData().isActive)
    {
        throw Exceptions::TriviaException("This room is currently ingame!");
    }

    if(room.getAllUsers().size() > room.getRoomData().maxPlayers)
        throw Exceptions::TriviaException("This room is full!");

    m_roomManager.getRoom(payload.roomId).addUser(m_user);

    JoinRoomResponse resp;
    resp.status = Responses::JOIN_ROOM_RESPONSE;

    return {
        JsonResponsePacketSerializer::serializeResponse(resp),
        m_handler.createRoomMemberRequestHandler(m_user, payload.roomId),
    };
}

const RequestResult MenuRequestHandler::createRoom(const RequestInfo& requestInfo)
{
    auto payload = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.buffer);

    RoomData metadata { uuidV4::UUID::New().String(), payload.roomName, payload.maxUsers, payload.questionCount, payload.answerTimeout, false };

    m_roomManager.createRoom(m_user, metadata);

    CreateRoomResponse resp;
    resp.status = Responses::CREATE_ROOM_RESPONSE;

    std::string id = metadata.id;
    
    return {
        JsonResponsePacketSerializer::serializeResponse(resp),
         m_handler.createRoomAdminRequestHandler(m_user, id),
    };
}


const RequestResult MenuRequestHandler::getRooms(const RequestInfo& requestInfo)
{
    auto rooms = m_roomManager.getRooms();

    GetRoomsResponse resp;
    resp.status = Responses::GET_ROOMS_RESPONSE;
    resp.rooms = rooms;

    // get only joinable rooms (the game didn't start)
    rooms.erase(std::remove_if(rooms.begin(), rooms.end(), [](RoomData& r) {
        return r.isActive;
       }), rooms.end());

    return {
        JsonResponsePacketSerializer::serializeResponse(resp),
        nullptr,
    };
}

const RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo)
{
    auto payload = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(requestInfo.buffer);
    
    Room& room = m_roomManager.getRoom(payload.roomId);
    GetPlayersInRoomResponse resp;
    resp.status = Responses::GET_PLAYERS_IN_ROOM_RESPONSE;
    resp.players = room.getAllUsers();

    return {
        JsonResponsePacketSerializer::serializeResponse(resp),
        nullptr,
    };
}

const RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& requestInfo)
{
    auto statManager = m_handler.getStatisticsManager();
    auto stats = statManager.getUserStatistics(m_user.username);

    GetPersonalStatsResponse resp;
    resp.status = Responses::GET_PERSONAL_STATS_RESPONSE;
    resp.statistics = stats;

    return {
        JsonResponsePacketSerializer::serializeResponse(resp),
        nullptr,
    };
}

const RequestResult MenuRequestHandler::getHighScore(const RequestInfo& requestInfo)
{
    auto statManager = m_handler.getStatisticsManager();
    auto stats = statManager.getHighScores();

    GetHighScoreResponse resp;
    resp.status = Responses::GET_HIGH_SCORE_RESPONSE;
    resp.statistics = stats;

    return {
        JsonResponsePacketSerializer::serializeResponse(resp),
        nullptr,
    };
}