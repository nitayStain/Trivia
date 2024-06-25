#pragma once

#include "IDatabase.h"

class StatisticsManager
{
public:
	// constructor
	StatisticsManager(IDatabase* db);

	// destructor
	~StatisticsManager();

	// methods
	std::vector<std::string> getHighScores(); // returns the 5 best scores
	std::vector<std::string> getUserStatistics(const std::string& username); // returns the user's statistics

private:
	// fields
	IDatabase* m_database;
};