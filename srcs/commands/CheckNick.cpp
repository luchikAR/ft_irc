#include "../../include/utils.hpp"

bool	checkNick(const std::string nick) {
	if (nick.length() > 9)
		return (false);
	std::string	special = "-[]\\`^{}";
	for (size_t i = 0; i < nick.size(); i++)
	{
		if ((nick[i] >= 'a' && nick[i] <= 'z')
			|| (nick[i] >= 'A' && nick[i] <= 'Z')
			|| (nick[i] >= '0' && nick[i] <= '9')
			|| (special.find(nick[i]) != std::string::npos))
			continue ;
		else
			return (false);
	}
	return (true);
}