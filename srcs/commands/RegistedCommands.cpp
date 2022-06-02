
#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

#include <set>
#include <fstream>

void	Server::sendMOTD(const User &user) const
{
	// hardcode
	// std::string motd = "Privet pupsiki !\nYou are registered!\r\n";
	std::vector<std::string> motd;
	std::string name = "ft_irc";

	std::string		line;
	std::ifstream	motdFile("/home/luchik/IRC_PROGECT/ft_irc/srcs/ft_irc.motd");
	if (motdFile.is_open())
	{
		while (getline(motdFile, line))
			motd.push_back(line);
		motdFile.close();
	}

	if (motd.size() == 0)
		sendError(user, ERR_NOMOTD);
	else
	{
		sendReply(name, user, RPL_MOTDSTART, name);
		for (size_t i = 0; i < motd.size(); ++i)
			sendReply(name, user, RPL_MOTD, motd[i]);
		sendReply(name, user, RPL_ENDOFMOTD);
	}
}

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
	else if (checkNick(msg[1]) == false || msg[1] == this->_name_server )
		sendError(user, ERR_ERRONEUSNICKNAME, msg[1]);
	else if (nickIsExist(msg[1]) )
		sendError(user, ERR_NICKNAMEINUSE, msg[1]);
	else
	{
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
				sendMOTD(user);
			}
		}
		else
			return (DISCONNECT);
	}
	return (0);
}

int		Server::quitCmd(const std::vector <std::string> &msg, User &user) {
	if (msg.size() > 1)
		user.setQuitMessage(msg[1]);
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
	std::string message;
	for (size_t i = 2; i < msg.size(); ++i) {
		message += msg[i];
		if (i + 1 != msg.size()) {
			message += " ";
		}
	}

	if (msg[0] == "NOTICE" && (receivers.size() > 1 \
	|| receivers.front()[0] == '#' || receivers.front()[0] == '&'))
		return (sendError(user, ERR_NOSUCHNICK, msg[1]));

	while (receivers.size() > 0)
	{
		if (uniqReceivers.find(receivers.front()) != uniqReceivers.end())
			return (sendError(user, ERR_TOOMANYTARGETS, receivers.front()));
		if (receivers.front()[0] == '#' || receivers.front()[0] == '&')
		{
			if (!this->containsChannel(receivers.front()))
				return (sendError(user, ERR_NOSUCHNICK, receivers.front()));
			if (!this->channels[receivers.front()]->containsNickname(user.getNickname()))
				return (sendError(user, ERR_CANNOTSENDTOCHAN, receivers.front()));
		}
		else if (!this->containsNickname(receivers.front()))
			return (sendError(user, ERR_NOSUCHNICK, msg[1]));
		uniqReceivers.insert(receivers.front());
		receivers.erase(receivers.begin());
	}
	for (std::set<std::string>::iterator it = uniqReceivers.begin(); it != uniqReceivers.end(); ++it)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&')
		{
			Channel *receiverChannel = this->channels[*it];
			if (receiverChannel->getFlags().moderated == true && (!receiverChannel->isOperator(user) && !receiverChannel->isSpeaker(user)))
				sendError(user, ERR_CANNOTSENDTOCHAN, *it);
			else
				receiverChannel->sendMessage(msg[0] + " " + *it + " :" + message + "\n", user, false);
		}
		else
		{
				this->getUserByName(*it)->sendMessage(":" + user.getPrefix() + " " + msg[0] + " " + *it + " :" + message + "\n");
		}
	}
	return 0;
}

int				Server::noticeCmd(const std::vector<std::string> &msg, User &user) {
	privmsgCmd(msg, user);
	return 0;
}