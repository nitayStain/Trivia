#pragma once

#include "pch.hpp"
#include "IDatabase.h"

class IDatabase;

struct LoggedUser {
	std::string username;
	bool operator<(const LoggedUser& user) const
	{
		return username < user.username;
	}
};

class LoginManager
{
public:
	LoginManager(IDatabase* m_database);
	~LoginManager();

	// a function used to register a user into the databse according to his credentials
	void signup(const std::string& username, const std::string& email, const std::string& password);

	// a function that adds a user to the logged users vector
	void login(const std::string& username, const std::string& password);

	// a function that removes the user from the vector.
	void logout(const std::string& username);
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
};
