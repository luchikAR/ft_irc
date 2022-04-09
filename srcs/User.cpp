
#include "../include/User.hpp"


User::User() { }

User::~User() { }

int User::setNick(char* nick) {
    this->_nick = nick;
    return (1);
}

int User::setUsername(char* username) {
    this->_user_name = username;
    return (1);
}