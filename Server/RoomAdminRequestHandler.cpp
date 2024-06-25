#include "RoomAdminRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "Exceptions.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const std::string& roomId, RequestHandlerFactory& handler, const LoggedUser& user, RoomManager& roomManager) :
	m_handlerFactory(handler), m_user(user), m_roomManager(roomManager), m_room(roomManager.getRoom(roomId))
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& reqInfo) const
{
	return reqInfo.id >= RequestCode::CLOSE_ROOM_REQUEST && reqInfo.id <= RequestCode::GET_ROOM_STATE_REQUEST;
}

const RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	try
	{
		switch (reqInfo.id)
		{
		case RequestCode::CLOSE_ROOM_REQUEST:
			return closeRoom(reqInfo);
			break;
		case RequestCode::START_GAME_REQUEST:
			return startGame(reqInfo);
			break;
		case RequestCode::GET_ROOM_STATE_REQUEST:
			return getRoomState(reqInfo);
			break;
		}
	}
	catch (Exceptions::TriviaException& e)
	{
		ErrorResponse err = { e.what() };
		return { JsonResponsePacketSerializer::serializeResponse(err), nullptr };
	}
	catch (json::exception& e)
	{
		ErrorResponse err = { "A serialization error occurred." };
		return { JsonResponsePacketSerializer::serializeResponse(err), nullptr };
	}
}

const RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& reqInfo)
{
	for (auto username : m_room.getAllUsers())
	{
		LoggedUser user{ username };
		m_room.removeUser(user);
	}

	m_roomManager.deleteRoom(m_room.getRoomData().id);
	CloseRoomResponse resp;
	resp.status = Responses::CLOSE_ROOM_RESPONSE;

	return { JsonResponsePacketSerializer::serializeResponse(resp), nullptr };
}

const RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& reqInfo)
{
	RoomData data = m_room.getRoomData();
	RoomData newData = { data.id, data.name, data.maxPlayers, data.numOfQuestionsInGame, data.timePerQuestion, ROOM_ACTIVE };
	m_room.setRoomData(newData);
	StartGameResponse resp;
	resp.status = Responses::START_GAME_RESPONSE;

	return { JsonResponsePacketSerializer::serializeResponse(resp), nullptr };
}

const RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& reqInfo)
{
	RoomData data = m_room.getRoomData();
	GetRoomStateResponse resp;
	resp.status = Responses::GET_ROOM_STATE_RESPONSE;
	resp.questionCount = data.numOfQuestionsInGame;
	resp.answerTimeOut = data.timePerQuestion;
	resp.hasGameBegun = data.isActive;
	resp.players = m_room.getAllUsers();


	return { JsonResponsePacketSerializer::serializeResponse(resp), nullptr };
}