#include "SqliteDatabase.h"
#include <algorithm>

bool SqliteDatabase::open()
{
	int fileExists = _access(DB_NAME, 0);
	int res = sqlite3_open(DB_NAME, &m_db);
	if (res != SQLITE_OK)
	{
		m_db = nullptr;
		return false;
	}

	std::vector<std::string> tableQueries = { USERS_TABLE_QUERY, QUESTIONS_TABLE_QUERY, STATISTICS_TABLE_QUERY };

	// there will be more queries for more tables
	for (auto& query : tableQueries) {
		try {
			runQuery(query);
		}
		catch (...) {
			return false;
		}
	}

	return true;
}

bool SqliteDatabase::close()
{
	try {
		sqlite3_close(m_db);
		m_db = nullptr;
	}
	catch (...) {
		m_db = nullptr;
		return false;
	}

	return true;
}


bool SqliteDatabase::doesUserExists(const std::string& username)
{
	bool exists = false;
	std::string query = "SELECT USERNAME FROM USERS WHERE USERNAME = '" + username + "';";

	runQuery(query, &exists, userExistsCallback);

	return exists;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	if (!doesUserExists(username))
	{
		throw Exceptions::TriviaException("User doesn't exist");
	}
	std::string query = "SELECT PASSWORD FROM USERS WHERE USERNAME = '" + username + "';";
	std::string usrPass;

	runQuery(query, &usrPass, specificItemCallback);

	return password == usrPass;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& email, const std::string& password)
{
	std::string query = "INSERT INTO USERS (USERNAME, EMAIL, PASSWORD) VALUES ('" + username + "', '" + email + "', '" + password + "');";
	runQuery(query);
}

std::vector<Question> SqliteDatabase::getQuestions(int numOfQuestions)
{
	std::string query = "SELECT * FROM QUESTIONS LIMIT " + std::to_string(numOfQuestions) + ";";
	std::vector<Question> questions;
	runQuery(query, &questions, questionsCallback);
	return questions;
}

void SqliteDatabase::addQuestion(const std::string& question, const std::string& correctAnswer, const std::string& answer_1, const std::string& answer_2, const std::string& answer_3)
{
	std::string query = "INSERT INTO QUESTIONS (QUESTION, ANSWER_1, ANSWER_2, ANSWER_3, ANSWER_4, CORRECT_ANSWER_ID) VALUES ("\
		"'" + question + "', " \
		"'" + correctAnswer + "', " \
		"'" + answer_1 + "', " \
		"'" + answer_2 + "', " \
		"'" + answer_3 + "', " \
		"0"\
		");";
	runQuery(query);
}

int SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
	int totalAnsTime = getTotalAnswerTime(username);
	int numOfAns = getNumOfTotalAnswers(username);
	int avgAnsTime = numOfAns == 0 ? totalAnsTime / numOfAns : 0;
	return avgAnsTime;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
	if (!doesUserExists(username))
	{
		throw Exceptions::TriviaException("User doesn't exist");
	}
	std::string userId = getUserId(username);
	std::string numOfCorrectAns;
	std::string query = "SELECT NUM_OF_CORRECT_ANSWERS FROM STATISTICS WHERE USER_ID = " + userId + ";";
	runQuery(query, &numOfCorrectAns, specificItemCallback);
	return numOfCorrectAns != "" ? stoi(numOfCorrectAns) : 0;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
	if (!doesUserExists(username))
	{
		throw Exceptions::TriviaException("User doesn't exist");
	}
	std::string userId = getUserId(username);
	std::string numOfAns;
	std::string query = "SELECT NUM_OF_ANSWERS FROM STATISTICS WHERE USER_ID = " + userId + ";";
	runQuery(query, &numOfAns, specificItemCallback);
	return numOfAns != "" ? stoi(numOfAns) : 0;

}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
	if (!doesUserExists(username))
	{
		throw Exceptions::TriviaException("User doesn't exist");
	}
	std::string userId = getUserId(username);
	std::string numOfGames;
	std::string query = "SELECT NUM_OF_GAMES FROM STATISTICS WHERE USER_ID = " + userId + ";";
	runQuery(query, &numOfGames, specificItemCallback);
	return numOfGames != "" ? stoi(numOfGames) : 0;
}

int SqliteDatabase::getPlayerScore(const std::string& username)
{
	return getNumOfCorrectAnswers(username);
}

