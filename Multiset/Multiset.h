#pragma once
#include <fstream>
#include "Constants.h"

namespace MultisetHelpers {
	uint8_t getBitIndex(unsigned int num, uint8_t maxBitsK);

	unsigned int getByteIndex(int num, uint8_t maxBitsK);
	unsigned char getMask(unsigned int num, unsigned int k);

	void printInBinary(unsigned int number, uint8_t k);
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
	~Multiset() noexcept;

	void add(unsigned int num, unsigned int times = 1);
	unsigned int count(unsigned int num) const;
	void print() const;
	void printInMemory() const;
	void printCounts() const;

	void readFromBinaryFile(const char* fileName);
	void writeToBinaryFile(const char* fileName) const;

	friend Multiset multisetsUnion(const Multiset& lhs, const Multiset& rhs);
	friend Multiset multisetsIntersection(const Multiset& lhs, const Multiset& rhs);
	friend Multiset multisetsDifference(const Multiset& lhs, const Multiset& rhs);
	friend Multiset multisetsAddition(const Multiset& multiset);

};

