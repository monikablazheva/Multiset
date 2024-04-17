#pragma once
#include <fstream>
#include "Constants.h"

namespace MultisetHelpers {
	unsigned char getMask(unsigned int n);
}

class Multiset {
private:
	unsigned char* data = nullptr;
	unsigned int maxNumberN = 0;
	uint8_t maxBitsK = 0;
	unsigned int bytesNeeded = 0;

	void copyFrom(const Multiset& other);
	void free();
	
	void setBytesNeeded();

public:
	Multiset() = default;
	Multiset(unsigned int maxNumberN, uint8_t maxBitsK);
	Multiset(const Multiset& other);
	Multiset& operator=(const Multiset& other);
	~Multiset();

	void add(unsigned int num);
	int count(unsigned int num) const;
	void print() const;
	void printInMemory() const;
	void printCounts() const;

	friend std::istream& operator>>(std::istream& is, Multiset& multiset);
	friend std::ostream& operator<<(std::ostream& of, const Multiset& multiset);
	friend Multiset multisetsUnion(const Multiset& lhs, const Multiset& rhs);
	friend Multiset multisetsIntersection(const Multiset& lhs, const Multiset& rhs);
};

