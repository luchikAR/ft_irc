#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "User.hpp"
#include "utils.hpp"

#define PRIVATE		1
#define SECRET		2
#define MODERATED	3
#define INVITEONLY	4
#define TOPICSET	5
#define NOMSGOUT	6

struct flagChannel
{
    bool _private;
    bool secret;
    bool moderated;
    bool inviteonly;
    bool topicset;
    bool nomsgout;

    flagChannel(): _private(0), secret(0), moderated(0), inviteonly(0), topicset(0), nomsgout(0)
    {}
};

class Channel
{
	private:
		std::string						name;
		std::string						pass;
		unsigned short					userLimit;
		std::vector<const User *>		operators;
		std::vector<const User *>		speakers;
		std::vector<const User *>		users;
		std::vector<const User *>		invitedUsers;
		std::vector<std::string>		banMasks;
		std::string						topic;
		flagChannel                     flags;

		Channel();
		Channel(const Channel& copy);
		Channel	&operator=(const Channel& other);
		bool							isBanned(const std::string &mask, const std::string &prefix);
		void							sendInfo(const User &user);
	public:
		Channel(const std::string &name, const User &creator, const std::string &pass = "");
		virtual ~Channel();

		const std::string				&getName() const;
		const std::string				&getTopic() const;
		flagChannel						getFlags() const;
		void							setTopic(const User &user, const std::string &topic);
		void							setLimit(unsigned short limit);
		void							setKey(const User &user, const std::string &key);

		bool							isInvited(const User &user) const;
		bool							isOperator(const User &user) const;
		bool							isSpeaker(const User &user) const;
		bool							isEmpty() const;
		bool							containsNickname(const std::string &nickname) const;

		void							connect(const User &user, const std::string &key);
		void							setFlag(unsigned char flag);
		void							removeFlag(unsigned char flag);
		void							sendMessage(const std::string &message, const User &from, bool includeUser) const;
		void							addOperator(const User &user);
		void							removeOperator(const User &user);
		void							addSpeaker(const User &user);
		void							removeSpeaker(const User &user);
		void							addBanMask(const std::string &mask);
		void							removeBanMask(const std::string &mask);
		void							displayTopic(const User &user);
		void							displayNames(const User &user);
		void							disconnect(const User &user);
		void							removeInvited(const User &user);
		void							displayChanInfo(const User &user);
		std::string						getFlagsAsString() const;
};

#endif