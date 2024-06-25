#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* db) : m_database(db)
{}

StatisticsManager::~StatisticsManager()
{
}

std::vector<std::string> StatisticsManager::getHighScores()
{
	return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username)
{
	std::string avgAnsTime = std::to_string(m_database->getPlayerAverageAnswerTime(username));
	std::string numOfCorrectAns = std::to_string(m_database->getNumOfCorrectAnswers(username));
	std::string numOfAns = std::to_string(m_database->getNumOfTotalAnswers(username));
	std::string numOfGames = std::to_string(m_database->getNumOfPlayerGames(username));
	std::vector<std::string> statistics = { avgAnsTime, numOfCorrectAns, numOfAns, numOfGames };
	return statistics;
}