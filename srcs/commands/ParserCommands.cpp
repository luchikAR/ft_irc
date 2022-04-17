//
// Created by Utherydes Bolt on 4/16/22.
//

#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

/*
*************************************************************
** Реализуем команды
*************************************************************
*/

int Server::passCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.empty() == true) {
		sendError(user, ERR_NEEDMOREPARAMS, msg[0]);
	}
	else if (user.getFlags().registered == true) {
		sendError(user, ERR_ALREADYREGISTRED);
	}
	/*
	 * 	else
		user.setPassword(msg.getParams()[0]);
	 */
	return 0;
}

int Server::nickCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.empty() == true) {
		sendError(user, ERR_NEEDMOREPARAMS, msg[0]);
	}
	else if (checkNick(msg[1]) == false /* || msg[1] == this->name */ ) // что за проверка msg[1] == this->name ????
		sendError(user, ERR_ERRONEUSNICKNAME, msg[1]);
	else if (nickIsExist(msg[1]) == false)
		sendError(user, ERR_NICKNAMEINUSE, msg[1]);
	else
	{
//		if (user.getFlags().registered == true)
//		{
//			notifyUsers(user, ":" + user.getPrefix() + " " + msg[0] + " " + msg[1] + "\n");
//			this->nicknamesHistory.addUser(user);
//		}
		user.setNickname(msg[1]);
	}
	return (REGISTERED); // тут стояла проверка checkConnection(user)
}

int		Server::userCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.size() < 5)
		sendError(user, ERR_NEEDMOREPARAMS, msg[0]);
	else if (user.getFlags().registered == true)
		sendError(user, ERR_ALREADYREGISTRED);
	else
	{
		user.setUsername(msg[1]);
		user.setRealname(msg[4]);
		user.setFlag(REGISTERED);
		user.sendMessage("You are registered!");
	}
	return (REGISTERED); // тут стояла проверка checkConnection(user)
}

int		Server::quitCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.size() > 1)
		user.setQuitMessage(msg[1]); //зачем фиксировать сообщение о выходе?
//	this->nicknamesHistory.addUser(user);
	return (DISCONNECT);
}