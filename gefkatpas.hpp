#ifndef _GEFKATPAS_HPP_
#define _GEFKATPAS_HPP_
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iostream>
#include <iomanip>


// advance-by-range
template<class tIter>
void abr (tIter l, tIter r, tIter &pointer, size_t n){
	size_t dist = n % std::distance(l, r);
	for (size_t i = 0; i < dist; i++){
		if (pointer == r){
			pointer = l;
		}
		pointer++;
	}
}

template<class T, class tIter, class Cmp>
tIter findPrefixBorder (tIter l, tIter r, tIter x, size_t s, Cmp &cmp){
	if (l == r){
		return r;
	}
	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	tIter currL = l;
	tIter currR = l + s;
	while (currR <= r){
		if (cmp(*x, *(currR - 1)) <= 0){
			return std::lower_bound(currL, currR, *x, defaultCMP);
		}
		currL = currR;
		std::advance(currR, s);
	}
	return std::lower_bound(currL, r, *x, defaultCMP);
}

template<class tIter, class Cmp>
tIter findNextXBlk (tIter OcB, tIter OcE, tIter YcB, tIter EcB,
    tIter EcE, tIter ec, tIter f, size_t s, Cmp &cmp){
	tIter res;
	tIter l;
	tIter r;
	bool escapeIsEmpty = ec == EcB ? 1 : 0;
	tIter currBlk;

	if (escapeIsEmpty){
		currBlk = OcE;
		l = OcB;
		r = OcE - 1;
		res = OcB;
	}
	else{
		currBlk = OcE;
		l = EcB;
		r = OcE - 1;
		res = EcB;
	}
	while (currBlk < YcB){
		if (currBlk == f || currBlk == EcB){
			std::advance(currBlk, s);
			continue;
		}
		int cmpRes = cmp(*currBlk, *l);
		if (cmpRes < 0){
			res = currBlk;
			l = currBlk;
			r = currBlk + s - 1;
		}
		else if (cmpRes == 0){
			int cmpRes2 = cmp(*(currBlk + s - 1), *r);
			if (cmpRes2 < 0){
				res = currBlk;
				l = currBlk;
				r = currBlk + s - 1;
			}
		}
		std::advance(currBlk, s);
	}
	return res;
}

template<class tIter, class Cmp>
tIter findNextXBlkM (tIter OcB, tIter OcE, tIter YcB, tIter EcB,
    tIter EcE, tIter ec, tIter f, size_t s, Cmp &cmp){
	tIter res;
	tIter l;
	tIter r;
	bool escapeIsEmpty = ec == EcB ? 1 : 0;
	tIter currBlk;

	currBlk = OcE;
	l = OcE;
	r = OcE + s - 1;
	res = OcE;

	while (currBlk < YcB){
		if (currBlk == f || currBlk == EcB){
			std::advance(currBlk, s);
			continue;
		}
		int cmpRes = cmp(*currBlk, *l);
		if (cmpRes < 0){
			res = currBlk;
			l = currBlk;
			r = currBlk + s - 1;
		}
		else if (cmpRes == 0){
			int cmpRes2 = cmp(*(currBlk + s - 1), *r);
			if (cmpRes2 < 0){
				res = currBlk;
				l = currBlk;
				r = currBlk + s - 1;
			}
		}
		std::advance(currBlk, s);
	}
	return res;
}

template<class T, class tIter, class Cmp>
void bufferDistribution (tIter l, tIter m, tIter r, Cmp &cmp){
	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	size_t N = std::distance(m, r);
	size_t M = std::distance(l, m);
	size_t blockSize = std::ceil((double)N / M);
	size_t bufSize = M;

	tIter currL = m;
	tIter currR = m + blockSize;
	tIter bufStart = l;
	tIter currBstart = m;
	while (bufSize > 0 && currL < r){
		if (currR > r){
			currR = r;
		}
		if (currL < currBstart){
			currL = currBstart;
		}
		if (cmp(*(currR - 1), *bufStart) >= 0){
			tIter swapBorder = std::lower_bound(currL, currR, *bufStart,
			    defaultCMP);
			size_t swapSize = std::distance(currBstart, swapBorder);
			std::reverse(bufStart, bufStart + bufSize);
			std::reverse(currBstart, swapBorder);
			std::reverse(bufStart, swapBorder);
			std::advance(bufStart, swapSize + 1);
			bufSize--;
			currBstart = swapBorder;
		}
		else{
			// std::advance(currL, blockSize);
			currL = currR;
			std::advance(currR, blockSize);
		}
	}
}

