#include "../../include/utils.hpp"

std::vector<std::string>	split(const std::string &s)
{
	std::vector<std::string>	ret;
	std::string::const_iterator	i = s.begin();
    char sep = ' ';
	while(i != s.end())
	{
		while (i != s.end() && *i == sep)
			++i;
		std::string::const_iterator	j = std::find(i, s.end(), sep);
		if (i != s.end())
		{
			if (j != s.end())
				ret.push_back(std::string(i, j + 1));
			else
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