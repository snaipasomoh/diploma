#ifndef _GEFKATPAS_HPP_
#define _GEFKATPAS_HPP_
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iostream>

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
	}
	return res;
}

template<class T, class tIter, class Cmp>
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
	}
	return res;
}

template<class tIter>
void updateYblk (tIter &YcB, tIter &YcE, tIter yc, tIter &f, size_t S){
	if (yc == YcE){
		f = YcB;
		YcB = YcE;
		YcE = YcB + S;
	}
}

template<class tIter>
void updateOblk (tIter &OcB, tIter &OcE, tIter oc, size_t S){
	if (oc == OcE){
		OcB = OcE;
		OcE = OcB + S;
	}
}

template<class T, class tIter, class Cmp>
void gefKatPas (tIter l, tIter m, tIter r, Cmp &cmp){
	size_t M = std::distance(l, m);
	size_t N = std::distance(m, r);
	size_t S = std::ceil(std::pow((M * M / std::log2(M)), 1.0 / 3));
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
	T holeElem = *ec;
	tIter xk;

	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	size_t blkSize = std::pow(2, std::floor(std::log2((double)N / M)));
	tIter prefBord = l;

	state_1:
	std::cout << "state_1" << std::endl;

	if (yc != r){
		xk = std::upper_bound(xc, XcE, *yc, defaultCMP);
	}
	else{
		xk = XcE;
	}
	
	
	if (xk == XcE){
		oc = xk;
		updateOblk(OcB, OcE, oc, S);

		if (std::distance(oc, yc) == 2 * S){
			goto state_11;
		}

		XcB = findNextXBlk(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
		XcE = XcB + S;
		xc = XcB;
		ec = EcB;
		if (OcB == XcB){
			goto state_1;
		}
		else if (oc == ec){
			goto state_7;
		}
		else{
			goto state_3;
		}
	}
	else{
		oc = xk;
		ec = l + (std::distance(l, ec) / S) * S + std::distance(l, xk) % S;
		holeElem = *oc;
		*oc = *yc;
		*yc = *ec;
		*ec = holeElem;
		xc = ec;
		XcB = EcB;
		XcE = EcE;
		oc++;
		yc++;
		ec++;
		updateYblk(YcB, YcE, yc, f, S);
		goto state_2;
	}


	state_2:
	std::cout << "state_2" << std::endl;
	if (ec == EcE){
		goto state_4;
	}

	if (prefBord == l){
		prefBord = findPrefixBorder<T>(yc, r, xc, blkSize, cmp);
	}

	if (yc != prefBord){
		holeElem = *oc;
		*oc = *yc;
		*yc = *ec;
		*ec = holeElem;
		oc++;
		ec++;
		yc++;
		updateYblk(YcB, YcE, yc, f, S);
	}
	else{
		prefBord = l;
		holeElem = *oc;
		*oc = *xc;
		*xc = *ec;
		*ec = holeElem;
		oc++;
		ec++;
		xc++;
		if (std::distance(oc, yc) == 2 * S){
			goto state_11;
		}
	}
	goto state_2;


	state_3:
	std::cout << "state_3" << std::endl;
	if (ec == EcE){	// section 4
		goto state_4;
	}
	if (xc == XcE){	// section 6
		f = XcB;
		xc = findNextXBlk(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
		XcB = xc;
		XcE = XcB + S;
		if (xc == EcB){
			goto state_2;
		}
		if (xc == OcB){
			goto state_1;
		}
	}

	if (prefBord == l){
		prefBord = findPrefixBorder<T>(yc, r, xc, blkSize, cmp);
	}

	if (yc != prefBord){
		holeElem = *oc;
		*oc = *yc;
		*yc = *ec;
		*ec = holeElem;
		oc++;
		ec++;
		yc++;
		updateYblk(YcB, YcE, yc, f, S);
	}
	else{
		prefBord = l;
		holeElem = *oc;
		*oc = *xc;
		*xc = *ec;
		*ec = holeElem;
		oc++;
		ec++;
		xc++;
		if (std::distance(oc, yc) == 2 * S){
			goto state_11;
		}
	}
	goto state_3;


	state_4:
	std::cout << "state_4" << std::endl;
	if (f == l){
		std::cout << "no free blk, but it's needed" << std::endl;
	}
	ec = f;
	EcB = f;
	EcE = EcB + S;
	f = l;
	updateOblk(OcB, OcE, oc, S);
	if (OcB == XcB){
		goto state_9;
	}
	goto state_3;


	state_7:
	std::cout << "state_7" << std::endl;
	if (ec == EcE){
		goto state_4;
	}
	
	if (prefBord == l){
		prefBord = findPrefixBorder<T>(yc, r, xc, blkSize, cmp);
	}

	if (yc != prefBord){
		holeElem = *oc;
		*oc = *yc;
		*yc = holeElem;
		oc++;
		ec++;
		yc++;
		updateYblk(YcB, YcE, yc, f, S);
	}
	else{
		prefBord = l;
		holeElem = *oc;
		*oc = *xc;
		*xc = holeElem;
		oc++;
		ec++;
		xc++;
		if (std::distance(oc, yc) == 2 * S){
			goto state_11;
		}
		if (xc == XcE){	// section 6
			f = XcB;
			xc = findNextXBlk(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
			XcB = xc;
			XcE = XcB + S;
			if (xc == EcB){
				goto state_2;
			}
			if (xc == OcB){
				goto state_1;
			}
		}
	}
	goto state_7;


	state_7_spec:
	std::cout << "state_7_spec" << std::endl;
	if (ec == EcE){
		goto state_4;
	}
	
	if (prefBord == l){
		prefBord = findPrefixBorder<T>(yc, r, xc, blkSize, cmp);
	}

	if (yc != prefBord){
		holeElem = *oc;
		*oc = *yc;
		*yc = holeElem;
		oc++;
		ec++;
		yc++;
		updateYblk(YcB, YcE, yc, f, S);
	}
	else{
		prefBord = l;
		holeElem = *oc;
		*oc = *xc;
		*xc = holeElem;
		oc++;
		ec++;
		xc++;
		if (std::distance(oc, yc) == 2 * S){
			goto state_11;
		}
		if (xc == XcE){	// section 6
			f = XcB;
			xc = findNextXBlkM<T>(OcB, OcE, YcB, EcB, EcE, ec, f, S, cmp);
			XcB = xc;
			XcE = XcB + S;
			if (xc == EcB){
				goto state_2;
			}
			if (xc == OcB){
				goto state_1;
			}
		}
	}
	goto state_7_spec;


	state_8:
	std::cout << "state_8" << std::endl;
	std::swap(f, ec);
	EcB = ec;
	EcE = EcB + S;
	goto state_7;


	state_9:
	std::cout << "state_9" << std::endl;
	if (oc == xc){
		goto state_1;
	}
	f = ec;
	ec = oc;
	EcB = OcB;
	EcE = OcE;
	goto state_7_spec;


	state_11:
	std::cout << "state_11" << std::endl;
	for (tIter i = l; i != r; i++){
		std::cout << *i << " ";
	}
	std::cout << std::endl;


}

#endif