#include "gefkatpas3.hpp"
#include <ctime>
#include "tester.hpp"

int comp (int const &a, int const &b){
	return a - b;
}

int main (int argc, char **argv){
	std::srand(std::time(NULL));
	// std::srand(0);
	// std::vector<int> foo = {1, 3, 3, 3, 5, 7, 9, 11, 13, 15, 17, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4};
	// gefKatPas<int>(foo.begin(), foo.begin() + 11, foo.end(), comp);
	test2(gefKatPas<Member, std::vector<Member>::iterator, Counter>, 1000, 10000, 100);

	// std::vector<int> foo = {0, 21, 22, 23, 24, 28, 33, 34, 36, 43, 51, 53, 56, 60, 60, 69, 69, 79, 80, 84, 87, 97, 5, 8, 10, 10, 11, 13, 14, 21, 21, 22, 27, 30, 31, 36, 36, 37, 38, 42, 43, 44, 48, 49, 51, 52, 57, 57, 64, 65, 66, 67, 74, 74, 75, 76, 79, 80, 86, 92, 93, 93, 93, 98};
	// gefKatPas<int>(foo.begin(), foo.begin() + 22, foo.end(), comp);
}