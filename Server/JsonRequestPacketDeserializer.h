#pragma once

#include "Requests.h"
#include "buffer.hpp"
#include "json.hpp"

using json = nlohmann::json;

class JsonRequestPacketDeserializer
{	
public:
	// constructor
	JsonRequestPacketDeserializer() = default;

	// destructor
	~JsonRequestPacketDeserializer() = default;

	// deconstructs a string buffer to a login request
	const static LoginRequest deserializeLoginRequest(Buffer buff);

	// deconstructs a string buffer to a signup request
	const static SignupRequest deserializeSignupRequest(Buffer buff);

	// deconstructs a string buffer to a create room request
	const static CreateRoomRequest deserializeCreateRoomRequest(Buffer buff);

	// deconstructs a string buffer to a get players in room request
	const static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(Buffer buff);

	// deconstructs a string buffer to a join room request
	const static JoinRoomRequest deserializeJoinRoomRequest(Buffer buff);

	// deconstructs a string buffer to a submit answer request
	const static SubmitAnswerRequest deserializeSubmitAnswerRequest(Buffer buff);
};