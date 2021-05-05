#include "symvonis.hpp"
#include <ctime>
#include "tester.hpp"

int comp (int const &a, int const &b){
	return a - b;
}

int main (int argc, char **argv){
	std::srand(std::time(NULL));
	// std::vector<int> foo = {1, 2, 3, 4, 4, 4, 5, 10};
	// auto res = BLS(foo.begin(), foo.end(), 4, 12, comp);
	// std::cout << std::distance(foo.begin(), res) << std::endl;

	// std::vector<int> foo = {5, 6, 7, 9, 4, 4, 5, 6, 6, 6, 7, 8, 8, 9, 9, 9};
	// std::vector<int> bar = {10, 11, 12, 13, 14};
	// simpleMerge<int>(foo.begin(), foo.begin() + 4, foo.end(), bar.begin(), comp);
	

	// for (auto i : foo){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
	// for (auto i : bar){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;

	// std::vector<int> foo = {1, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5};
	// auto m = foo.begin() + 5;
	// bufferExtraction<int>(foo.begin(), m, foo.end(), comp);

	// for (auto i : foo){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
	// for (auto i = m; i != foo.end(); i++){
	// 	std::cout << *i << " ";
	// }
	// std::cout << std::endl;

	// std::vector<int> bar = {1, 2, 4, 1, 1, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5};
	// bufferDistribution<int>(bar.begin(), bar.begin() + 3, bar.end(), comp);
	// for (auto i : bar){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;





	// std::vector<int> foo/* = {1, 1, 3, 5, 5, 5, 6, 6, 6, 6, 9, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9}*/;
	// std::vector<int> mrg = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	// std::vector<int> sim = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
	// for (size_t i = 0; i < 100; i++){
	// 	foo.push_back(rand() % 10);
	// }
	// for (size_t i = 0; i < 100; i++){
	// 	foo.push_back(rand() % 10);
	// }
	// std::sort(foo.begin(), foo.begin() + 100);
	// std::sort(foo.begin() + 100, foo.end());
	// // stableManUkon<int>(foo.begin(), foo.begin() + 100, foo.end(), mrg.begin(), sim.begin(), 10, comp);
	// symvonis<int>(foo.begin(), foo.begin() + 100, foo.end(), comp);

	// std::cout << std::is_sorted(foo.begin(), foo.end()) << std::endl;

	// for (auto i : foo){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
	// for (auto i : mrg){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
	// for (auto i : sim){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;

	test2(symvonis<Member, std::vector<Member>::iterator, Counter>, 1000, 1000000);

}