#include "ConvertBot.hpp"

ConvertBot::ConvertBot(int port, std::string password)
	: Bot("CBot", port, password)  {}

ConvertBot::~ConvertBot() {}

bool	ConvertBot::_isNumber(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}

std::string ConvertBot::_converter(long src, int toSystem)
{
    // array to store number
    char num[1024];
 
    // counter for number array
    int crt = 0;
    while (src > 0) {
		int tmp = 0;
		tmp = src % toSystem;
		if (tmp < 10)
			num[crt] = tmp + 48;
		else
			num[crt] = tmp + 55;
		src /= toSystem;
		crt++;
    }
	num[crt] = '\0';

	// reverse
	for(int i = 0; i < crt / 2; i++){
        char tmp = num[i];
        num[i] = num[crt - i - 1];
        num[crt - i - 1] = tmp;
	}
	std::string answer = std::string(num);
	if (answer.empty())
		return "0";
	return answer;
}
std::string	ConvertBot::_botReply(std::string msg) {
	std::cout << "I'm in method botReply\n";
	if (!_isNumber(msg))
		return BOT_ERROR_MSG;
	long src = atoi(msg.c_str());
	if (src > INT_MAX || src < 0)
		return BOT_ERROR_MSG;
	std::string answer =	"dec: " + msg + 
							"\nbin: " + _converter(src, 2) +
							"\noct: " + _converter(src, 8) +
							"\nhex: " + _converter(src, 16);
	return answer;
}
