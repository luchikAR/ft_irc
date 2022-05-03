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
		user.sendMessage("You are registered!\n");
	}
	return (REGISTERED); // тут стояла проверка checkConnection(user)
}

int		Server::quitCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.size() > 1)
		user.setQuitMessage(msg[1]); //зачем фиксировать сообщение о выходе?
//	this->nicknamesHistory.addUser(user);
	return (DISCONNECT);
}

int 	Server::privmsgCmd(const std::vector<std::string> &msg, User &user)
{
	std::cout << "I'm in the privmsgCmd\n";

	if (msg.size() == 1)
		return (sendError(user, ERR_NORECIPIENT, msg[0]));
	if (msg.size() == 2)
		return (sendError(user, ERR_NOTEXTTOSEND));

	std::vector<std::string> receivers = split(msg[1], ',');
	std::set<std::string> uniqReceivers;

	for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
	{
//		 checking if there contains dublicate receiver
//		if (uniqReceivers.find(receivers.front()) != uniqReceivers.end())     <--
//																				 |	эти строки лишние, т к в set не допускаются повторения
//			return (sendError(user, ERR_TOOMANYTARGETS, receivers.front()));  <--
//		 if receiver is channel
		if (receivers.front()[0] == '#' || receivers.front()[0] == '$')
		{
//			 checking if there such a channel
			if (!this->containsChannel(receivers.front()))
				return (sendError(user, ERR_NOSUCHNICK, receivers.front()));
//			 check that the current user is in the channel
			if (!this->channels[receivers.front()]->containsNickname(user.getNickname()))
				return (sendError(user, ERR_CANNOTSENDTOCHAN, receivers.front()));
		}
//			 checking if there such a nickname
		else if (this->nickIsExist(*it) == false)
			return (sendError(user, ERR_NOSUCHNICK, msg[1]));
		uniqReceivers.insert(*it);
//		receivers.pop_back();
	}
	for (std::set<std::string>::iterator it = uniqReceivers.begin(); it != uniqReceivers.end(); ++it)
	{
		if ((*it)[0] == '#' || (*it)[0] == '$')
		{
			Channel *receiverChannel = this->channels[*it];
			// check that user can send message to channel (user is operator or speaker on moderated channel)
			if (receiverChannel->getFlags().nomsgout & MODERATED && (!receiverChannel->isOperator(user) && !receiverChannel->isSpeaker(user)))
				sendError(user, ERR_CANNOTSENDTOCHAN, *it);
			else
				receiverChannel->sendMessage(msg[0] + " " + *it + " :" + msg[1] + "\n", user, false);
		}
		else
		{
//			if (msg[0] == "PRIVMSG" && (this->getUserByName(*it)->getFlags() & AWAY))																 <--
//																																						| этот чеккер нужен для автоответа с флагом AWAY ??
//			sendReply(user.getServername(), user, RPL_AWAY, *it, this->getUserByName(*it)->getAwayMessage()); //для чего тут нужен getAwayMessage??? <--
			if (msg[0] != "NOTICE" || (this->getUserByName(*it)->getFlags().registered & RECEIVENOTICE))
				this->getUserByName(*it)->sendMessage(":" + user.getPrefix() + " " + msg[0] + " " + *it + " :" + msg[1] + "\n");
		}
	}
	return 0;
}

int		Server::noticeCmd(const std::vector<std::string> &msg, User &user)
{
	int size = msg.size();
	std::string user1 = user.getNickname();
	return size;
}