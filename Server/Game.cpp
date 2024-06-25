#include "Game.h"

Game::Game(const std::vector<Question>& questions, const std::vector<std::string>& usernames, const std::string& gameId)
{
	m_questions = questions;
	m_gameId = gameId;
	GameData gameData{ m_questions[DEFAULT_VALUE], DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE };
	for (int i = 0; i < usernames.size(); i++)
	{
		m_players.insert({ LoggedUser{usernames[i]}, gameData });
	}
}

Question* Game::getQuestionForUser(const LoggedUser& user)
{
	GameData userGameData = m_players.at(user);
	for (int i = 0; i < m_questions.size(); i++)
	{
		if (i == m_questions.size() - 1)
		{
			return nullptr;
		}
		else if (areQuestionsEqual(m_questions[i], userGameData.currQuestion))
		{
			m_players.at(user).currQuestion = m_questions[i + 1];
			userGameData.currQuestion = m_questions[i + 1];
			return &userGameData.currQuestion;
		}
	}
}

void Game::submitAnswer(const LoggedUser& user, unsigned int answerId, int answerTime)
{
	GameData& userGameData = m_players.at(user);
	Question question = userGameData.currQuestion;
	userGameData.totalTime += answerTime;

	auto possibleAnswers = question.getPossibleAnswers();
	if (!areQuestionsEqual(question, m_questions[LAST_QUESTION_INDEX]))
	{
		if (question.getPossibleAnswers()[question.getCorrectAnswerId()] == question.getPossibleAnswers()[answerId])
		{
			userGameData.correctAnsCount++;
		}
		else
		{
			userGameData.wrongAnsCount++;
		}
	}
}

void Game::removePlayer(const LoggedUser& user)
{
	m_players.erase(m_players.find(user));
}

const std::string Game::getGameId() const
{
	return m_gameId;
}

const GameData Game::getGameDataOfUser(const LoggedUser& user) const
{
	return m_players.at(user);
}

std::map<LoggedUser, GameData> Game::getPlayersResults()
{
	return m_players;
}

bool Game::areQuestionsEqual(const Question& q1, const Question& q2) const
{
	return q1.getQuestion() == q2.getQuestion();
}
