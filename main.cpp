#include <filesystem>
#include <csignal>
#include <fstream>

#include "game.hpp"

void sig(int code) {

	// log
	std::ofstream file("./data/log", std::ios_base::app);
	if(code == SIGSEGV)
		file << time(nullptr) << " Segmentation fault"  << '\n';
	else if(code == SIGTERM || code == SIGINT || code == SIGILL)
		file << time(nullptr) << " Unexpected external termination"  << '\n';
	else
		file << time(nullptr) << " Unexpected error"  << '\n';

	file.close();
	std::terminate();
}


int main(int argc, char* argv[]) {

	(void)argc;

	// reset directory
	std::filesystem::current_path(std::filesystem::absolute(argv[0]).parent_path());
	if(!std::filesystem::exists("./data")) {
		std::filesystem::create_directory("data");
	}

	// setup signal handlers
	signal(SIGSEGV, sig); 
	signal(SIGTERM, sig); 
	signal(SIGINT, sig); 
	signal(SIGILL, sig); 
	signal(SIGFPE, sig); 

	try {
		Game game;
		game.mainloop();
	} catch(const std::exception& e) {

		// log
		std::ofstream file("./data/log", std::ios_base::app);
		file << time(nullptr) << ' ' << e.what() << '\n';
	}
}