#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "manukkon.hpp"
#include "hwanglin.hpp"
#include "tester.hpp"

int main (int argc, char **argv){
	std::srand(std::time(nullptr));
	// Result res = silentTest2(ManUkon<Member, std::vector<Member>::iterator, Counter>, 100000, 100000);
	// size_t testSize = 500;
	// std::cout << std::setprecision(1) << std::fixed;
	// for (size_t i = 0; i < testSize; i++){
	// 	size_t size1 = std::rand() % 2000000;
	// 	size_t size2 = std::rand() % 2000000;
	// 	Result tRes = silentTest2(ManUkon<Member, std::vector<Member>::iterator, Counter>, size1, size2);
	// 	if (tRes < res){
	// 		res = tRes;
	// 	}
	// 	std::cout << ((double)i / testSize) * 100 << "%\r";
	// 	std::cout.flush();
	// }
	// std::cout << "100.0%" << std::endl << res;
	test2(ManUkon<Member, std::vector<Member>::iterator, Counter>, 10000, 100000);
	// test2(HwangLinWrapper<Member, std::vector<Member>::iterator, Counter>, 1000000, 100000);
}