#include "JsonRequestPacketDeserializer.h"

const LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer buff)
{
	auto obj = json::parse(buff);

	LoginRequest req;
	req.password = obj.at("password");
	req.username = obj.at("username");

	return req;
}

const SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer buff)
{
	auto obj = json::parse(buff);

	SignupRequest req;
	req.password = obj.at("password");
	req.username = obj.at("username");
	req.email = obj.at("email");

	return req;
}

const CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(Buffer buff)
{
	auto obj = json::parse(buff);

	CreateRoomRequest req;
	req.roomName = obj.at("roomName");
	req.maxUsers = obj.at("maxUsers");
	req.questionCount = obj.at("questionCount");
	req.answerTimeout = obj.at("answerTimeout");

	return req;
}

const GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(Buffer buff)
{
	auto obj = json::parse(buff);

	GetPlayersInRoomRequest req;
	req.roomId = obj.at("roomId");
	
	return req;
}

const JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(Buffer buff)
{
	return deserializeGetPlayersInRoomRequest(buff);
}

const SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(Buffer buff)
{
	auto obj = json::parse(buff);
	SubmitAnswerRequest req;
	req.answerId = obj.at("answerId");
	return req;
}