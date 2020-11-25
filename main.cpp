#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iostream>
#include "manukkon.hpp"
#include "tester.hpp"

int main (int argc, char **argv){
	test2(ManUkon<Member, std::vector<Member>::iterator, Counter>, 16, 20);
}