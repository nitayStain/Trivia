#include "Question.h"

Question::Question(const std::string& question, const std::vector<std::string>& possibleAnswers) : m_question(question), m_possibleAnswers(possibleAnswers)
{}

std::string Question::getQuestion() const
{
	return m_question;
}

std::vector<std::string> Question::getPossibleAnswers() const
{
	return m_possibleAnswers;
}

int Question::getCorrectAnswerId() const
{
	return 0;
}