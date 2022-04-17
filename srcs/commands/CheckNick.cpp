#include "../../include/utils.hpp"

bool	checkNick(const std::string nick) const{
	if (nick.size() > 9 || nick.size() < 1)
		return false;
	for (int i = 0; i < nick.size(); ++i) {
		if ((nick[i] < 'a' || nick[i] > 'z')
			&& ((nick[i] < 'A') || (nick[i] > 'Z'))
			&& ((nick[i] < '1') || (nick[i] > '9')))
			return false;
	}
	return true;
}