template<class T, class tIter, class Cmp>
void gefKatPas (tIter l, tIter m, tIter r, Cmp &cmp){
	size_t M = std::distance(l, m);
	size_t N = std::distance(m, r);
	size_t S = std::ceil(std::pow((M * M / std::log2(M)), 1.0 / 3));
	// size_t S = std::ceil(std::sqrt(M) + 1);
	size_t Sx = M % S;
	size_t Sy = S % S;
	tIter f = m - 2 * S;
	tIter ec = Sx ? m - Sx : m - S;
	tIter EcB = m - S;
	tIter EcE = m;
	tIter xc = l;
	tIter XcB = l;
	tIter XcE = XcB + Sx;
	tIter yc = m;
	tIter YcB = m;
	tIter YcE = YcB + S;
	tIter oc = l;
	tIter OcB = XcB;
	tIter OcE = XcE;
	T holeElem;
	tIter xk;
	bool specialXUpdMode = 0;
	int mode = 0;

	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	size_t blkSize = std::pow(2, std::max(std::floor(std::log2((double)N / M)), 0.0));
	tIter prefBord = l;

	while (std::distance(oc, yc) > 2 * S){
		if (mode == 0){

			// prefBord = l;
			specialXUpdMode = 0;
			if (yc != r){
				xk = std::upper_bound(xc, XcE, *yc, defaultCMP);
			}
			else{
				xk = XcE;
			}
			if (xk == XcE){
				prefBord = l;
				mode = 1;
				oc = xk;
				OcB = XcE;
				OcE = OcB + S;
				ec = EcB;
				xc = findNextXBlk(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
				XcB = xc;
				XcE = XcB + S;

				if (OcB == EcB){
					specialXUpdMode = 1;
				}
				if (OcB == f){
					std::swap(ec, f);
					EcB = ec;
					EcE = EcB + S;
				}
				if (OcB == XcB){
					mode = 0;
					continue;
				}
			}
			else{
				mode = 1;
				oc = xk;
				ec = EcE - std::distance(oc, OcE);
				holeElem = *oc;
				*oc = *yc;
				*yc = *ec;
				*ec = holeElem;

				xc = ec;
				XcB = EcB;
				XcE = EcE;
				yc++;
				if (yc >= prefBord){
					prefBord = l;
				}
				if (yc == YcE){
					f = YcB;
					YcB = YcE;
					YcE = YcB + S < r ? YcB + S : r;
				}
				oc++;
				if (oc == OcE){
					OcB = OcE;
					OcE = OcB + S;
				}
				ec++;
				if (ec == EcE){
					ec = f;
					EcB = ec;
					EcE = EcB + S;
					f = r;
				}
				if (OcB == EcB){
					specialXUpdMode = 1;
				}
			}
		}
		else if (mode == 1){

			if (prefBord == l){
				prefBord = findPrefixBorder<T>(yc, r, xc, blkSize, cmp);
			}

			if (yc < prefBord){
				holeElem = *oc;
				*oc = *yc;
				if (!specialXUpdMode){
					*yc = *ec;
					*ec = holeElem;
				}
				else{
					*yc = holeElem;
				}
				yc++;
				oc++;
				ec++;
			}
			else{
				prefBord = l;
				holeElem = *oc;
				*oc = *xc;
				if (!specialXUpdMode){
					*xc = *ec;
					*ec = holeElem;
				}
				else{
					*xc = holeElem;
				}
				xc++;
				oc++;
				ec++;
			}
			
			if (oc == OcE){
				OcB = OcE;
				OcE = OcB + S;
			}
			if (ec == EcE){
				ec = f;
				EcB = ec;
				EcE = EcB + S;
				f = r;
				specialXUpdMode = 0;
			}
			if (yc == YcE){
				f = YcB;
				YcB = yc;
				YcE = YcB + S;
			}
			if (xc == XcE){
				if (specialXUpdMode){
					xc = findNextXBlkM(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
				}
				else{
					xc = findNextXBlk(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
				}
				f = XcB;
				XcB = xc;
				XcE = XcB + S;
			}
			if (XcB == EcB){

			}
			if (OcB == EcB){
				specialXUpdMode = 1;
			}
			if (OcB == f){
				std::swap(f, ec);
				EcB = ec;
				EcE = EcB + S;
				specialXUpdMode = 1;
			}
			if (OcB == XcB){
				if (oc == xc){
					mode = 0;
					continue;
				}
				f = EcB;
				mode = 2;
			}
		}
		else if (mode == 2){

			if (prefBord == l){
				prefBord = findPrefixBorder<T>(yc, r, xc, blkSize, cmp);
			}

			if (yc < prefBord){
				holeElem = *oc;
				*oc = *yc;
				*yc = holeElem;
				yc++;
				oc++;
				ec++;
			}
			else{
				prefBord = l;
				holeElem = *oc;
				*oc = *xc;
				*xc = holeElem;
				xc++;
				oc++;
				ec++;
			}
			if (yc == YcE){
				ec = YcB;
				YcB = yc;
				YcE = YcB + S;
				EcB = ec;
				EcE = EcB + S;
				ec = EcB + std::distance(OcB, oc);
				mode = 0;
				continue;
			}
			if (xc == XcE){
				xc = findNextXBlkM(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
				XcB = xc;
				XcE = XcB + S;
				specialXUpdMode = 1;
				mode = 1;
				continue;
			}
		}
	}


	std::sort(oc, yc, defaultCMP);
	// bufferDistribution<T>(oc, yc, r, cmp);
	std::inplace_merge(oc, yc, r, defaultCMP);
}

#endif