#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iostream>
#include "manukkon.hpp"
#include "tester.hpp"

int main (int argc, char **argv){
	std::srand(std::time(nullptr));
	test2(ManUkon<Member, std::vector<Member>::iterator, Counter>, 10000, 10000, 100);
}