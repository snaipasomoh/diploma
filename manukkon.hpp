#ifndef _MANUKKON_HPP_
#define _MANUKKON_HPP_
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iostream>

template<class T, class tIter, class Cmp>
tIter binarySearch (tIter l, tIter r, T x, Cmp &cmp){
	while (std::distance(l, r) > 1){
		auto m = l + (r - l) / 2;
		int cmpRes = cmp(*m, x);
		if (cmpRes <= 0){
			l = m;
		}
		else{
			r = m;
		}
	}
	if (cmp(*l, x) > 0){
		return l;
	}
	return r;
}


template<class tIter>
void interchange (tIter first1, tIter last1, tIter first2, tIter last2, tIter &a){
	std::cout << "### Interchange ###" << std::endl;
	if (std::distance(first1, last1) == std::distance(first2, last2)){
		if (first1 <= a && a < last1){
			std::cout << "eq dist from 1 to 2" << std::endl;
			std::advance(a, std::distance(first1, first2) /*+
			                std::distance(first1, a)*/);
		}
		else if (first2 <= a && a < last2){
			std::cout << "eq dist from 2 to 1" << std::endl;
			std::advance(a, std::distance(first2, first1) /*+
			                std::distance(first2, a)*/);
		}
		std::swap_ranges(first1, last1, first2);
	}
	else if (last1 == first2){
		if (first1 <= a && a < last1){
			std::cout << "not eq dist from 1 to 2" << std::endl;
			std::advance(a, std::distance(first2, last2));
		}
		else if (first2 <= a && a < last2){
			std::cout << "not eq dist from 2 to 1" << std::endl;
			std::advance(a, -std::distance(first1, last1));
		}
		std::reverse(first1, last1);
		std::reverse(first2, last2);
		std::reverse(first1, last2);
	}
	else{
		throw("wrong interchange\n");
	}
	std::cout << "###################" << std::endl;
}

template<class tIter>
tIter rbr (tIter it, tIter rs, tIter re){
	auto len = std::distance(rs, re);
	if (len == 0){
		return rs;
	}
	while (it < rs){
		std::advance(it, len);
	}
	while (it > re){
		std::advance(it, -len);
	}
	return it;
}


template<class tIter, class Cmp>
void findAblock (tIter midSec, tIter lastSec, tIter lastAstart, size_t k2,
                size_t lastAsize, tIter &s, tIter &e, Cmp &cmp){
	std::cout << "FIND A BLOCK" << std::endl;
	auto secLen = std::distance(midSec, lastSec);
	size_t blocks = secLen % k2 ? secLen / k2 + 1 : secLen / k2;
	std::cout << "secLen: " << secLen << " blocks: " << blocks << std::endl;
	s = lastAstart;
	e = rbr(lastAstart + lastAsize, midSec, lastSec);
	tIter nexts = s;
	tIter nexte = e;
	// std::cout << *nexts << " " << *(nexte - 1) << std::endl;
	for (size_t i = 0; i < blocks; i++){
		nexts = nexte;
		nexte = rbr(nexte + k2, midSec, lastSec);
		// std::cout << *nexts << " " << *(nexte - 1) << std::endl;
		if (s == lastAstart){
			s = nexts;
			e = nexte;
			continue;
		}
		int cmpRes = cmp(*(e - 1), *(nexte - 1));
		if (cmpRes > 0 || (cmpRes == 0 && cmp(*(s), *(nexts)) > 0)){
			s = nexts;
			e = nexte;
		}
	}
	if (s == lastSec){
		s = midSec;
	}
}


template<class tIter>
void moveAblock (tIter &midSec, tIter lastSec, tIter &lastAstart, size_t k2,
                 size_t lastAsize, size_t  offset, tIter s, tIter e){
	if (s == lastSec){
		return;
	}
	if (1 || k2 == lastAsize){
		std::cout << "MOVE A BLOCK" << std::endl;
		std::cout << "MAB0: " << std::distance(midSec, lastAstart) << std::endl;
		auto len = std::distance(s, e);
		if (s == e){
			interchange(midSec, s, s, lastSec, lastAstart);
			std::cout << "MAB1: " << std::distance(midSec, lastAstart) << std::endl;
			std::advance(midSec, std::distance(midSec, lastSec));
		}
		else if (s > e){
			auto partLen = std::distance(s, lastSec);
			interchange(e, e + partLen, s, s + partLen, lastAstart);
			std::cout << "MAB1: " << std::distance(midSec, lastAstart) << std::endl;
			interchange(midSec, e, e, e + partLen, lastAstart);
			std::cout << "MAB2: " << std::distance(midSec, lastAstart) << std::endl;
			std::advance(midSec, std::distance(midSec, e + partLen));
		}
		else{
			if (std::distance(midSec, s) <= std::distance(midSec, midSec + len)){
				interchange(midSec, s, s, e, lastAstart);
			}
			else{
				interchange(midSec, midSec + len, s, e, lastAstart);
				std::cout << "MAB1: " << std::distance(midSec, lastAstart) << std::endl;
				interchange(lastSec - offset, lastSec, e - offset, e, lastAstart);
				std::cout << "MAB2: " << std::distance(midSec, lastAstart) << std::endl;
				interchange(s, e - offset, e - offset, e, lastAstart);
				std::cout << "MAB3: " << std::distance(midSec, lastAstart) << std::endl;
			}
			std::advance(midSec, len);
			
		}
		return;
	}
}

