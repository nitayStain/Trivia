#include "LoginRequestHandler.h"
#include "JsonResponsePacketserializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "Exceptions.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handler) : m_handler(handler)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	return requestInfo.id == RequestCode::LOGIN_REQUEST || requestInfo.id == RequestCode::SIGNUP_REQUEST;
}

const RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	try {
		return requestInfo.id == RequestCode::LOGIN_REQUEST ? login(requestInfo) : signup(requestInfo);
	}
	catch (Exceptions::TriviaException& e) {
		ErrorResponse err = { e.what() };
		return {
			JsonResponsePacketSerializer::serializeResponse(err),
			nullptr
		};
	}
	catch (json::exception& e) {
		ErrorResponse err = { "A serialization error occurred." };
		return {
			JsonResponsePacketSerializer::serializeResponse(err),
			nullptr
		};
	}
}

RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
	auto payload = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
	LoginManager& manager = m_handler.getLoginManager();

	LoginResponse logResp;
	logResp.status = Responses::LOGIN_RESPONSE;

	manager.login(payload.username, payload.password);

	return {
		JsonResponsePacketSerializer::serializeResponse(logResp),
		m_handler.createMenuRequestHandler({ payload.username }),
	};
}

RequestResult LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
	auto manager = m_handler.getLoginManager();
	auto payload = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);

	SignupResponse signupResp;
	signupResp.status = Responses::SIGNUP_RESPONSE;

	manager.signup(payload.username, payload.email, payload.password);

	return {
		JsonResponsePacketSerializer::serializeResponse(signupResp),
		m_handler.createMenuRequestHandler({ payload.username }),
	};
}
