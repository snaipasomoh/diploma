#ifndef _TESTER_HPP_
#define _TESTER_HPP_
#include <vector>
#include <ctime>
#include <algorithm>
#include <ostream>
#include <iomanip>

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
	size_t cmps;
	double time;
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
		res[i].id = i;
	}
	return res;
}

std::vector<Member> genMaxMixedSeq (size_t size1, size_t size2, size_t range = -1){
	std::vector<Member> res;
	for (size_t i = 0; i < size1 + size2; i++){
		Member newMember = {(int)(std::rand() % range), 0, 0};
		res.push_back(newMember);
	}
	std::sort(res.begin(), res.end());

	std::vector<Member> arr1;
	std::vector<Member> arr2;
	if (size1 <= size2){
		size_t stepSize = (size1 + size2) / size1;
		for (size_t i = 0; i < size1 + size2; i++){
			if (arr1.size() < size1 && i % stepSize == 0){
				arr1.push_back(res[i]);
			}
			else{
				arr2.push_back(res[i]);
			}
		}
	}
	else{
		size_t stepSize = (size1 + size2) / size2;
		for (size_t i = 0; i < size1 + size2; i++){
			if (arr2.size() < size2 && i % stepSize == 0){
				arr2.push_back(res[i]);
			}
			else{
				arr1.push_back(res[i]);
			}
		}
	}
	for (size_t i = 0; i < arr1.size(); i++){
		arr1[i].id = i;
		arr1[i].arrId = 0;
	}
	for (size_t i = 0; i < arr2.size(); i++){
		arr2[i].id = i;
		arr2[i].arrId = 1;
	}
	res.clear();
	res.insert(res.end(), arr1.begin(), arr1.end());
	res.insert(res.end(), arr2.begin(), arr2.end());
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

	Result res;
	res.res = 1;
	if (std::find(arrStab.begin(), arrStab.end(), 1) != arrStab.end()){
		if (std::find(arrStab.begin(), arrStab.end(), 0) != arrStab.end()){
			res.res = 2;
		}
		else{
			res.res = 3;
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
	// for (auto i : arr){
	// 	std::cout << i.value << " ";
	// }
	// std::cout << std::endl;
	Counter cnt;
	auto t0 = std::clock();
	mrg(arr.begin(), arr.begin() + size1, arr.end(), cnt);
	auto t1 = std::clock();
	// for (auto i : arr){
	// 	std::cout << i.value << " ";
	// }
	// std::cout << std::endl;
	Result res = check(arr, 1);
	res.cmps = cnt.getComps();
	res.time = double(t1 - t0) / CLOCKS_PER_SEC;
	std::cout << "###   Test   ###" << std::endl;
	std::cout << "Array1 size: " << size1 << std::endl;
	std::cout << "Array2 size: " << size2 << std::endl;
	// Danger! Overflow is possible in next line!
	std::cout << "Total size: " << size1 + size2 << std::endl;
	std::cout << "Size ratio: " << (double)size1 / (double)size2 << std::endl;
	std::cout << res;
	std::cout << "Comparisons: " << cnt.getComps() << std::endl;
	std::cout << "Merge time: " << double(t1 - t0) / CLOCKS_PER_SEC << std::endl;
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
	auto t0 = std::clock();
	mrg(arr.begin(), arr.begin() + size1, arr.end(), cnt);
	auto t1 = std::clock();
	Result res = check(arr, 1);
	res.cmps = cnt.getComps();
	res.time = double(t1 - t0) / CLOCKS_PER_SEC;
	return res;
}

double origTest (std::vector<Member> const &seq, std::vector<Member> const &orig){
	size_t N = seq.size();
	size_t good = 0;
	for (size_t i = 0; i < N; i++){
		if (seq[i].arrId == orig[i].arrId && seq[i].id == orig[i].id){
			good++;
		}
	}
	return (double)good / N;
}

template<class Merger>
void complexTest2 (Merger mrg, size_t size1, size_t size2, size_t tests, size_t range = -1){
	std::vector<double> Times;
	std::vector<size_t> Comps;
	std::vector<Member> minTimeSeq;
	std::vector<Member> maxTimeSeq;
	std::vector<Member> minCompSeq;
	std::vector<Member> maxCompSeq;
	double minTime;
	double maxTime;
	double minComp;
	double maxComp;
	double minTimeOrig;
	double maxTimeOrig;
	double minCompOrig;
	double maxCompOrig;
	double ext1Time;
	double ext2Time;
	double ext3Time;
	size_t ext1Comp;
	size_t ext2Comp;
	size_t ext3Comp;
	clock_t t0;
	clock_t t1;
	Counter cnt;
	std::vector<Member> currSeq;
	std::vector<Member> currSeqOrig;
	double currTime;
	size_t currComp;
	double currOrig;

	//extra-case 1
	for (size_t i = 0; i < size1 + size2; i++){
		Member newMember = {(int)(std::rand() % range), 0, 0};
		currSeq.push_back(newMember);
	}
	std::sort(currSeq.begin(), currSeq.end());
	for (size_t i = 0; i < size1; i++){
		currSeq[i].id = i;
	}
	for (size_t i = 0; i < size2; i++){
		currSeq[size1 + i].id = i;
		currSeq[size1 + i].arrId = 1;
	}
	currSeqOrig = currSeq;
	t0 = std::clock();
	mrg(currSeq.begin(), currSeq.begin() + size1, currSeq.end(), cnt);
	t1 = std::clock();
	if (check(currSeq, 1).res == 0){
		std::cerr << "EXTRA_CASE_1 UNORDERED" << std::endl;
		ext1Time = 0;
		ext1Comp = 0;
		goto extra2;
	}
	currTime = double(t1 - t0) / CLOCKS_PER_SEC;
	currComp = cnt.getComps();
	currOrig = origTest(currSeq, currSeqOrig);
	ext1Time = currTime;
	ext1Comp = currComp;
	Times.push_back(currTime);
	Comps.push_back(currComp);
	minTime = currTime;
	maxTime = currTime;
	minComp = currComp;
	maxComp = currComp;
	minTimeOrig = currOrig;
	maxTimeOrig = currOrig;
	minCompOrig = currOrig;
	maxCompOrig = currOrig;
	minTimeSeq = currSeq;
	maxTimeSeq = currSeq;
	minCompSeq = currSeq;
	maxCompSeq = currSeq;


	//extra-case 2
	extra2:
	currSeq.clear();
	cnt.reset();
	for (size_t i = 0; i < size1 + size2; i++){
		Member newMember = {(int)(std::rand() % range), 1, 0};
		currSeq.push_back(newMember);
	}
	std::sort(currSeq.begin(), currSeq.end());
	for (size_t i = 0; i < size2; i++){
		currSeq[i].id = i;
	}
	for (size_t i = 0; i < size1; i++){
		currSeq[size2 + i].id = i;
		currSeq[size2 + i].arrId = 0;
	}
	std::reverse(currSeq.begin(), currSeq.begin() + size2);
	std::reverse(currSeq.begin() + size2, currSeq.end());
	std::reverse(currSeq.begin(), currSeq.end());
	currSeqOrig = currSeq;
	t0 = std::clock();
	mrg(currSeq.begin(), currSeq.begin() + size1, currSeq.end(), cnt);
	t1 = std::clock();
	if (check(currSeq, 1).res == 0){
		std::cerr << "EXTRA_CASE_2 UNORDERED" << std::endl;
		ext2Time = 0;
		ext2Comp = 0;
		goto extra3;
	}
	currTime = double(t1 - t0) / CLOCKS_PER_SEC;
	currComp = cnt.getComps();
	currOrig = origTest(currSeq, currSeqOrig);
	ext2Time = currTime;
	ext2Comp = currComp;
	Times.push_back(currTime);
	Comps.push_back(currComp);
	if (currTime < minTime){
		minTime = currTime;
		minTimeOrig = currOrig;
		minTimeSeq = currSeq;
	}
	if (currTime > maxTime){
		maxTime = currTime;
		maxTimeOrig = currOrig;
		maxTimeSeq = currSeq;
	}
	if (currComp < minComp){
		minComp = currComp;
		minCompOrig = currOrig;
		minCompSeq = currSeq;
	}
	if (currComp > maxComp){
		maxComp = currComp;
		maxCompOrig = currOrig;
		maxCompSeq = currSeq;
	}

	
	//extra-case 3
	extra3:
	currSeq.clear();
	cnt.reset();
	currSeq = genMaxMixedSeq(size1, size2, range);
	currSeqOrig = currSeq;
	t0 = std::clock();
	mrg(currSeq.begin(), currSeq.begin() + size1, currSeq.end(), cnt);
	t1 = std::clock();
	if (check(currSeq, 1).res == 0){
		std::cerr << "EXTRA_CASE_3 UNORDERED" << std::endl;
		ext3Time = 0;
		ext3Comp = 0;
		goto def;
	}
	currTime = double(t1 - t0) / CLOCKS_PER_SEC;
	currComp = cnt.getComps();
	currOrig = origTest(currSeq, currSeqOrig);
	ext3Time = currTime;
	ext3Comp = currComp;
	Times.push_back(currTime);
	Comps.push_back(currComp);
	if (currTime < minTime){
		minTime = currTime;
		minTimeOrig = currOrig;
		minTimeSeq = currSeq;
	}
	if (currTime > maxTime){
		maxTime = currTime;
		maxTimeOrig = currOrig;
		maxTimeSeq = currSeq;
	}
	if (currComp < minComp){
		minComp = currComp;
		minCompOrig = currOrig;
		minCompSeq = currSeq;
	}
	if (currComp > maxComp){
		maxComp = currComp;
		maxCompOrig = currOrig;
		maxCompSeq = currSeq;
	}

	std::cerr << "Extra-cases done" << std::endl;

	//default cases
	def:
	for (size_t i = 0; i < tests; i++){
		currSeq.clear();
		cnt.reset();
		currSeq = genArray(size1, 0, range);
		currSeqOrig = genArray(size2, 1, range);
		currSeq.insert(currSeq.end(), currSeqOrig.begin(), currSeqOrig.end());
		currSeqOrig = currSeq;

		t0 = std::clock();
		mrg(currSeq.begin(), currSeq.begin() + size1, currSeq.end(), cnt);
		t1 = std::clock();
		if (check(currSeq, 1).res == 0){
			std::cerr << "DEFAULT CASE UNORDERED" << std::endl;
			continue;
		}
		currTime = double(t1 - t0) / CLOCKS_PER_SEC;
		currComp = cnt.getComps();
		currOrig = origTest(currSeq, currSeqOrig);
		Times.push_back(currTime);
		Comps.push_back(currComp);
		if (currTime < minTime){
			minTime = currTime;
			minTimeOrig = currOrig;
			minTimeSeq = currSeq;
		}
		if (currTime > maxTime){
			maxTime = currTime;
			maxTimeOrig = currOrig;
			maxTimeSeq = currSeq;
		}
		if (currComp < minComp){
			minComp = currComp;
			minCompOrig = currOrig;
			minCompSeq = currSeq;
		}
		if (currComp > maxComp){
			maxComp = currComp;
			maxCompOrig = currOrig;
			maxCompSeq = currSeq;
		}
		std::cerr << std::setprecision(0) << std::fixed << ((double)i / tests) * 100 << "% of default cases done\r";
		std::cerr.flush();
	}
	std::cerr.clear();
	std::cerr << "100% of default cases done" << std::endl;
	

	//result
	std::cout << "### TEST RESULT ###" << std::endl;
	std::cout << "Array1 size: " << size1 << std::endl;
	std::cout << "Array2 size: " << size2 << std::endl;
	// Danger! Overflow is possible in next line!
	std::cout << "Total size: " << size1 + size2 << std::endl;
	std::cout << "Size ratio: " << (double)size1 / (double)size2 << std::endl;
	std::cout << std::endl;
	std::cout << Times.size() << " of " << tests + 3 << " OK" << std::endl;
	std::cout << std::endl;

	std::cout << "Extra-case 1 time: " << ext1Time << std::endl;
	std::cout << "Extra-case 1 compares: " << ext1Comp << std::endl;
	std::cout << "Extra-case 2 time: " << ext2Time << std::endl;
	std::cout << "Extra-case 2 compares: " << ext2Comp << std::endl;
	std::cout << "Extra-case 3 time: " << ext3Time << std::endl;
	std::cout << "Extra-case 3 compares: " << ext3Comp << std::endl;

	std::cout << "Min time: " << minTime << " sec on sequence:" << std::endl;
	for (Member &i : minTimeSeq){
		std::cout << i.arrId << " ";
	}
	std::cout << std::endl;
	std::cout << "with saving " << minTimeOrig * 100 << "% element's places" << std::endl;
	std::cout << std::endl;

	std::cout << "Max time: " << maxTime << " sec on sequence:" << std::endl;
	for (Member &i : maxTimeSeq){
		std::cout << i.arrId << " ";
	}
	std::cout << std::endl;
	std::cout << "with saving " << maxTimeOrig * 100 << "% element's places" << std::endl;
	std::cout << std::endl;

	std::cout << "Min compares: " << minComp << " on sequence:" << std::endl;
	for (Member &i : minCompSeq){
		std::cout << i.arrId << " ";
	}
	std::cout << std::endl;
	std::cout << "with saving " << minCompOrig * 100 << "% element's places" << std::endl;
	std::cout << std::endl;

	std::cout << "Max compares: " << maxComp << " on sequence:" << std::endl;
	for (Member &i : maxCompSeq){
		std::cout << i.arrId << " ";
	}
	std::cout << std::endl;
	std::cout << "with saving " << maxCompOrig * 100 << "% element's places" << std::endl;
	std::cout << std::endl;

	std::sort(Times.begin(), Times.end());
	std::sort(Comps.begin(), Comps.end());
	size_t N = Times.size();
	double avTime = 0;
	double avComp = 0;
	for (size_t i = 0; i < N; i++){
		avTime += Times[i] / N;
		avComp += (double)Comps[i] / N;
	}
	
	std::cout << "Average time: " << avTime << std::endl;
	std::cout << "Median time: " << Times[N / 2 + 1] << std::endl;
	std::cout << "Average compares: " << avComp << std::endl;
	std::cout << "Median compares: " << Comps[N / 2 + 1] << std::endl;
}

#endif