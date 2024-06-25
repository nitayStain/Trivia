#pragma once

#include "IDatabase.h"
#include "sqlite3.h"
#include "Exceptions.h"
#include <io.h>


#define DB_NAME "trivia.db"

#define USERS_TABLE_QUERY "CREATE TABLE IF NOT EXISTS USERS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, USERNAME TEXT NOT NULL, EMAIL TEXT NOT NULL, PASSWORD TEXT NOT NULL);"
#define QUESTIONS_TABLE_QUERY "CREATE TABLE IF NOT EXISTS QUESTIONS (QUESTION TEXT PRIMARY KEY NOT NULL, ANSWER_1 TEXT NOT NULL, ANSWER_2 TEXT NOT NULL, ANSWER_3 TEXT NOT NULL, ANSWER_4 TEXT NOT NULL, CORRECT_ANSWER_ID INTEGER NOT NULL);"
#define STATISTICS_TABLE_QUERY "CREATE TABLE IF NOT EXISTS STATISTICS (TOTAL_ANSWER_TIME INTEGER NOT NULL, NUM_OF_CORRECT_ANSWERS INTEGER NOT NULL, NUM_OF_ANSWERS INTEGER NOT NULL, NUM_OF_GAMES INTEGER NOT NULL, USER_ID INTEGER NOT NULL, FOREIGN KEY(USER_ID) REFERENCES USERS(ID));"

#define NUM_OF_HIGH_SCORES 5

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase() = default;
	virtual ~SqliteDatabase() = default;

	// parent class functions
	bool doesUserExists(const std::string& username) override;

	bool doesPasswordMatch(const std::string& username, const std::string& password) override;

	void addNewUser(const std::string& username, const std::string& email, const std::string& password) override;

	std::vector<Question> getQuestions(int numOfQuestions) override;

	void addQuestion(const std::string& question, const std::string& correctAnswer, const std::string& answer_1, const std::string& answer_2, const std::string& answer_3) override;

	virtual int getPlayerAverageAnswerTime(const std::string& username) override;

	virtual int getNumOfCorrectAnswers(const std::string& username) override;

	virtual int getNumOfTotalAnswers(const std::string& username) override;

	virtual int getNumOfPlayerGames(const std::string& username) override;

	virtual int getPlayerScore(const std::string& username) override;

	virtual std::vector<std::string> getHighScores() override;

	virtual void submitGameStatistics(const GameData& gameData, const std::string& username) override;

	bool open() override;
	bool close() override;
private:
	sqlite3* m_db;

	// this function is a shortcut for handling the error received from the sqlite driver
	void runQuery(std::string query);

	// this function does the same thing but it executes the query with given parameters and a optional callback
	void runQuery(std::string query, void* d, int(*cb) (void*, int, char**, char**));

	// returns the total answer time of a user
	int getTotalAnswerTime(const std::string& username);

	// returns a user's id
	std::string getUserId(const std::string& username);

	// callbacks
	static int userExistsCallback(void* data, int, char** argv, char**);

	static int questionsCallback(void* data, int argc, char** argv, char** azColName);

	static int specificItemCallback(void* data, int argc, char** argv, char** azColName);

	static int highScoresCallback(void* data, int argc, char** argv, char** azColName);
};