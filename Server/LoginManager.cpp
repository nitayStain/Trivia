#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* m_database) : m_database(m_database)
{
}

LoginManager::~LoginManager()
{
}

void LoginManager::signup(const std::string& username, const std::string& email, const std::string& password)
{
	if (m_database->doesUserExists(username))
		throw Exceptions::TriviaException("User already exists.");

	m_database->addNewUser(username, email, password);
	login(username, password);
}

void LoginManager::login(const std::string& username, const std::string& password)
{
	if (!m_database->doesPasswordMatch(username, password))
		throw Exceptions::TriviaException("Username or password invalid.");

	m_loggedUsers.push_back({ username });
}

void LoginManager::logout(const std::string& username)
{
	m_loggedUsers.erase(std::remove_if(m_loggedUsers.begin(), m_loggedUsers.end(), [username](const LoggedUser& s) {
		return s.username == username;
	}), m_loggedUsers.end());
}
