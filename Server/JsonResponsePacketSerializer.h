#pragma once

#include "Responses.h"
#include "Utils.hpp"
#include "json.hpp"
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

#define LENGTH_BYTES 4

class JsonResponsePacketSerializer
{
public:
	// constructor
	JsonResponsePacketSerializer() = default;

	// destructor
	~JsonResponsePacketSerializer() = default;

	const static Buffer serializeResponse(ErrorResponse response);
	
	// login-type response serializers
	const static Buffer serializeResponse(LoginResponse response);
	const static Buffer serializeResponse(SignupResponse response);
	
	// menu response serializers
	const static Buffer serializeResponse(LogoutResponse response);
	const static Buffer serializeResponse(CreateRoomResponse response);
	const static Buffer serializeResponse(JoinRoomResponse response);
	const static Buffer serializeResponse(GetRoomsResponse response);
	const static Buffer serializeResponse(GetPlayersInRoomResponse response);
	const static Buffer serializeResponse(GetHighScoreResponse response);
	const static Buffer serializeResponse(GetPersonalStatsResponse response);

	// room admin response serializers
	const static Buffer serializeResponse(CloseRoomResponse response);
	const static Buffer serializeResponse(StartGameResponse response);
	// room member response serializers
	const static Buffer serializeResponse(LeaveRoomResponse response);
	const static Buffer serializeResponse(GetRoomStateResponse response);

	// game response serializers
	const static Buffer serializeResponse(GetGameResultsResponse response);
	const static Buffer serializeResponse(SubmitAnswerResponse response);
	const static Buffer serializeResponse(GetQuestionResponse response);
	const static Buffer serializeResponse(LeaveGameResponse response);

private:
	// help methods
	const static Buffer serializeResponse(const Responses code, const std::string& json); // a global function that returns a buffer with the format of: code (1 byte) length (4 bytes) json (length bytes)
};