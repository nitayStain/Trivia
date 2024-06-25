#pragma once

#include "Game.h"
#include "IDatabase.h"
#include "Room.h"

#define NUM_OF_QUESTIONS 10

class GameManager
{
public:
	// constructor
	GameManager(IDatabase* db);

	// destructor
	~GameManager();

	// methods
	const Game createGame(const Room& room); // creates a new game
	void deleteGame(const std::string& gameId); // deletes a game
	void submitAnswer(Game& game, LoggedUser& user, unsigned int answerId, int answerTime);
private:
	// fields
	IDatabase* m_database;
	std::vector<Game> m_games;
};