std::vector<std::string> SqliteDatabase::getHighScores()
{
	// go over the users in the db and use the getPlayerScore function on each user to get the 5 best scores
	std::string query = "SELECT USERNAME FROM USERS;";
	std::vector<std::string> usernames;
	std::vector<int> scores;
	std::vector<std::string> highScores;
	runQuery(query, &usernames, highScoresCallback);
	for (int i = 0; i < usernames.size(); i++)
	{
		scores.push_back(getPlayerScore(usernames[i]));
	}
	std::sort(scores.begin(), scores.end());
	std::reverse(scores.begin(), scores.end());
	scores.resize(NUM_OF_HIGH_SCORES);
	for (int i = 0; i < scores.size(); i++)
	{
		highScores.push_back(std::to_string(scores[i]));
	}
	return highScores;
}

void SqliteDatabase::submitGameStatistics(const GameData& gameData, const std::string& username)
{
	if (!doesUserExists(username))
	{
		throw Exceptions::TriviaException("User doesn't exist");
	}
	std::string userId = getUserId(username);

	std::string selectQuery = "SELECT COUNT(*) FROM STATISTICS WHERE USER_ID = " + userId + ";";
	std::string result = "";
	runQuery(selectQuery, &result, specificItemCallback);

	std::string query = "";

	if (std::stoi(result) > 0)
	{
		query = "UPDATE STATISTICS SET "
			"TOTAL_ANSWER_TIME = TOTAL_ANSWER_TIME + " + std::to_string(gameData.totalTime) + ", "
			"NUM_OF_CORRECT_ANSWERS = NUM_OF_CORRECT_ANSWERS + " + std::to_string(gameData.correctAnsCount) + ", "
			"NUM_OF_ANSWERS = NUM_OF_ANSWERS + " + std::to_string(gameData.correctAnsCount + gameData.wrongAnsCount) + ", "
			"NUM_OF_GAMES = NUM_OF_GAMES + 1 "
			"WHERE USER_ID = " + userId + ";";
	}
	else
	{
		query = "INSERT INTO STATISTICS (TOTAL_ANSWER_TIME, NUM_OF_CORRECT_ANSWERS, NUM_OF_ANSWERS, NUM_OF_GAMES, USER_ID) VALUES" \
				"(" + std::to_string(gameData.totalTime) + ", " +
				std::to_string(gameData.correctAnsCount) + ", " +
				std::to_string(gameData.correctAnsCount + gameData.wrongAnsCount) + ", " +
				"1" +
				userId + ";";
	}

	runQuery(query);
}

int SqliteDatabase::getTotalAnswerTime(const std::string& username)
{
	std::string userId = getUserId(username);
	std::string query = "SELECT TOTAL_ANSWER_TIME FROM STATISTICS WHERE USER_ID = " + userId + ";";
	std::string totalAnsTime;
	runQuery(query, &totalAnsTime, specificItemCallback);
	return totalAnsTime != "" ? stoi(totalAnsTime) : 0;
}

std::string SqliteDatabase::getUserId(const std::string& username)
{
	std::string query = "SELECT ID FROM USERS WHERE USERNAME = '" + username + "';";
	std::string userId;
	runQuery(query, &userId, specificItemCallback);
	return userId;
}

void SqliteDatabase::runQuery(std::string query)
{
	runQuery(query, nullptr, nullptr);
}

void SqliteDatabase::runQuery(std::string query, void* d, int(*cb) (void*, int, char**, char**))
{
	char* err = nullptr;
	int res = sqlite3_exec(m_db, query.c_str(), cb, d, &err);
	if (res != SQLITE_OK)
		throw Exceptions::SqliteException(err);
}

/* Callbacks */

int SqliteDatabase::userExistsCallback(void* data, int argc, char** argv, char** azColName)
{
	bool* exists = (bool*)data;

	if (std::string(azColName[0]) == "USERNAME")
		*exists = true;

	return 0;
}

int SqliteDatabase::questionsCallback(void* data, int argc, char** argv, char** azColName)
{
	std::vector<Question>* questions = (std::vector<Question>*)data;
	questions->push_back(Question(argv[0], std::vector<std::string>({ argv[1], argv[2], argv[3], argv[4] })));
	return 0;
}

int SqliteDatabase::specificItemCallback(void* data, int argc, char** argv, char** azColName)
{
	std::string* item = (std::string*)data;
	*item = argv[0];
	return 0;
}

int SqliteDatabase::highScoresCallback(void* data, int argc, char** argv, char** azColName)
{
	std::vector<std::string>* highScores = (std::vector<std::string>*)data;
	highScores->push_back(argv[0]);
	return 0;
}
