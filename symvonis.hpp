#ifndef _SYMVONIS_HPP_
#define _SYMVONIS_HPP_
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iostream>

// binary-like search
template<class T, class tIter, class Cmp>
tIter BLS (tIter l, tIter r, size_t m, T const &x, Cmp &cmp){
	size_t seqLen = std::distance(l, r);
	size_t blockSize = std::ceil((double)seqLen / m);

	tIter currL = l;
	tIter currR = l + blockSize;
	while (currR <= r){
		if (cmp(x, *(currR - 1)) <= 0){
			return std::lower_bound(currL, currR, x,
			    [&](T const &a, T const &b){return cmp(a, b) < 0;});
		}
		std::advance(currL, blockSize);
		std::advance(currR, blockSize);
	}
	if (currR > r){
		return std::lower_bound(currL, r, x,
		    [&](T const &a, T const &b){return cmp(a, b) < 0;});
	}
}


// buf size must be not less than distance(l, m)
template<class T, class tIter, class Cmp>
void simpleMerge (tIter l, tIter m, tIter r, tIter buf, Cmp &cmp){
	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	size_t N = std::distance(m, r);
	size_t M = std::distance(l, m);
	size_t blockSize = std::ceil((double)N / M);
	std::swap_ranges(l, m, buf);

	// for (size_t k = 0; k < M; k++){
	// 	std::cout << *(l + k) << " ";
	// }
	// for (size_t k = 0; k < N; k++){
	// 	std::cout << *(m + k) << " ";
	// }
	// std::cout << std::endl;
	// for (size_t k = 0; k < M; k++){
	// 	std::cout << *(buf + k) << " ";
	// }
	// std::cout << std::endl << std::endl;

	tIter currL = m;
	tIter currR = m + blockSize;
	tIter currBstart = m;
	tIter currT = l;

	// std::cout << blockSize << std::endl;

	for (size_t i = 0; i < M;){

		// for (auto it = l; it != r; it++){
		// 	std::cout << *it << " ";
		// }
		// std::cout << std::endl;

		if (currR > r){
			currR = r;
		}
		if (currL < currBstart){
			currL = currBstart;
		}
		if (currL >= currR){
			// std::cout << "dang" << std::endl;
			size_t swapSize = std::distance(currBstart, r);
			std::reverse(currT, currBstart);
			std::reverse(currBstart, r);
			std::reverse(currT, r);
			std::advance(currT, swapSize);
			std::swap_ranges(currT, r, buf + i);
			break;
		}
		// std::cout << *currL << " " << *currR << " " << *(buf + i) << std::endl;
		if (cmp(*(buf + i), *(currR - 1)) <= 0){
			tIter swapBorder = std::lower_bound(currL, currR, *(buf + i), 
			    defaultCMP);
			size_t swapSize = std::distance(currBstart, swapBorder);
			if (std::distance(currT, currBstart) >= swapSize){
				std::swap_ranges(currBstart, swapBorder, currT);
			}
			else{
				std::reverse(currT, currBstart);
				std::reverse(currBstart, swapBorder);
				std::reverse(currT, swapBorder);
			}
			std::advance(currT, swapSize);
			std::advance(currBstart, swapSize);
			std::iter_swap(currT, buf + i);
			std::advance(currT, 1);
			i++;
			// std::cout << "swap" << std::endl;
		}
		else{
			currL = currR;
			std::advance(currR, blockSize);
			// std::cout << "move" << std::endl;
		}
	}
}

