#include "Message.hpp"

Message::Message() {}

Message::Message(std::string & mess) {
	size_t i = 0;
	size_t j = 0;
	while (mess.length() > 0) {
		i = mess.find(" ");
		if (i == std::string::npos && j == 0 ) {
			_command = mess;
			transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
			mess.clear();
		}
		else if (i == 0) {
			mess.erase(0, 1);
		}
		else if ( (i == std::string::npos && j > 0) || (j > 0 && mess.find(":") == 0 )) {
			if (mess.find(":") == 0)
				mess.erase(0, 1);
			_params.push_back(mess);
			mess.clear();
		}
		else if (j == 0 && mess.find(":") == 0) {
			_prefix = mess.substr(1, i - 1);
			mess.erase(0, i + 1);
		}
		else if ( j == 0 && _command.length() == 0) {
			_command = mess.substr(0, i);
			transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
			mess.erase(0, i + 1);
			++j;
		}
		else {
			_params.push_back(mess.substr(0, i));
			mess.erase(0, i + 1);
			++j;
		}
	}
}

Message::~Message() {}

std::string & Message::getPrefix() {
	return _prefix;
}

std::string & Message::getCommand() {
	return _command;
}

std::vector<std::string> & Message::getParams() {
	return _params;
}
