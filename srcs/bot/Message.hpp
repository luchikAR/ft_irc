#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include <string>
#include <vector>

class Message {
	public:
		Message();
		Message(std::string & mess);
		~Message();
		std::string & getPrefix();
		std::string & getCommand();
		std::vector<std::string> & getParams();


	private:
		std::string _prefix;
		std::string _command;
		std::vector<std::string> _params;

		Message(const Message& copy);
		Message	&operator=(const Message& other);

};


#endif
