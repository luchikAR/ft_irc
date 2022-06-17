#include "../../include/send_modul.hpp"

int		sendReply(const std::string &from, const User &user, int rpl, \
				const std::string &arg1,const std::string &arg2, \
				const std::string &arg3,const std::string &arg4, \
				const std::string &arg5,const std::string &arg6, \
				const std::string &arg7,const std::string &arg8)
{
	std::string	msg = ":" + from + " ";
	std::stringstream	ss;
	if (rpl < 10)
		msg += "00";
	ss << rpl;
	msg += ss.str() + " " + user.getNickname() + " ";
	switch (rpl)
	{
	case RPL_WELCOME:
		msg += ":" + arg1 + "\r\n";
		break;
	case RPL_YOURHOST:
		msg += ":" + arg1 + "\r\n";
		break;
	case RPL_CREATED:
		msg += ":" + arg1 + "\r\n";
		break;
	case RPL_MYINFO:
		msg += ":" + arg1 + "\r\n";
		break;
	case RPL_USERHOST:
		msg += ":" + arg1 + "\r\n";
		break;
	case RPL_ISON:
		msg += ":" + arg1 + "\r\n";
		break;
	case RPL_AWAY:
		msg += arg1 + " :" + arg2 + "\r\n";
		break;
	case RPL_UNAWAY:
		msg += ":You are no longer marked as being away\r\n";
		break;
	case RPL_NOWAWAY:
		msg += ":You have been marked as being away\r\n";
		break;
	case RPL_WHOISUSER:
		msg += arg1 + " " + arg2 + " " + arg3 + " * :" + arg4 + "\r\n";
		break;
	case RPL_WHOISSERVER:
		msg += arg1 + " " + arg2 + " :" + arg3 + "\r\n";
		break;
	case RPL_WHOISOPERATOR:
		msg += arg1 + " :is an IRC operator\r\n";
		break;
	case RPL_WHOISIDLE:
		msg += arg1 + " " + arg2 + " " + arg3 + " :seconds idle\r\n";
		break;
	case RPL_ENDOFWHOIS:
		msg += arg1 + " :End of /WHOIS list\r\n";
		break;
	case RPL_WHOISCHANNELS:
		msg += arg1 + " :" + arg2 + "\r\n";
		break;
	case RPL_WHOWASUSER:
		msg += arg1 + " " + arg2 + " " + arg3 + " * :" + arg4 + "\r\n";
		break;
	case RPL_ENDOFWHOWAS:
		msg += arg1 + " :End of WHOWAS\r\n";
		break;
	case RPL_LISTSTART:
		msg += "Channel :Users  Name\r\n";
		break;
	case RPL_LIST:
		msg += arg1 +  " " + arg2 + " :" + arg3 + "\r\n";
		break;
	case RPL_LISTEND:
		msg += ":End of /LIST\r\n";
		break;
	case RPL_CHANNELMODEIS:
		msg += arg1 + " +" + arg2 + "\r\n";
		break;
	case RPL_NOTOPIC:
		msg += arg1 + " :No topic is set\r\n";
		break;
	case RPL_TOPIC:
		msg += arg1 + " :" + arg2 + "\r\n";
		break;
	case RPL_INVITING:
		msg += arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_SUMMONING:
		msg += arg1 + " :Summoning user to IRC\r\n";
		break;
	case RPL_VERSION:
		msg += arg1 + "." + arg2 + " " + arg3 + " :" + arg4 + "\r\n";
		break;
	case RPL_WHOREPLY:
		msg += arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " ";
		msg += arg5 + " " + arg6 + " :" + arg7 + " " + arg8 + "\r\n";
		break;
	case RPL_ENDOFWHO:
		msg += arg1 + " :End of /WHO list\r\n";
		break;
	case RPL_NAMREPLY:
		msg += arg1 + " :" + arg2 + "\r\n";
		break;
	case RPL_ENDOFNAMES:
		msg += arg1 + " :End of /NAMES list\r\n";
		break;
	case RPL_LINKS:
		msg += arg1 + " " + arg2 + ": " + arg3 + " " + arg4 + "\r\n";
		break;
	case RPL_ENDOFLINKS:
		msg += arg1 + " :End of /LINKS list\r\n";
		break;
	case RPL_BANLIST:
		msg += arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_ENDOFBANLIST:
		msg += arg1 + " :End of channel ban list\r\n";
		break;
	case RPL_INFO:
		msg += ":" + arg1 + "\r\n";
		break;
	case RPL_ENDOFINFO:
		msg += ":End of /INFO list\r\n";
		break;
	case RPL_MOTDSTART:
		msg += ":- " + arg1 + " Message of the day - \r\n";
		break;
	case RPL_MOTD:
		msg += ":- " + arg1 + "\r\n";
		break;
	case RPL_ENDOFMOTD:
		msg += ":End of /MOTD command\r\n";
		break;
	case RPL_YOUREOPER:
		msg += ":You are now an IRC operator\r\n";
		break;
	case RPL_REHASHING:
		msg += arg1 + " :Rehashing\r\n";
		break;
	case RPL_TIME:
		msg += arg1 + " :" + arg2; // ctime return string following '\r\n'
		break;
	case RPL_USERSSTART:
		msg += ":UserID   Terminal  Host\r\n";
		break;
	case RPL_USERS:
		msg += ":%-8s %-9s %-8s\r\n"; // Хз, что это UPD: Понял, строка длиной 8 символов
		break;
	case RPL_ENDOFUSERS:
		msg += ":End of users\r\n";
		break;
	case RPL_NOUSERS:
		msg += ":Nobody logged in\r\n";
		break;
	case RPL_TRACELINK:
		msg += "Link " + arg1 + " " + arg2 + " " + arg3 + "\r\n";
		break;
	case RPL_TRACECONNECTING:
		msg += "Try. " + arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_TRACEHANDSHAKE:
		msg += "H.S. " + arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_TRACEUNKNOWN:
		msg += "???? " + arg1 + " " + arg2 + "\r\n"; // ????
		break;
	case RPL_TRACEOPERATOR:
		msg += "Oper " + arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_TRACEUSER:
		msg += "User " + arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_TRACESERVER:
		msg += "Serv " + arg1 + " " + arg2 + "S " + arg3 + "C ";
		msg += arg4 + " " + arg5 + "@" + arg6 + "\r\n";
		break;
	case RPL_TRACENEWTYPE:
		msg += arg1 + " 0 " + arg2 + "\r\n";
		break;
	case RPL_TRACELOG:
		msg += "File " + arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_STATSLINKINFO:
		msg += arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " ";
		msg += arg5 + " " + arg6 + " " + arg7 + "\r\n";
		break;
	case RPL_STATSCOMMANDS:
		msg += arg1 + " " + arg2 + "\r\n";
		break;
	case RPL_STATSCLINE:
		msg += "C " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\r\n";
		break;
	case RPL_STATSNLINE:
		msg += "N " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\r\n";
		break;
	case RPL_STATSILINE:
		msg += "I " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\r\n";
		break;
	case RPL_STATSKLINE:
		msg += "K " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\r\n";
		break;
	case RPL_STATSYLINE:
		msg += "Y " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\r\n";
		break;
	case RPL_ENDOFSTATS:
		msg += arg1 + " :End of /STATS report\r\n";
		break;
	case RPL_STATSLLINE:
		msg += "L " + arg1 + " * " + arg2 + " " + arg3 + "\r\n";
		break;
	case RPL_STATSUPTIME:
		msg += ":Server Up %d days %d:%02d:%02d\r\n"; // Поменять
		break;
	case RPL_STATSOLINE:
		msg += "O " + arg1 + " * " + arg2 + "\r\n";
		break;
	case RPL_STATSHLINE:
		msg += "H " + arg1 + " * " + arg2 + "\r\n";
		break;
	case RPL_UMODEIS:
		msg += arg1 + "\r\n";
		break;
	case RPL_LUSERCLIENT:
		msg += ":There are " + arg1 + " users and " + arg2;
		msg += " invisible on " + arg3 + " servers\r\n";
		break;
	case RPL_LUSEROP:
		msg += arg1 + " :operator(s) online\r\n";
		break;
	case RPL_LUSERUNKNOWN:
		msg += arg1 + " :unknown connection(s)\r\n";
		break;
	case RPL_LUSERCHANNELS:
		msg += arg1 + " :channels formed\r\n";
		break;
	case RPL_LUSERME:
		msg += ":I have " + arg1 + " clients and " + arg2 + " servers\r\n";
		break;
	case RPL_ADMINME:
		msg += arg1 + " :Administrative info\r\n";
		break;
	case RPL_ADMINLOC1:
		msg += ":Name     " + arg1 + "\r\n";
		break;
	case RPL_ADMINLOC2:
		msg += ":Nickname " + arg1 + "\r\n";
		break;
	case RPL_ADMINEMAIL:
		msg += ":E-Mail   " + arg1 + "\r\n";
		break;
	default:
		msg += "UNKNOWN REPLY\r\n";
		break;
	}
    std::cerr << CYAN << "[SERVER->CLIENT]: " << GREEN << msg << RESET << "\r\n";
	send(user.getSockfd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	return 0;
}