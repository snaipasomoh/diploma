#include "gefkatpas.hpp"

int comp (int const &a, int const &b){
	return a - b;
}

int main (int argc, char **argv){
	std::vector<int> foo = {1, 3, 3, 3, 5, 7, 9, 11, 13, 15, 17, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	gefKatPas<int>(foo.begin(), foo.begin() + 11, foo.end(), comp);
}