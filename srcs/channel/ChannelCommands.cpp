#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

bool	Server::containsChannel(const std::string &name) const
{
	try
	{
		channels.at(name);
		return true;
	}
	catch(const std::exception& e)
	{}
	return false;
}

int		Server::joinCmd(const std::vector<std::string> &msg, User &user)
{
    int coutParam = msg.size() - 1;
	if (coutParam == 0)
		sendError(user, ERR_NEEDMOREPARAMS, msg[0]);
	else
	{
		std::vector<std::string>	chans = split(msg[1], ',');
		std::vector<std::string>	keys;
		if (coutParam > 1)
			keys = split(msg[2], ',');
		for (; chans.size() > 0; chans.erase(chans.begin()))
		{
			std::string	key = keys.size() ? keys.front() : "";
			if (keys.size() > 0)
				keys.erase(chans.begin());
			if (!isValidChannelName(chans.front()))
				sendError(user, ERR_NOSUCHCHANNEL, chans.front());
			else if (user.getChannels().size() >= maxChannels)
				sendError(user, ERR_TOOMANYCHANNELS, chans.front());
			else if (connectToChannel(user, chans.front(), key) == 1)
				user.addChannel(*(channels.at(chans.front())));
		}
	}
	return 0;
}

int		Server::connectToChannel(const User &user, const std::string &name, const std::string &key)
{
	try
	{
		Channel	*tmp = channels.at(name);
		tmp->connect(user, key);
		return (1);
	}
	catch(const std::exception& e)
	{
		channels[name] = new Channel(name, user, key);
	}
	return (1);
}

int		Server::kickCmd(const std::vector<std::string> &msg, User &user)
{
	int coutParam = msg.size() - 1;
	if (coutParam < 2)
		sendError(user, ERR_NEEDMOREPARAMS, "KICK");
	else if (!containsChannel(msg[1]) )
		sendError(user, ERR_NOSUCHCHANNEL, msg[1]);
	else if (!channels.at(msg[1])->isOperator(user))
		sendError(user, ERR_CHANOPRIVSNEEDED, msg[1]);
	else if (!channels.at(msg[1])->containsNickname(user.getNickname()))
		sendError(user, ERR_NOTONCHANNEL, msg[1]);
	else if (!containsNickname(msg[2]) )
		sendError(user, ERR_NOSUCHNICK, msg[2]);
	else if (!channels.at( msg[1] )->containsNickname( msg[2] ) )
		sendError(user, ERR_USERNOTINCHANNEL, msg[2], msg[1]);
	else
	{
		Channel	*chan = channels.at(msg[1]);
		std::string	message = "KICK " + chan->getName() + " " + msg[2] + " :";
		if (coutParam > 2)
			message += msg[3];
		else
			message += user.getNickname();
		chan->sendMessage(message + "\n", user, true);
		chan->disconnect(*(getUserByName( msg[2] ) ));
		getUserByName(msg[2] )->removeChannel( msg[1] );
	}
	return 0;
}