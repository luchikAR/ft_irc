#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <queue>
#include <algorithm>
#include <string>

std::vector<std::string>	split(const std::string &s, char sep);
bool	                    isEqualToRegex(std::string mask, std::string subString);
bool						checkNick(const std::string nick);
void 						setNickname(const std::string nick);
bool	                    isValidChannelName(const std::string &name);

#endif