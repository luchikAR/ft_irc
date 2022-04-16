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