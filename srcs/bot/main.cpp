#include "Bot.hpp"
//#include "Models/WeatherBot.hpp"
//#include "Models/ApodBot.hpp"
#include "ConvertBot.hpp"

// You can try my bots in console.
// 1) Go to srcs/Bots/scripts and type: "./weather_bot.py [city]".
// Parameter (city) can be in two languages.
// Bot send information about weather

// 2) Go to srcs/Bots/scripts and type: "./apod_bot.py [count]".
// Parameter (count) must be between 1 and 5.
// Bot send Astronomy Pictures of the Day. (url to picture)
static void mainError() {
		std::cerr << "Usage: ./bot <bot_name> <port> <password>" << std::endl;
		std::cerr << "Available bot types:" << std::endl;
		std::cerr << "	convert" << std::endl;
}


int main(int argc, char* argv[]) {
	if (argc != 4) {
		mainError();
		return 1;
	}

	try {
		std::string bot_name = argv[1];
		int port = std::atoi(argv[2]);
		std::string password = argv[3];
		
		Bot* bot;
		if ("convert" == bot_name)
			bot = new ConvertBot(port, password);
		else {
			mainError();
			return 1;
		}
		bot->run();
	} catch (const std::exception & e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return 1;
	}
}