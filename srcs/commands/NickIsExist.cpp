#include "../../include/utils.hpp"
#include "../../include/Server.hpp"

bool nickIsExist(const std::string nick) const {
	std::vector<User *> users = Server::getUsers();
	for (int i = 0; i < users.size(); ++i) {
		if (users[i]->getNickname() == nick) {
			return false;
		}
	}
	return true;
}