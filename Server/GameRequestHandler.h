#pragma once

#include "RequestHandlerFactory.h"
#include "Requests.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
public:
	// constructor
	GameRequestHandler(Game& game, const LoggedUser& user, RequestHandlerFactory& handlerFactory);

	// methods
	bool isRequestRelevant(const RequestInfo& reqInfo) const override; // checks if the request is for getting a question, submiting an answer, getting a game's results
																		   // or leaving a game

	const RequestResult handleRequest(const RequestInfo& reqInfo) override; // handles relevant request

private:
	// fields
	Game& m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;

	time_t m_lastRequestTime;

	// private methods
	const RequestResult getQuestion(const RequestInfo& requestInfo); // gets a new question for the user
	const RequestResult submitAnswer(const RequestInfo& requestInfo); // submits the user's answer
	const RequestResult getGameResults(const RequestInfo& requestInfo); // gets the results of the game
	const RequestResult leaveGame(const RequestInfo& requestInfo); // leaves the game for the user
};