#ifndef _HWANGLIN_HPP_
#define _HWANGLIN_HPP_
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

template<class tIter>
void dump (tIter b, tIter e, tIter out){
	while (b != e){
		*out++ = *b++;
	}
}

template<class T, class tIter, class Cmp>
void HwangLin (tIter b1, tIter e1, tIter b2, tIter e2, tIter out, Cmp &cmp){
	while (b1 != e1 && b2 != e2){
		int bound = 3;
		while ((b2 + bound + 1 <= e2) && cmp(*(b2 + bound), *b1) < 0){
			auto tmp_iter = b2 + bound;
			while (b2 <= tmp_iter){
				*out++ = *b2++;
			}
		}
		bound = std::min(bound, (int)(e2 - b2));

		auto b2_iter = std::upper_bound(b2, b2 + bound, *b1, [&](T const &a, T const &b){return cmp(a, b) < 0;});
		while (b2 < b2_iter){
			*out++ = *b2++;
		}
		*out++ = *b1++;
	}
	if (b1 == e1){
		dump(b2, e2, out);
		return;
	}
	if (b2 == e2){
		dump(b1, e1, out);
		return;
	}
}

template<class T, class tIter, class Cmp>
void HwangLinWrapper (tIter first, tIter mid, tIter last, Cmp &cmp){
	std::vector<T> temp (std::distance(first, last));
	HwangLin<T>(first, mid, mid, last, temp.begin(), cmp);
	std::swap_ranges(first, last, temp.begin());
}

#endif