#include "../../include/Server.hpp"
/*---------------------------------------------------------
					PING PONG
----------------------------------------------------------*/

int		Server::pingCmd(const std::vector<std::string> &msg, User &user)
{
    if (msg.size() <= 1)
		return (sendError(user, ERR_NOORIGIN));
	user.sendMessage(":" + this->_name_server + " PONG :" + msg[1] + "\n");
	return 0;
}

int		Server::pongCmd(const std::vector<std::string> &msg, User &user)
{
	if (msg.size() <= 1 || msg[1] != this->_name_server)
		return (sendError(user, ERR_NOSUCHSERVER, msg.size() > 1 ? msg[1] : ""));
	user.removeFlag(PINGING);
	return 0;
}