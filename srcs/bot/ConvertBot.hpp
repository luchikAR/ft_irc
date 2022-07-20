#ifndef CONVERTBOT_HPP
# define CONVERTBOT_HPP

# include "Bot.hpp"
# include <limits.h>

class ConvertBot : public Bot {

public:
	ConvertBot(int port, std::string password);
	~ConvertBot();

private:
	ConvertBot();
	ConvertBot(const ConvertBot & copy);
	ConvertBot &operator=(const ConvertBot & copy);

	std::string	_botReply(std::string msg);
	std::string	_converter(long src, int toSystem);
	bool		_isNumber(std::string str);
};

#endif