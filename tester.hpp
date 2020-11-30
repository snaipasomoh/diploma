#ifndef _TESTER_HPP_
#define _TESTER_HPP_
#include <vector>
#include <ctime>
#include <algorithm>
#include <ostream>

struct Member{
	int value;
	size_t arrId;
	size_t id;
};

bool operator< (Member const &a, Member const &b){
	return (a.value - b.value) < 0;
}

std::ostream &operator<< (std::ostream &fout, Member const &a){
	fout << a.value;
	return fout;
}

class Counter{
	size_t comps = 0;

	public:

	size_t getComps () const{
		return comps;
	}

	void reset (){
		comps = 0;
	}

	int operator()(Member const &a, Member const &b){
		comps++;
		return a.value - b.value;
	}
};

// 0 - unordered
// 1 - ordered, unstable
// 2 - ordered, semistable
// 3 - ordered, stable
struct Result{
	char res;
};

bool operator< (Result const &a, Result const &b){
	return a.res < b.res;
}

std::ostream &operator<< (std::ostream &fout, Result const &a){
	if (a.res == 0){
		fout << "Ordered: NO" << std::endl;
	}
	else if (a.res == 1){
		fout << "Ordered: YES" << std::endl;
		fout << "Stability: UNSTABLE" << std::endl;
	}
	else if (a.res == 2){
		fout << "Ordered: YES" << std::endl;
		fout << "Stability: SEMISTABLE" << std::endl;
	}
	else if (a.res == 3){
		fout << "Ordered: YES" << std::endl;
		fout << "Stability: STABLE" << std::endl;
	}
	return fout;
}

std::vector<Member> genArray (size_t size, size_t arrId, size_t range= -1){
	std::vector<Member> res;
	for (size_t i = 0; i < size; i++){
		Member newMember = {(int)(std::rand() % range), arrId, 0};
		res.push_back(newMember);
	}
	std::sort(res.begin(), res.end());
	for (size_t i = 0; i < size; i++){
		res[i].id = 0;
	}
	return res;
}

Result check (std::vector<Member> const &arr, size_t maxId){
	std::vector<bool> arrStab(maxId + 1, 1);
	std::vector<size_t> prevInArr(maxId + 1, 0);
	if (arr.empty()){
		return {3};
	}

	prevInArr[arr[0].arrId] = arr[0].id;
	for (size_t i = 1; i < arr.size(); i++){
		if (arr[i].value < arr[i - 1].value){
			return {0};
		}
		arrStab[arr[i].arrId] = arrStab[arr[i].arrId] &&
		                        arr[i].id >= prevInArr[arr[i].arrId];
		prevInArr[arr[i].arrId] = arr[i].id;
	}

	Result res = {1};
	if (std::find(arrStab.begin(), arrStab.end(), 1) != arrStab.end()){
		if (std::find(arrStab.begin(), arrStab.end(), 0) != arrStab.end()){
			res = {2};
		}
		else{
			res = {3};
		}
	}

	return res;
}

template<class Merger>
void test2 (Merger mrg, size_t size1, size_t size2, size_t range = -1){
	// size1 = 0 calls exception
	std::vector<Member> arr = genArray(size1, 0, range);
	std::vector<Member> tArr = genArray(size2, 1, range);
	arr.insert(arr.end(), tArr.begin(), tArr.end());
	Counter cnt;
	mrg(arr.begin(), arr.begin() + size1, arr.end(), cnt);
	Result res = check(arr, 1);
	std::cout << "###   Test   ###" << std::endl;
	std::cout << "Array1 size: " << size1 << std::endl;
	std::cout << "Array2 size: " << size2 << std::endl;
	// Danger! Overflow is possible in next line!
	std::cout << "Total size: " << size1 + size2 << std::endl;
	std::cout << "Size ratio: " << (double)size1 / (double)size2 << std::endl;
	std::cout << res;
	std::cout << "Comparisons: " << cnt.getComps() << std::endl;
}

template<class Merger>
Result silentTest2 (Merger mrg, size_t size1, size_t size2, size_t range = -1){
	if (size1 == 0){
		return {3};
	}
	std::vector<Member> arr = genArray(size1, 0, range);
	std::vector<Member> tArr = genArray(size2, 1, range);
	arr.insert(arr.end(), tArr.begin(), tArr.end());
	Counter cnt;
	mrg(arr.begin(), arr.begin() + size1, arr.end(), cnt);
	Result res = check(arr, 1);
	return res;
}

#endif