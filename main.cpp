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


	// for (size_t i = 3; i < 7; i++){
	// 	for (size_t j = 3; j < 7; j++){
	// 		std::cout << "n: " << i << std::endl;
	// 		std::cout << "m: " << j << std::endl;
	// 		double time = 0;
	// 		double cmps = 0;
	// 		for(size_t k = 0; k < 10; k++){
	// 			Result res = silentTest2(ManUkon<Member, std::vector<Member>::iterator, Counter>, std::pow(10, i), std::pow(10, j));
	// 			std::cout << std::endl << std::endl;
	// 			time += res.time / 10;
	// 			cmps += (double)res.cmps / 10;
	// 		}
	// 		std::cout << "\n\n AVERAGE:" << std::endl;
	// 		std::cout << "Time: " << time << std::endl;
	// 		std::cout << "Compares:" << cmps << std::endl;
	// 		std::cout << "\n\n\n###########\n\n" << std::endl;
	// 	}
	// }


	test2(ManUkon<Member, std::vector<Member>::iterator, Counter>, 1000, 9000);
	test2(ManUkon<Member, std::vector<Member>::iterator, Counter>, 4500, 5500);
	// test2(HwangLinWrapper<Member, std::vector<Member>::iterator, Counter>, 1000000, 100000);
}