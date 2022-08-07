#ifndef CONVERTBOT_HPP
# define CONVERTBOT_HPP

# include "Bot.hpp"
# include <limits.h>

class BotWithFacts : public Bot {

public:
	BotWithFacts(int port, std::string password);
	~BotWithFacts();

private:
	BotWithFacts();
	BotWithFacts(const BotWithFacts & copy);
	BotWithFacts &operator=(const BotWithFacts & copy);

	std::string	_botReply(std::string msg);
	std::string	_converter(long src, int toSystem);
	bool		_isNumber(std::string str);
};

#endif