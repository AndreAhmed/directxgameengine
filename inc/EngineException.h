#pragma once 
#include <string>
#include <exception>

namespace gameErrorNS
{
	// Error codes
	// Negative numbers are fatal errors that may require the game to be shutdown.
	// Positive numbers are warnings that do not require the game to be shutdown.
	const int FATAL_ERROR = -1;
	const int WARNING = 1;
}

class cGameException : public std::exception
{
private:
	int     errorCode;
	std::string message;
public:
	// default constructor
	cGameException() throw() :errorCode(gameErrorNS::FATAL_ERROR), message("Undefined Error in game.") {}
	// copy constructor
	cGameException(const cGameException& e) throw() : std::exception(e), errorCode(e.errorCode), message(e.message) {}
	// constructor with args
	cGameException(int code, const std::string &s) throw() :errorCode(code), message(s) {}
	// assignment operator
	cGameException& operator= (const cGameException& rhs) throw()
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}
	// destructor
	virtual ~cGameException() throw() {};

	// override what from base class
	virtual const char* what() const throw() { return this->getMessage(); }

	const char* getMessage() const throw() { return message.c_str(); }
	int getErrorCode() const throw() { return errorCode; }
};