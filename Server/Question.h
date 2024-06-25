#pragma once

#include <string>
#include <vector>

class Question
{
public:
	// constructor
	Question(const std::string& question, const std::vector<std::string>& possibleAnswers);

	// destructor
	~Question() = default;

	// methods
	std::string getQuestion() const; // gets the question
	std::vector<std::string> getPossibleAnswers() const; // gets the 4 possible answers of the question
	int getCorrectAnswerId() const; // gets the id of the correct answer

private:
	// fields
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
};