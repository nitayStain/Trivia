#pragma once

#include <exception>
#include <string>

namespace Exceptions {
	// base exception
	class TriviaException : public std::exception {
	private:
		std::string m_message;
	public:

		TriviaException(const std::string& msg) : m_message(msg) {}
		virtual ~TriviaException() noexcept = default;
		virtual const char* what() const noexcept { return m_message.c_str(); }
	};

	class NetworkingException : public TriviaException {
	public:
		NetworkingException() : TriviaException("The server cannot read client actions anymore.") {}
	};
	
	class SqliteException : public TriviaException {
	public:
		SqliteException(char* err) : TriviaException("[SQLite]" + std::string(err)) {}
	};
}