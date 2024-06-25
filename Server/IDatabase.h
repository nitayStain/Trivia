#pragma once

#include "Game.h"

struct GameData;

class IDatabase {
public:
	virtual ~IDatabase() = default;

	virtual bool open() = 0;
	virtual bool close() = 0;

	// checks for user existance in the database
	virtual bool doesUserExists(const std::string& username) = 0;

	// compares between saved password and given password of a certain user
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;

	// creates a new user in the database
	virtual void addNewUser(const std::string& username, const std::string& email, const std::string& password) = 0;

	virtual void addQuestion(const std::string& question, const std::string& correctAnswer, const std::string& answer_1, const std::string& answer_2, const std::string& answer_3) = 0;

	// returns the questions stored in the database
	virtual std::vector<Question> getQuestions(int numOfQuestions) = 0;

	// returns the average time it took the player to answer each question
	virtual int getPlayerAverageAnswerTime(const std::string& username) = 0;

	// returns the amount of correct answer the player answered
	virtual int getNumOfCorrectAnswers(const std::string& username) = 0;

	// returns the amount of questions the player answered
	virtual int getNumOfTotalAnswers(const std::string& username) = 0;

	// returns the amount of games the player played
	virtual int getNumOfPlayerGames(const std::string& username) = 0;

	// returns the score of the player
	virtual int getPlayerScore(const std::string& username) = 0;

	// returns the 5 highest player scores
	virtual std::vector<std::string> getHighScores() = 0;

	// adds the statistics of a user to the database
	virtual void submitGameStatistics(const GameData& gameData, const std::string& username) = 0;
};
