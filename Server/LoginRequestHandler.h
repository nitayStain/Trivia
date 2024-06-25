#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:

	LoginRequestHandler(RequestHandlerFactory& handler);

	// this function check if a request is either login/register or invalid
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;

	// this function handles the request according to the given data and code
	const RequestResult handleRequest(const RequestInfo& requestInfo) override;
private:
	
	/* Database interactions */
	RequestResult login(const RequestInfo& requestInfo);

	RequestResult signup(const RequestInfo& requestInfo);

	RequestHandlerFactory& m_handler;
};

