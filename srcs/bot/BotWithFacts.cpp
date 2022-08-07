#include "BotWithFacts.hpp"

BotWithFacts::BotWithFacts(int port, std::string password)
	: Bot("CBot", port, password)  {}

BotWithFacts::~BotWithFacts() {}

bool	BotWithFacts::_isNumber(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}

std::string BotWithFacts::_converter(long src, int toSystem)
{
    char num[1024];
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

std::string	BotWithFacts::_botReply(std::string msg) {
	std::vector<std::string> facts;
	(void) msg;
	facts.push_back("Щенки мужского пола специально дают девочкам-щенкам выигрывать в игривых драках, чтобы узнать их получше.");
	facts.push_back("Существует такое животное, как квокка (короткохостый кенгуру), и оно самое счастливое животное в мире.");
	facts.push_back("Морские коньки моногамны и всю жизнь живут с одним партнером. Они передвигаются, сцепившись хвостами.");
	facts.push_back("Существует бессмертная медуза. Ну, почти. Если не считать, что она может пасть жертвой хищников, медуза вида Turritopsis nutricula может регенерировать клетки постоянно.");
	facts.push_back("Бабочки могут пробовать нектар на вкус своими лапками.");
	facts.push_back("Миллионы деревьев вырастают каждый год, благодаря тому, что белки забывают, куда спрятали свои орехи.");
	facts.push_back("Каланы (морские выдры) держат друг друга за лапки, когда спят, чтобы их не отнесло неизвестно куда течением.");
	facts.push_back("У осьминога прямоугольный зрачок, что дает обзор на все 340 градусов. У человека, кстати, всего 190 градусов.");
	facts.push_back("У панд нет специального места для сна - ни кровати, ни гнезда. Поэтому они засыпают там, где оказались в момент усталости. И если сон сморил их в тот момент, когда они залезли на дерево, ну что ж, так тому и быть.");
	facts.push_back("Альбатрос может спать прямо во время полета. Действует птица по тому же принципу, что и дельфин: в полете у альбатроса отдыхает одна половина мозга, а вторая следит, чтобы он удерживался в воздухе и не залетел в турбину самолета.");
	return (facts[rand() % 10]);
}
