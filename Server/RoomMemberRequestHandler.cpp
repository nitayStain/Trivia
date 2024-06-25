#include "RoomMemberRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "Exceptions.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const std::string& roomId, const LoggedUser& user, RequestHandlerFactory& handler, RoomManager& roomManager) :
	m_user(user), m_roomManager(roomManager), m_handler(handler), m_room(roomManager.getRoom(roomId))
{
	std::cout << m_room.getAllUsers().size() << " " << roomManager.getRoom(roomId).getAllUsers().size() << std::endl;
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& reqInfo) const
{
	return reqInfo.id == RequestCode::LEAVE_ROOM_REQUEST || reqInfo.id == RequestCode::GET_ROOM_STATE_REQUEST;
}

const RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	try
	{
		return reqInfo.id == RequestCode::LEAVE_ROOM_REQUEST ? leaveRoom(reqInfo) : getRoomState(reqInfo);
	}
	catch (Exceptions::TriviaException& e) {
		ErrorResponse err = { e.what() };
		return { JsonResponsePacketSerializer::serializeResponse(err), nullptr };
	}
	catch (json::exception& e) {
		ErrorResponse err = { "A serialization error occurred." };
		return { JsonResponsePacketSerializer::serializeResponse(err), nullptr };
	}
}

const RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& reqInfo)
{
	m_room.removeUser(m_user);
	LeaveRoomResponse resp;
	resp.status = Responses::LEAVE_ROOM_RESPONSE;

	return { JsonResponsePacketSerializer::serializeResponse(resp), nullptr };
}

const RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& reqInfo)
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
