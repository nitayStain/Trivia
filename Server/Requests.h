#pragma once

#include <string>
#include <ctime>
#include "buffer.hpp"
#include "IRequestHandler.h"

class IRequestHandler;

using string = std::string;

enum class RequestCode { 
	/* login requests */
	LOGIN_REQUEST = 0,
	SIGNUP_REQUEST,

	/* room requests */
	CREATE_ROOM_REQUEST = 10,
	GET_PLAYERS_IN_ROOM_REQUEST,
	JOIN_ROOM_REQUEST,
	GET_HIGH_SCORE_REQUEST,
	GET_PERSONAL_STATISTICS_REQUEST,
	GET_ROOMS_REQUEST,
	LOGOUT_REQUEST,

	/* admin requests */
	CLOSE_ROOM_REQUEST = 20,
	START_GAME_REQUEST,
	/* member requests */
	GET_ROOM_STATE_REQUEST = 25,
	LEAVE_ROOM_REQUEST,

	/* game requests */
	LEAVE_GAME_REQUEST = 30,
	SUBMIT_ANSWER_REQUEST,
	GET_GAME_RESULTS_REQUEST,
	GET_QUESTION_REQUEST,

	/* other */
	EXIT = 99
};

/* request payloads */
struct LoginRequest 
{
	string username, password;
};

struct SignupRequest 
{
	string username, password, email;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers, questionCount, answerTimeout;
};

struct JoinRoomRequest
{
	std::string roomId;
};

struct GetPlayersInRoomRequest : public JoinRoomRequest {}; // same fields

struct SubmitAnswerRequest
{
	unsigned int answerId;
};

/* Request data structures */
struct RequestInfo
{
	RequestCode id;
	time_t receivalTime;
	Buffer buffer;
};

struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler;
};