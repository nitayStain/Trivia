#include "JsonResponsePacketSerializer.h"

const Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
	json data = {
		{
			"message", response.message
		}
	};

	return serializeResponse(Responses::ERROR_RESPONSE, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
	json data = { 
		{
			"code", response.status
		} 
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
	json data = {
		{
			"code", response.status
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(LogoutResponse response)
{
	json data = {
		{
			"code", response.status
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse response)
{
	json data = {
		{
			"code", response.status,
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse response)
{
	json data = {
		{
			"code", response.status,
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse response)
{
	json data = {
		{
			"code", response.status,
		},
	};

	for (int i = 0; i < response.rooms.size(); i++)
	{
		auto room = response.rooms[i];
		data["rooms"][i] = {
			{ "name", room.name},
			{ "id", room.id},
			{ "maxPlayers", room.maxPlayers},
			{ "isActive", room.isActive},
		};
	}

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse response)
{
	json data = {
		{
			"code", response.status,
		},
		{
			"players", response.players,
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse response)
{
	json data = {
		{
			"code", response.status,
		},
		{
			"statistics", response.statistics,
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse response)
{
	json data = {
		{
			"code", response.status,
		},
		{
			"statistics", response.statistics,
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse response)
{
	json data = {
		{
			"code", response.status,
		},
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(StartGameResponse response)
{
	json data = {
		{
			"code", response.status,
		},
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse response)
{
	json data = {
		{
			"code", response.status,
		},
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse response)
{
	json data = {
		{
			"code", response.status,
		},
		{
			"hasGameBegun", response.hasGameBegun,
		},
		{
			"players", response.players,
		},
		{
			"questionCount", response.questionCount,
		},
		{
			"answerTimeOut", response.answerTimeOut,
		},
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse response)
{
	json data = {
		{
			"code", response.status,
		},
		{
			"status", response.success,
		},
	};
	for (int i = 0; i < response.results.size(); i++)
	{
		PlayerResults res = response.results[i];
		data["playerResults"][i] = {
			{ "username", res.username },
			{ "correctAnswers", res.correctAnsCount },
			{ "wrongAnswers", res.wrongAnsCount },
			{ "averageAnswerTime", res.avgAnsTime },
		};
	}

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse response)
{
	json data = {
		{
			"code", response.status,
		},
		{
			"correctAnswerId", response.correctAnsId
		}
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse response)
{
	json data = {
		{
			"code", response.status,
		},
		{
			"status", response.success,
		},
		{
			"question", response.question,
		},
		{
			"answers", response.answers,
		},
	};

	return serializeResponse((Responses)response.status, data.dump());
}

const Buffer JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse response)
{
	json data = {
		{
			"code", response.status,
		},
	};

	return serializeResponse((Responses)response.status, data.dump());
}


// Private functions

const Buffer JsonResponsePacketSerializer::serializeResponse(const Responses code, const std::string& json)
{
	Buffer buffer;
	Buffer lengthAsBytes;

	// code will be no longer than 1 byte.
	Utils::appendToBuffer(buffer, (unsigned char)code);
	Utils::appendToBuffer(buffer, (int)json.length());
	Utils::addStringToBuffer(buffer, json); // add payload

	return buffer;
}

