#include "gefkatpas.hpp"
// #include "hwanglin.hpp"
// #include "symvonis.hpp"
// #include "manukkon.hpp"
#include <ctime>
#include "tester.hpp"

int main (int argc, char **argv){
	std::srand(std::time(NULL));
	
	for (size_t i = 3; i < 7; i++){
		for (size_t j = 3; j < 7; j++){
			std::cerr << "n: " << i << std::endl;
			std::cerr << "m: " << j << std::endl;
			complexTest2(gefKatPas<Member, std::vector<Member>::iterator, Counter>, std::pow(10, i), std::pow(10, j), 100);
			std::cerr << "\n\n###########\n\n" << std::endl;
			std::cout << "\n\n\n\n" << std::endl;
		}
	}

}