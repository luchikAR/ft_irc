#include "Bot.hpp"
#include "BotWithFacts.hpp"

static void mainError() {
		std::cerr << "Usage: ./bot bot <port> <password>" << std::endl;
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
		if ("bot" == bot_name)
			bot = new BotWithFacts(port, password);
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