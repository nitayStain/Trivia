#include "GameRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

GameRequestHandler::GameRequestHandler(Game& game, const LoggedUser& user, RequestHandlerFactory& handlerFactory) : m_game(game), m_user(user), m_handlerFactory(handlerFactory),
m_gameManager(handlerFactory.getGameManager())
{}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& reqInfo) const
{
	return reqInfo.id >= RequestCode::LEAVE_GAME_REQUEST && reqInfo.id <= RequestCode::GET_QUESTION_REQUEST;
}

const RequestResult GameRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	try
	{
		switch (reqInfo.id)
		{
		case RequestCode::LEAVE_GAME_REQUEST:
			return leaveGame(reqInfo);
			break;
		case RequestCode::SUBMIT_ANSWER_REQUEST:
			return submitAnswer(reqInfo);
			break;
		case RequestCode::GET_GAME_RESULTS_REQUEST:
			return getGameResults(reqInfo);
			break;
		case RequestCode::GET_QUESTION_REQUEST:
			return getQuestion(reqInfo);
			break;
		}
	}
	catch (Exceptions::TriviaException& e)
	{
		ErrorResponse err = { e.what() };
		return { JsonResponsePacketSerializer::serializeResponse(err), nullptr };
	}
	catch (json::exception& e)
	{
		ErrorResponse err = { "A serialization error occurred." };
		return { JsonResponsePacketSerializer::serializeResponse(err), nullptr };
	}
}

const RequestResult GameRequestHandler::getQuestion(const RequestInfo& reqInfo)
{
	Question* question = m_game.getQuestionForUser(m_user);

	GetQuestionResponse resp;
	resp.status = Responses::GET_QUESTION_RESPONSE;
	resp.success = question != nullptr ? 1 : 0;
	resp.question = question != nullptr ? question->getQuestion() : "";
	for (int i = 0; i < question->getPossibleAnswers().size(); i++)
	{
		resp.answers.insert({ i, question->getPossibleAnswers()[i] });
	}

	delete question;
	m_lastRequestTime = reqInfo.receivalTime;
	return { JsonResponsePacketSerializer::serializeResponse(resp), nullptr };
}

const RequestResult GameRequestHandler::submitAnswer(const RequestInfo& reqInfo)
{
	SubmitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(reqInfo.buffer);
	int timeTook = reqInfo.receivalTime - m_lastRequestTime;
	m_gameManager.submitAnswer(m_game, m_user, req.answerId, timeTook);

	SubmitAnswerResponse resp;
	resp.status = Responses::SUBMIT_ANSWER_RESPONSE;
	resp.correctAnsId = m_game.getGameDataOfUser(m_user).currQuestion.getCorrectAnswerId();
	return { JsonResponsePacketSerializer::serializeResponse(resp), nullptr };
}

const RequestResult GameRequestHandler::getGameResults(const RequestInfo& reqInfo)
{
	GetGameResultsResponse resp;
	bool checkSuccess = true;
	resp.status = Responses::GET_GAME_RESULTS_RESPONSE;
	auto results = m_game.getPlayersResults();
	for (auto& it : results)
	{
		resp.results.push_back({
			it.first.username,
			it.second.correctAnsCount,
			it.second.wrongAnsCount,
			it.second.totalTime / (it.second.correctAnsCount + it.second.wrongAnsCount),
		});

		if (it.second.correctAnsCount + it.second.wrongAnsCount < 10)
		{
			checkSuccess = false;
		}
	}
	resp.success = checkSuccess ? 1 : 0;

	return { JsonResponsePacketSerializer::serializeResponse(resp), m_handlerFactory.createMenuRequestHandler(m_user) };
}

const RequestResult GameRequestHandler::leaveGame(const RequestInfo& reqInfo)
{
	m_game.removePlayer(m_user);

	LeaveGameResponse resp;
	resp.status = Responses::LEAVE_GAME_RESPONSE;
	return { JsonResponsePacketSerializer::serializeResponse(resp), nullptr };
}