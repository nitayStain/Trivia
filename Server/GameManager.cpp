#include "GameManager.h"
#include "Exceptions.h"

GameManager::GameManager(IDatabase* db) : m_database(db)
{}

GameManager::~GameManager()
{
	delete m_database;
	m_database = nullptr;
}

const Game GameManager::createGame(const Room& room)
{
	std::vector<Question> questions = m_database->getQuestions(NUM_OF_QUESTIONS);
	Game game(questions, room.getAllUsers(), room.getRoomData().id);
	m_games.push_back(game);
	return game;
}

void GameManager::deleteGame(const std::string& gameId)
{
	for (auto it = m_games.begin(); it != m_games.end(); ++it)
	{
		if (gameId == it->getGameId())
		{
			m_games.erase(it);
			return;
		}
	}
	throw Exceptions::TriviaException("No game with such id");
}

void GameManager::submitAnswer(Game& game, LoggedUser& user, unsigned int answerId, int answerTime)
{
	game.submitAnswer(user, answerId, answerTime);
	m_database->submitGameStatistics(game.getGameDataOfUser(user), user.username);
}