// m will be changed if elements from B were added to buffer
// returned iterator points to new l position (buffer is between l and "new l")
template<class T, class tIter, class Cmp>
tIter bufferExtraction (tIter l, tIter &m, tIter r, size_t bds, Cmp &cmp){
	size_t N = std::distance(m, r);
	size_t M = std::distance(l, m);
	// size_t bufDstSize = std::ceil(std::sqrt(M));
	size_t bufDstSize = bds;

	tIter bufStart = l;
	size_t bufSize = 1;

	for (tIter i = l + 1; i < m; i++){
		if (cmp(*i, *(bufStart + bufSize - 1)) > 0){
			size_t swapSize = std::distance(bufStart, i) - bufSize;
			std::reverse(bufStart, bufStart + bufSize);
			std::reverse(bufStart + bufSize, i);
			std::reverse(bufStart, i);
			std::advance(bufStart, swapSize);
			bufSize++;
		}
		if (bufSize == bufDstSize){
			std::reverse(l, bufStart);
			std::reverse(bufStart, bufStart + bufSize);
			std::reverse(l, bufStart + bufSize);
			return l + bufSize;
		}
	}
	size_t notChangingMbufSize = bufSize;
	// std::cout << "stage 1: OK" << std::endl;
	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	size_t blockSize = std::ceil((double)N / M);
	// std::cout << "blck " << blockSize << std::endl;
	tIter candidate = m;
	tIter currL = m;
	tIter currR = m + blockSize;
	tIter placeInBuf = std::lower_bound(bufStart, bufStart + bufSize,
	    *candidate, defaultCMP);
	if (placeInBuf == bufStart + bufSize){
		size_t swapSize = std::distance(bufStart, candidate) - bufSize;
		std::reverse(bufStart, bufStart + bufSize);
		std::reverse(bufStart + bufSize, candidate);
		std::reverse(bufStart, candidate);
		std::advance(bufStart, swapSize);
		bufSize++;
		// std::cout << "1" << std::endl;
	}
	else if (cmp(*placeInBuf, *candidate) != 0){
		size_t swapSize = std::distance(bufStart, candidate) - bufSize;
		
		// for (auto i = l; i != r; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;

		std::reverse(placeInBuf, bufStart + bufSize);
		std::reverse(bufStart + bufSize, candidate + 1);
		std::reverse(placeInBuf, candidate + 1);
		std::reverse(bufStart, placeInBuf);
		std::reverse(placeInBuf, placeInBuf + swapSize);
		std::reverse(bufStart, placeInBuf + swapSize);
		std::advance(bufStart, swapSize);
		bufSize++;
		// std::cout << "2" << std::endl;
	}
	// // std::cout << "stage 2: OK" << std::endl;
	while (bufSize < bufDstSize && candidate != r){
		// std::cout << "entered" << std::endl;
		if (currR > r){
			currR = r;
		}
		tIter newCandidate = std::upper_bound(currL, currR, *candidate,
		    defaultCMP);
		if (newCandidate == currR){
			//stops if not enough distinct elements
			if (currR == r){
				break;
			}
			// std::cout << "moved" << std::endl;
			// std::advance(currL, blockSize);
			currL = currR;
			std::advance(currR, blockSize);
		}
		else{
			candidate = newCandidate;
			placeInBuf = std::lower_bound(bufStart, bufStart + bufSize,
			    *candidate, defaultCMP);
			if (placeInBuf == bufStart + bufSize){
				size_t swapSize = std::distance(bufStart, candidate) - bufSize;
				std::reverse(bufStart, bufStart + bufSize);
				std::reverse(bufStart + bufSize, candidate);
				std::reverse(bufStart, candidate);
				std::advance(bufStart, swapSize);
				bufSize++;
				currL = candidate;
				currR = currL + blockSize;
			}
			else if (cmp(*placeInBuf, *candidate) != 0){
				size_t swapSize = std::distance(bufStart, candidate) - bufSize;
				std::reverse(placeInBuf, bufStart + bufSize);
				std::reverse(bufStart + bufSize, candidate + 1);
				std::reverse(placeInBuf, candidate + 1);
				std::reverse(bufStart, placeInBuf);
				std::reverse(placeInBuf, placeInBuf + swapSize);
				std::reverse(bufStart, placeInBuf + swapSize);
				std::advance(bufStart, swapSize);
				bufSize++;
				currL = candidate;
				currR = currL + blockSize;
			}
		}
	}
	// // std::cout << "stage 3: OK" << std::endl;
	std::reverse(l, bufStart);
	std::reverse(bufStart, bufStart + bufSize);
	std::reverse(l, bufStart + bufSize);
	std::advance(m, bufSize - notChangingMbufSize);
	return l + bufSize;
	// std::cout << "stage 4: OK" << std::endl;
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
	while (bufSize > 0){
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
void stableManUkon (tIter l, tIter m, tIter r, tIter mrgBuf, tIter simBuf,
    size_t ABlockSize, Cmp &cmp){
	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	size_t M = std::distance(l, m);
	size_t blockQuant = M / ABlockSize;
	size_t mergedBlocks = 0;
	tIter midSecStart = l;
	tIter lastSecStart = m;
	size_t offset = 0;

	while (mergedBlocks < blockQuant){

		// std::cout << "enter" << std::endl;
		// for (auto i = l; i != r; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;

		size_t nextBlockIndex = std::distance(simBuf + mergedBlocks,
			std::min_element(simBuf + mergedBlocks, simBuf + blockQuant));
		
		// std::cout << "merged " << mergedBlocks << std::endl;
		// for (size_t k = 0; k < ABlockSize; k++){
		// 	std::cout << *(simBuf + k) << " ";
		// }
		// std::cout << std::endl;
		// std::cout << "index " << nextBlockIndex << std::endl;

		if (offset == 0){
			std::swap_ranges(midSecStart, midSecStart + ABlockSize,
				midSecStart + nextBlockIndex * ABlockSize);
			std::iter_swap(simBuf + mergedBlocks, simBuf + mergedBlocks +
				nextBlockIndex);
		}
		else{
			size_t frontOffset = ABlockSize - offset;
			if (mergedBlocks + 1 == blockQuant){
				std::reverse(midSecStart, lastSecStart - offset);
				std::reverse(lastSecStart - offset, lastSecStart);
				std::reverse(midSecStart, lastSecStart);
			}
			else if (nextBlockIndex == 0){
				std::reverse(midSecStart, midSecStart + frontOffset);
				std::reverse(midSecStart + frontOffset, midSecStart +
				    frontOffset + ABlockSize);
				std::reverse(midSecStart, midSecStart + frontOffset +
				    ABlockSize);
			}
			else if (nextBlockIndex == blockQuant - mergedBlocks - 1){
				std::swap_ranges(midSecStart + frontOffset,
				    midSecStart + ABlockSize, lastSecStart - offset);
				std::reverse(midSecStart, midSecStart + frontOffset);
				std::reverse(midSecStart + frontOffset, midSecStart +
				    ABlockSize);
				std::reverse(midSecStart, midSecStart + ABlockSize);
				std::iter_swap(simBuf + mergedBlocks, simBuf + mergedBlocks +
				    nextBlockIndex);
			}
			else{
				std::swap_ranges(midSecStart, midSecStart + ABlockSize,
				    midSecStart + nextBlockIndex * ABlockSize + frontOffset);
				std::swap_ranges(lastSecStart - offset, lastSecStart,
				    midSecStart + nextBlockIndex * ABlockSize +
				    2 * frontOffset);
				tIter bs = midSecStart + nextBlockIndex * ABlockSize +
				    frontOffset;
				tIter bm = midSecStart + nextBlockIndex * ABlockSize +
				    2 * frontOffset;
				tIter be = midSecStart + (nextBlockIndex + 1) * ABlockSize +
				    frontOffset;
				std::reverse(bs, bm);
				std::reverse(bm, be);
				std::reverse(bs, be);
				std::iter_swap(simBuf + mergedBlocks, simBuf + mergedBlocks +
					nextBlockIndex);
				std::iter_swap(simBuf + mergedBlocks + nextBlockIndex,
					simBuf + blockQuant - 1);
			}
		}
		mergedBlocks++;
		std::advance(midSecStart, ABlockSize);

		// std::cout << "A moved" << std::endl;
		// for (auto i = l; i != r; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;
		// for (size_t k = 0; k < ABlockSize; k++){
		// 	std::cout << *(simBuf + k) << " ";
		// }
		// std::cout << std::endl;

		tIter swapBorder = std::lower_bound(lastSecStart, r, *(midSecStart - 1),
			defaultCMP);
		size_t swapSize = std::distance(lastSecStart, swapBorder);
		// size_t rotatesNum = (swapSize - (ABlockSize - offset)) / ABlockSize;
		// std::rotate(simBuf + mergedBlocks, simBuf + mergedBlocks + rotatesNum,
		//     simBuf + blockQuant);

		// std::cout << "offset " << offset << std::endl;

		if (swapSize > std::distance(midSecStart, lastSecStart)){
			std::reverse(midSecStart, lastSecStart);
			std::reverse(lastSecStart, swapBorder);
			std::reverse(midSecStart, swapBorder);
		}
		else{
			std::swap_ranges(lastSecStart, swapBorder, midSecStart);

			size_t rotatesNum;
			size_t frontOffset = (ABlockSize - offset) % ABlockSize;
			if (!(blockQuant - mergedBlocks) || frontOffset >= swapSize){
				rotatesNum = 0;
			}
			else{
				rotatesNum = (size_t)(std::ceil((swapSize - frontOffset) /
				    (double)ABlockSize)) % (blockQuant - mergedBlocks);
			}
			

			offset = (offset + swapSize) % ABlockSize;
			std::rotate(simBuf + mergedBlocks, simBuf + mergedBlocks +
			    rotatesNum, simBuf + blockQuant);
		}
		std::advance(lastSecStart, swapSize);

		// std::cout << "offset " << offset << std::endl;

		// std::cout << "before merge" << std::endl;
		// for (auto i = l; i != r; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;
		// for (size_t k = 0; k < ABlockSize; k++){
		// 	std::cout << *(simBuf + k) << " ";
		// }
		// std::cout << std::endl;

		simpleMerge<T>(midSecStart - ABlockSize, midSecStart, midSecStart +
		    swapSize, mrgBuf, cmp);

		// std::cout << "after merge" << std::endl;
		// for (auto i = l; i != r; i++){
		// 	std::cout << *i << " ";
		// }
		// std::cout << std::endl;

		std::advance(midSecStart, swapSize);
		
		// std::cout << std::endl;
	}
}

template<class T, class tIter, class Cmp>
void symvonis (tIter l, tIter m, tIter r, Cmp &cmp){
	auto defaultCMP = [&](T const &a, T const &b){return cmp(a, b) < 0;};
	size_t M = std::distance(l, m);
	size_t BufDstSize = std::ceil(std::sqrt(M));
	tIter buf1 = l;
	tIter buf2 = bufferExtraction<T>(l, m, r, BufDstSize, cmp);
	if (std::distance(l, buf2) < BufDstSize){
		throw "Extraction of buf1 is not possible";
		return;
	}
	l = buf2;
	l = bufferExtraction<T>(l, m, r, BufDstSize, cmp);
	if (std::distance(buf2, l) < BufDstSize){
		throw "Extraction of buf2 is not possible";
		return;
	}
	size_t newM = std::distance(l, m);
	tIter Asmall = l;
	size_t rem = newM % BufDstSize;
	std::advance(l, rem);
	stableManUkon<T>(l, m, r, buf1, buf2, BufDstSize, cmp);
	bufferDistribution<T>(Asmall, l, r, cmp);
	std::sort(buf2, Asmall, defaultCMP);
	std::sort(buf1, buf2, defaultCMP);
	bufferDistribution<T>(buf2, Asmall, r, cmp);
	bufferDistribution<T>(buf1, buf2, r, cmp);
}

#endif