template<class T, class tIter, class Cmp>
void moveBblock (tIter &midSec, tIter &lastSec, tIter last, size_t &offset,
                 size_t k2, tIter valIt, tIter &lastAstart, Cmp &cmp){
	if (lastSec == last){
		return;
	}
	tIter Bend = binarySearch(lastSec, last, *valIt, cmp);
	size_t Blen = std::distance(lastSec, Bend);
	if (midSec + Blen > lastSec){
		interchange(midSec, lastSec, lastSec, Bend, lastAstart);
	}
	else{
		interchange(midSec, midSec + Blen, lastSec, Bend, lastAstart);
		offset = (offset + Blen) % k2;
	}
	std::advance(midSec, Blen);
	std::advance(lastSec, Blen);
}

// [first; mid) - array1
// [mid; last) - array2
template<class T, class tIter, class Cmp>
void ManUkon (tIter first, tIter mid, tIter last, Cmp &cmp){
	size_t n = std::distance(first, mid);
	// size_t m = std::distance(mid, last);

	size_t k2 = std::ceil(std::sqrt((double)n));
	size_t k = n % k2 ? n / k2 + 1 : n / k2;

	std::cout << k << " " << k2 << std::endl;

	// for (auto i = first; i != last; i++){
	// 	if (i == first){
	// 		std::cout << "# ";
	// 	}
	// 	if (i == mid){
	// 		std::cout << "& ";
	// 	}
	// 	std::cout << *i << " ";
	// }
	// std::cout << std::endl;

	tIter midSec = first;
	tIter lastSec = mid;
	size_t offset = 0;
	tIter lastAstart = first + k2 * (k - 1);
	size_t lastAsize = std::distance(lastAstart, mid);
	for (size_t j = 0; j < k; j++){
		// find A block
		tIter Astart;
		tIter Aend;
		tIter s, m, e;
		findAblock(midSec, lastSec, lastAstart, k2, lastAsize, Astart, Aend, cmp);
		std::cout << "A found " << std::distance(first, Astart) << " " << std::distance(first, Aend) << std::endl;
		std::cout << "las: " << std::distance(first, lastAstart) << std::endl;
		
		s = midSec;
		moveAblock(midSec, lastSec, lastAstart, k2, lastAsize, offset,
		           Astart, Aend);

		m = midSec;
		
		std::cout << "A moved" << std::endl;
		std::cout << "las: " << std::distance(first, lastAstart) << std::endl;

		// for (auto i = first; i != last; i++){
		// 	if (i == midSec){
		// 		std::cout << "# ";
		// 	}
		// 	if (i == lastSec){
		// 		std::cout << "& ";
		// 	}
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;
		
		
		moveBblock<T>(midSec, lastSec, last, offset, k2, midSec - 1,
		              lastAstart, cmp);
		e = midSec;
		std::cout << "B moved" << std::endl;
		std::cout << "las: " << std::distance(first, lastAstart) << std::endl;

		// for (auto i = first; i != last; i++){
		// 	if (i == midSec){
		// 		std::cout << "# ";
		// 	}
		// 	if (i == lastSec){
		// 		std::cout << "& ";
		// 	}
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;

		std::inplace_merge(s, m, e, [&](T const &a, T const &b){return cmp(a, b) < 0;});
		std::cout << "sorted" << std::endl;


		// for (auto i = first; i != last; i++){
		// 	if (i == midSec){
		// 		std::cout << "# ";
		// 	}
		// 	if (i == lastSec){
		// 		std::cout << "& ";
		// 	}
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;
	}
}

#endif

// int main (int argc, char **argv){
// 	std::vector<int> foo = {1, 1, 1, 1, 1, 3, 3, 5, 5, 5, 5, 5, 7, 7, 7, 7, 9, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 6, 6};
// 	// auto bar = binarySearch(foo.begin(), foo.end(), 0, [](int a, int b){return a - b;});
// 	// std::cout << bar - foo.begin() << std::endl;
// 	// std::swap_ranges(foo.begin(), foo.begin() + 5, foo.begin());
// 	ManUkon(foo.begin(), foo.begin() + 17, foo.end(), [](int a, int b){return a - b;});
// // 	std::vector<int>::iterator a, b;
// // 	findAblock(foo.begin(), foo.begin()+2, foo.begin()+1, 3, 2, 1, a, b, [](int a, int b){return a - b;});
// // 	std::cout << std::distance(foo.begin(), a) << std::endl;
// // 	std::cout << std::distance(foo.begin(), b) << std::endl;
// }