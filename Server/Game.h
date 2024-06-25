#pragma once

#include "Question.h"
#include "LoginManager.h"

#define DEFAULT_VALUE 0
#define LAST_QUESTION_INDEX 9

class IDatabase;
struct LoggedUser;

struct GameData
{
	Question currQuestion;
	unsigned int correctAnsCount;
	unsigned int wrongAnsCount;
	unsigned int totalTime;
};

class Game
{
public:
	// constructor
	Game(const std::vector<Question>& questions, const std::vector<std::string>& usernames, const std::string& gameId);

	// methods
	Question* getQuestionForUser(const LoggedUser& user); // switches a question for the user
	void submitAnswer(const LoggedUser& user, unsigned int answerId, int answerTime); // changes the user's game data according to his answer
	void removePlayer(const LoggedUser& user); // removes the player from the game
	const std::string getGameId() const; // returns the id of the game
	const GameData getGameDataOfUser(const LoggedUser& user) const; // returns the game data of a user

	std::map<LoggedUser, GameData> getPlayersResults(); // returns the game data of every player

private:
	// fields
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	std::string m_gameId;

	// private methods
	bool areQuestionsEqual(const Question& q1, const Question& q2) const; // checks if two questions are equal
};
