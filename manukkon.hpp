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
	if (std::distance(first1, last1) == std::distance(first2, last2)){
		if (first1 <= a && a < last1){
			std::advance(a, std::distance(first1, first2));
		}
		else if (first2 <= a && a < last2){
			std::advance(a, std::distance(first2, first1));
		}
		std::swap_ranges(first1, last1, first2);
	}
	else if (last1 == first2){
		if (first1 <= a && a < last1){
			std::advance(a, std::distance(first2, last2));
		}
		else if (first2 <= a && a < last2){
			std::advance(a, -std::distance(first1, last1));
		}
		std::reverse(first1, last1);
		std::reverse(first2, last2);
		std::reverse(first1, last2);
	}
	else{
		throw("wrong interchange\n");
	}
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

template<class tIter>
typename std::iterator_traits<tIter>::difference_type dbr (tIter s, tIter e,
                                                           tIter rs, tIter re){
	auto len = std::distance(rs, re);
	while (e < s){
		std::advance(e, len);
	}
	return std::distance(s, e);
}

template<class tIter, class Cmp>
void findAblock (tIter midSec, tIter lastSec, tIter lastAstart, size_t k2,
                size_t lastAsize, size_t &offset, tIter &s, tIter &e, Cmp &cmp){
	auto secLen = std::distance(midSec, lastSec);
	size_t blocks = secLen % k2 ? secLen / k2 + 1 : secLen / k2;
	s = lastAstart;
	e = rbr(lastAstart + lastAsize, midSec, lastSec);
	auto leastOffset = std::distance(midSec, e);
	tIter nexts = s;
	tIter nexte = e;
	for (size_t i = 0; i < blocks - 1; i++){
		nexts = nexte;
		nexte = rbr(nexte + k2, midSec, lastSec);

		auto currOffset = std::distance(midSec, nexte);
		if (currOffset < leastOffset){
			leastOffset = currOffset;
		}
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
	offset = leastOffset;
}

template<class tIter>
void moveAblock (tIter &midSec, tIter lastSec, tIter &lastAstart, size_t k2,
                 size_t lastAsize, size_t  offset, tIter s, tIter e){
	auto blockLen = dbr(s, e, midSec, lastSec);
	if (s == lastSec){
		return;
	}
	if (s == midSec){
		std::advance(midSec, blockLen);
		return;
	}

	if (k2 != lastAsize && s != lastAstart){
		if (s < e){
			if (midSec == lastAstart){
				if (lastAstart + lastAsize == s){
					interchange(midSec, s, s, e, lastAstart);
					std::advance(midSec, blockLen);
					return;
				}
				auto tm = midSec + lastAsize;
				auto te = tm + k2;
				interchange(midSec, tm, tm, te, lastAstart);
				offset = k2;
			}
			else if (std::distance(lastAstart, lastSec) <
			         dbr(lastAstart, lastAstart + lastAsize, midSec, lastSec)){
				if (rbr(lastAstart + lastAsize, midSec, lastSec) == s){
					interchange(midSec, s, s, e, lastAstart);
					std::advance(midSec, blockLen);
					return;
				}
				auto tm = rbr(lastAstart + lastAsize, midSec, lastSec);
				interchange(midSec, tm, tm, tm + k2, lastAstart);
				offset = k2;
			}
			else if (midSec < lastAstart && lastAstart < midSec + blockLen){
				if (lastAstart + lastAsize == s){
					interchange(lastAstart, s, s, e, lastAstart);
					std::advance(s, -lastAsize);
					std::advance(e, -lastAsize);
				}
				else{
					auto tm = lastAstart + lastAsize;
					auto te = tm + k2;
					interchange(lastAstart, tm, tm, te, lastAstart);
				}
			}
		}
		else{
			if (e == lastAstart){
				if (lastAstart + lastAsize == s){
					interchange(lastAstart, s, s, lastSec, lastAstart);
					interchange(midSec, e, e, lastAstart, lastAstart);
					std::advance(midSec, blockLen);
					return;
				}
				auto tm = lastAstart + lastAsize;
				auto te = tm + k2;
				interchange(lastAstart, tm, tm, te, lastAstart);
			}
		}
	}

	if (s == e){
		interchange(midSec, s, s, lastSec, lastAstart);
		std::advance(midSec, std::distance(midSec, lastSec));
		return;
	}
	else if (s > e){
		auto partLen = std::distance(s, lastSec);
		interchange(e, e + partLen, s, s + partLen, lastAstart);
		interchange(midSec, e, e, e + partLen, lastAstart);
	}
	else if (std::distance(midSec, s) <= blockLen){
		interchange(midSec, s, s, e, lastAstart);
	}
	else{
		if (offset % k2){
			interchange(midSec, midSec + blockLen, s, e, lastAstart);
			size_t partLen = k2 - offset;
			interchange(e - partLen, e, lastSec - partLen, lastSec, lastAstart);
			interchange(s, e - partLen, e - partLen, e, lastAstart);
		}
		else{
			interchange(midSec, midSec + blockLen, s, e, lastAstart);
		}
		
	}
	std::advance(midSec, blockLen);
}

template<class T, class tIter, class Cmp>
void moveBblock (tIter &midSec, tIter &lastSec, tIter last,
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
	}
	std::advance(midSec, Blen);
	std::advance(lastSec, Blen);
}

// [first; mid) - array1
// [mid; last) - array2
template<class T, class tIter, class Cmp>
void ManUkon (tIter first, tIter mid, tIter last, Cmp &cmp){
	size_t n = std::distance(first, mid);

	size_t k2 = std::ceil(std::sqrt((double)n));
	size_t k = n % k2 ? n / k2 + 1 : n / k2;

	tIter midSec = first;
	tIter lastSec = mid;
	size_t offset = 0;
	tIter lastAstart = first + k2 * (k - 1);
	size_t lastAsize = std::distance(lastAstart, mid);
	for (size_t j = 0; j < k; j++){
		tIter Astart;
		tIter Aend;
		tIter s, m, e;
		findAblock(midSec, lastSec, lastAstart, k2, lastAsize, offset,
		           Astart, Aend, cmp);
		s = midSec;
		moveAblock(midSec, lastSec, lastAstart, k2, lastAsize, offset,
		           Astart, Aend);
		m = midSec;
		moveBblock<T>(midSec, lastSec, last, k2, midSec - 1, lastAstart, cmp);
		e = midSec;
		std::inplace_merge(s, m, e,
		                   [&](T const &a, T const &b){return cmp(a, b) < 0;});
	}
}

#endif