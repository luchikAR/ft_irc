#include "../../include/utils.hpp"

std::vector<std::string>	split(const std::string &s, char sep)
{
	std::vector<std::string>	ret;
	std::string::const_iterator	i = s.begin();
	while(i != s.end())
	{
		while (i != s.end() && *i == sep)
			++i;
		std::string::const_iterator	j = std::find(i, s.end(), sep);
		if (i != s.end())
		{
				ret.push_back(std::string(i, j));
			i = j;
		}
	}
	return ret;
}

bool	isEqualToRegex(std::string mask, std::string subString)
{
	const char *rs=0, *rp;
	const char *s = subString.c_str();
	const char *p = mask.c_str();
	while (1)
	{
		if (*p == '*')
		{
			rs = s;
			rp = ++p;
		}
		else if (!*s)
		{
			return (!(*p));
		}
		else if (*s == *p)
		{
			++s;
			++p;
		}
		else if (rs)
		{
			s = ++rs;
			p = rp;
		}
		else
		{
			return (false);
		}
	}
}

bool	isValidChannelName(const std::string &name)
{
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == 7 || name[i] == 0 \
			|| name[i] == 13 || name[i] == 10 || name[i] == ',')
			return false;
	}
	return true;
}

bool	checkNick(const std::string nick) {
	if (nick.size() > 9 || nick.size() < 1)
		return false;
	for (size_t i = 0; i < nick.size(); ++i) {
		if ((nick[i] < 'a' || nick[i] > 'z')
			&& ((nick[i] < 'A') || (nick[i] > 'Z'))
			&& ((nick[i] < '1') || (nick[i] > '9')))
			return false;
	}
	return true;
}