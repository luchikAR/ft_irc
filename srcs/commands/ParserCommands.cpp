//
// Created by Utherydes Bolt on 4/16/22.
//

#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

#include <set>

/*
*************************************************************
** Реализуем команды
*************************************************************
*/

int Server::passCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.empty() == true || msg.size() <= 1) {
		sendError(user, ERR_NEEDMOREPARAMS, msg[0]);
	}
	else if (user.getFlags().registered == true) {
		sendError(user, ERR_ALREADYREGISTRED);
	}
	else {
		user.setPassword(msg[1]);
	}
	return 0;
}

int Server::nickCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.size() <= 1) {
		sendError(user, ERR_NEEDMOREPARAMS, msg[0]);
	}
	else if (checkNick(msg[1]) == false || msg[1] == this->_name_server ) // что за проверка msg[1] == this->name ????
		sendError(user, ERR_ERRONEUSNICKNAME, msg[1]);
	else if (nickIsExist(msg[1]) )
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
	return (checkConnection(user));
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
	}
	return (checkConnection(user));
}

int		Server::checkConnection(User &user)
{
	if (user.getNickname().size() > 0 && user.getUsername().size() > 0)
	{
		if (_pass.size() == 0 || user.getPassword() == _pass)
		{
			if (!(user.getFlags().registered == true))
			{
				user.setFlag(REGISTERED);
				user.sendMessage("You are registered!\r\n"); // можно переместить в другое место
				// sendMOTD(user); выводило характеристики сервера
			}
		}
		else
			return (DISCONNECT);
	}
	return (0);
}

int		Server::quitCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.size() > 1)
		user.setQuitMessage(msg[1]); //зачем фиксировать сообщение о выходе?
//	this->nicknamesHistory.addUser(user);
	return (DISCONNECT);
}

int 	Server::privmsgCmd(const std::vector<std::string> &msg, User &user)
{
	if (msg.size() < 2)
		return (sendError(user, ERR_NORECIPIENT, msg[0]) );
	if (msg.size() == 2)
		return (sendError(user, ERR_NOTEXTTOSEND));

	std::vector<std::string> receivers = split(msg[1], ',');
	std::set<std::string> uniqReceivers;

	if (msg[0] == "NOTICE" && (receivers.size() > 1 \
	|| receivers.front()[0] == '#' || receivers.front()[0] == '&'))
		return (sendError(user, ERR_NOSUCHNICK, msg[1]));

	while (receivers.size() > 0)
	{
		// checking if there contains dublicate receiver
		if (uniqReceivers.find(receivers.front()) != uniqReceivers.end())
			return (sendError(user, ERR_TOOMANYTARGETS, receivers.front()));
		// if receiver is channel
		if (receivers.front()[0] == '#' || receivers.front()[0] == '&')
		{
			// checking if there such a channel 
			if (!this->containsChannel(receivers.front()))
				return (sendError(user, ERR_NOSUCHNICK, receivers.front()));
			// check that the current user is in the channel
			if (!this->channels[receivers.front()]->containsNickname(user.getNickname()))
				return (sendError(user, ERR_CANNOTSENDTOCHAN, receivers.front()));
		}
		// checking if there such a nickname 
		else if (!this->containsNickname(receivers.front()))
			return (sendError(user, ERR_NOSUCHNICK, msg[1]));
		uniqReceivers.insert(receivers.front());
		receivers.erase(receivers.begin()); // receivers.pop();
	}
	for (std::set<std::string>::iterator it = uniqReceivers.begin(); it != uniqReceivers.end(); ++it)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&')
		{
			Channel *receiverChannel = this->channels[*it];
			// check that user can send message to channel (user is operator or speaker on moderated channel)
			if (receiverChannel->getFlags().moderated == true && (!receiverChannel->isOperator(user) && !receiverChannel->isSpeaker(user)))
				sendError(user, ERR_CANNOTSENDTOCHAN, *it);
			else
				receiverChannel->sendMessage(msg[0] + " " + *it + " :" + msg[2] + "\n", user, false);
		}
		else
		{
			// if (msg[0] == "PRIVMSG" && (this->getUserByName(*it)->getFlags() & AWAY))
			// 	sendReply(user.getServername(), user, RPL_AWAY, *it, this->getUserByName(*it)->getAwayMessage());
			// if (msg[0] != "NOTICE" || (this->getUserByName(*it)->getFlags() & RECEIVENOTICE))
				this->getUserByName(*it)->sendMessage(":" + user.getPrefix() + " " + msg[0] + " " + *it + " :" + msg[2] + "\n");
		}
	}
	return 0;
}
