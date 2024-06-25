#pragma once

#include "Requests.h"

struct LoginRequest;
struct SignupRequest;
struct RequestInfo;
struct RequestResult;
enum requests;

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) const = 0;
	virtual const RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};

