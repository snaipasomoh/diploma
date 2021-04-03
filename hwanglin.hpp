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

		auto b2_iter = std::upper_bound(b2, b2 + bound, *b1,
		                    [&](T const &a, T const &b){return cmp(a, b) < 0;});
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

template<class T, class tIter, class Cmp>
void HwangLin2 (tIter b1, tIter e1, tIter b2, tIter e2, tIter out, Cmp &cmp){
	size_t m, n;
	tIter ab = b1, ae = e1, bb = b2, be = e2;
	tIter outb = out;
	m = std::distance(b1, e1);
	n = std::distance(b2, e2);
	while (m > 0 && n > 0){
		// std::cout << m << " " << n << std::endl;
		if (m > n){
			std::swap(m, n);
			std::swap(ab, bb);
			std::swap(ae, be);
			// std::cout << "swapped " << m << " " << n << std::endl;
		}
		// std::cout << m << " " << n << std::endl;
		// std::cout << "a: ";
		// for (auto i = ab; i != ae; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;
		// std::cout << "b: ";
		// for (auto i = bb; i != be; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;
		// std::cout << "out: ";
		// for (auto i = outb; i != out; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl << std::endl;;

		unsigned a = floor(log2((double)n / m));
		size_t x = n - pow(2, a) /*+ 1*/;
		// std::cout << "comp ";
		int cmpRes = cmp(*(ae - 1), *(bb + x));
		// std::cout << "ok" << std::endl;
		if (cmpRes < 0){
			size_t len = std::distance(bb + x, be);
			std::reverse_copy(bb + x, be, out);
			out += len;
			be = bb + x;
			// std::cout << "less" << std::endl;
		}
		else{
			auto border = std::lower_bound(bb + x, be, *(ae - 1), 
			                [&](T const &a, T const &b){return cmp(a, b) < 0;});
			size_t len = std::distance(border, be);
			std::reverse_copy(border, be, out);
			out += len;
			std::copy(ae - 1, ae, out);
			out += 1;
			be = border;
			ae -= 1;
			// std::cout << "not less" << std::endl;
		}
		m = std::distance(ab, ae);
		n = std::distance(bb, be);
	}
	if (m > 0){
		std::reverse_copy(ab, ae, out);
		out += m;
	}
	else if (n > 0){
		std::reverse_copy(bb, be, out);
		out += n;
	}
	std::reverse(outb, out);
}

template<class T, class tIter, class Cmp>
void HwangLinWrapper2 (tIter first, tIter mid, tIter last, Cmp &cmp){
	std::vector<T> temp (std::distance(first, last));
	HwangLin2<T>(first, mid, mid, last, temp.begin(), cmp);
	std::swap_ranges(first, last, temp.begin());
}

#endif