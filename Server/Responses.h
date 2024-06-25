#pragma once

#include <string>
#include <vector>
#include "Room.h"

struct PlayerResults
{
	std::string username;
	unsigned int correctAnsCount;
	unsigned int wrongAnsCount;
	unsigned int avgAnsTime;
};

enum class Responses : unsigned int {
	/* Login-type */
	LOGIN_RESPONSE = 0,
	SIGNUP_RESPONSE,

	/* menu responses */
	CREATE_ROOM_RESPONSE = 10,
	JOIN_ROOM_RESPONSE,
	GET_ROOMS_RESPONSE,
	GET_PLAYERS_IN_ROOM_RESPONSE,
	GET_HIGH_SCORE_RESPONSE,
	GET_PERSONAL_STATS_RESPONSE,
	LOGOUT_RESPONSE,

	/* admin responses */
	CLOSE_ROOM_RESPONSE = 20,
	START_GAME_RESPONSE,
	/* member responses */
	GET_ROOM_STATE_RESPONSE = 25,
	LEAVE_ROOM_RESPONSE,

	/* game responses */
	LEAVE_GAME_RESPONSE = 30,
	SUBMIT_ANSWER_RESPONSE,
	GET_GAME_RESULTS_RESPONSE,
	GET_QUESTION_RESPONSE,

	/* other */
	ERROR_RESPONSE = 99,
};

struct ErrorResponse
{
	std::string message;
};

struct BaseResponse
{
	Responses status;
};

/* login responses */
struct LoginResponse : public BaseResponse {};

struct LogoutResponse : public BaseResponse {};

struct SignupResponse : public BaseResponse {};

/* room responses */

struct CreateRoomResponse : public BaseResponse {};

struct JoinRoomResponse : public BaseResponse {};

struct GetRoomsResponse : public BaseResponse
{
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse : public BaseResponse
{
	std::vector<std::string> players;
};

/* statistics responses */
struct GetHighScoreResponse : public BaseResponse
{
	std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse : public BaseResponse
{
	std::vector<std::string> statistics;
};

/* room admin responses */
struct CloseRoomResponse : public BaseResponse {};

struct StartGameResponse : public BaseResponse {};

/* room member responses */
struct LeaveRoomResponse : public BaseResponse {};

struct GetRoomStateResponse : public BaseResponse
{
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeOut;
};

/* game responses */
struct LeaveGameResponse : public BaseResponse {};

struct SubmitAnswerResponse : public BaseResponse
{
	unsigned int correctAnsId;
};

struct GetGameResultsResponse : public BaseResponse
{
	unsigned int success;
	std::vector<PlayerResults> results;
};

struct GetQuestionResponse : public BaseResponse
{
	unsigned int success;
	std::string question;
	std::map<unsigned int, std::string> answers;
};