#include "Multiset.h"
#include<iostream>

namespace MultisetHelpers {
	uint8_t getBitIndex(unsigned int num, uint8_t maxBitsK) {
		return (num * maxBitsK) % BYTE_SIZE;
	}

	unsigned int getByteIndex(int num, uint8_t maxBitsK) {
		return (num * maxBitsK) / BYTE_SIZE;
	}

	unsigned char getMask(unsigned int num, unsigned int k) {
		unsigned char mask = (1 << k) - 1;
		return mask;
	}

	void printInBinary(unsigned int number, uint8_t k) {

		int numBits = sizeof(int) * 8;
		unsigned int mask = 1 << (numBits - 1);

		// Flag to track if significant bits have been encountered
		bool foundSignificant = false;

		for (int i = 0; i < numBits; ++i) {

			int bit = (number & mask) ? 1 : 0;

			// Set foundSignificant to true once a significant bit (1) is encountered
			if (bit == 1)
				foundSignificant = true;

			// Print the bit if it's significant or if we need leading zeros to reach width k
			if (foundSignificant || i >= (numBits - k))
				std::cout << bit;

			mask >>= 1;
		}
	}

	unsigned int powerOfTwo(unsigned power) {
		// 2^31 is the maximum power of two bitwise 
		if (power > 31) {
			throw std::invalid_argument("Power of two function argument too large!");
		}

		return (1 << power);
	}
}

//PRIVATE FUNCTIONS
void Multiset::copyFrom(const Multiset& other) {
	maxNumberN = other.maxNumberN;
	maxBitsK = other.maxBitsK;
	bytesNeeded = other.bytesNeeded;
	data = new unsigned char[bytesNeeded];
	for (size_t i = 0; i < bytesNeeded; i++) {
		data[i] = other.data[i];
	}
}

void Multiset::free() {
	delete[] data;
	data = nullptr;
	maxNumberN = maxBitsK = bytesNeeded = 0;
}

void Multiset::setBytesNeeded() {
	bytesNeeded = (((maxNumberN + 1) * maxBitsK + 7) / BYTE_SIZE) + 1; // +7 to round to the bigger number
}

//THE BIG FOUR
Multiset::Multiset(unsigned int maxNumberN, uint8_t maxBitsK) : maxNumberN(maxNumberN) {
	if (maxBitsK < 1 || maxBitsK > 8) {
		throw std::invalid_argument("Invalid bits K value!");
	}

	this->maxBitsK = maxBitsK;
	setBytesNeeded();
	data = new unsigned char[bytesNeeded] {};
}

Multiset::Multiset(const Multiset& other) {
	copyFrom(other);
}

Multiset& Multiset::operator=(const Multiset& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

Multiset::~Multiset() noexcept {
	free();
}

//CLASS FUNCTIONS
void Multiset::add(unsigned int num, unsigned int times) {
	if (num > maxNumberN) {
		throw std::invalid_argument("Add function number argument too large!");
	}

	unsigned int currCount = count(num);
	unsigned int maxTimes = MultisetHelpers::powerOfTwo(maxBitsK) - 1;

	if (currCount >= maxTimes) {
		throw std::length_error("Number is already added max times!");
	}

	if (times > maxTimes) {
		throw std::invalid_argument("Add function times argument too large!");
	}

	uint8_t bitIndex = MultisetHelpers::getBitIndex(num, maxBitsK);
	unsigned int byteIndex = MultisetHelpers::getByteIndex(num, maxBitsK);
	unsigned char mask = MultisetHelpers::getMask(maxNumberN, maxBitsK);
	int bitsRemainingCurrByte = 8 - bitIndex;

	if (currCount < mask) {

		currCount += times;
		if (currCount > maxTimes) {
			throw std::length_error("Number cannot be added this much times!");
		}

		// Entire count fits within the current byte
		if (bitsRemainingCurrByte >= maxBitsK) {
			data[byteIndex] &= ~(mask << bitIndex); // Clear old bits
			data[byteIndex] |= (currCount << bitIndex); // Update with new bits
		}
		else {
			// Count spans across two bytes
			unsigned int firstByteBits = currCount & ((1 << bitsRemainingCurrByte) - 1);
			unsigned int secondByteBits = currCount >> bitsRemainingCurrByte;

			// Update the first byte
			data[byteIndex] &= ~(mask << bitIndex); // Clear old bits in first byte
			data[byteIndex] |= (firstByteBits << bitIndex); // Update new bits in first byte

			// Update the second byte
			data[byteIndex + 1] &= ~((1 << (maxBitsK - bitsRemainingCurrByte)) - 1); // Clear old bits in second byte
			data[byteIndex + 1] |= secondByteBits; // Update new bits in second byte
		}
	}
}

unsigned int Multiset::count(unsigned int num) const {
	if (num > maxNumberN) {
		throw std::invalid_argument("Count function argument too large!");
	}

	uint16_t bitIndex = MultisetHelpers::getBitIndex(num, maxBitsK);
	unsigned int byteIndex = MultisetHelpers::getByteIndex(num, maxBitsK);
	unsigned char mask = MultisetHelpers::getMask(maxNumberN, maxBitsK);
	int bitsRemainingCurrByte = 8 - bitIndex;

	unsigned int count = 0;

	// If the number is between two bytes
	if (bitsRemainingCurrByte < maxBitsK) {

		unsigned char firstByte = data[byteIndex];
		unsigned char secondByte = data[byteIndex + 1];

		// Combine bits from both bytes to get the current number
		unsigned int firstByteBits = (firstByte >> bitIndex) & ((1 << bitsRemainingCurrByte) - 1);
		unsigned int secondByteBits = secondByte & ((1 << (maxBitsK - bitsRemainingCurrByte)) - 1);
		count = (secondByteBits << bitsRemainingCurrByte) | firstByteBits;
	}
	else {
		// Count bits contained entirely within one byte
		count = (data[byteIndex] >> bitIndex) & mask;
	}

	return count;
}

void Multiset::printCounts() const {
	for (size_t i = 0; i <= maxNumberN; ++i) {
		std::cout << count(i) << " ";
	}
	std::cout << std::endl;
}

void Multiset::print() const {
	for (unsigned int i = 0; i <= maxNumberN; ++i) {

		if (count(i) > 0) {
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;
}

void Multiset::printInMemory() const {
	for (unsigned int i = 0; i <= maxNumberN; ++i) {
		std::cout << "Number " << i << ": ";
		MultisetHelpers::printInBinary(count(i), maxBitsK);
		std::cout << std::endl;
	}
}

void Multiset::readFromBinaryFile(const char* fileName) {
	if (fileName == nullptr) {
		throw std::invalid_argument("Read from binary file function argument is empty!");
	}

	std::ifstream iFile(fileName, std::ios::binary);

	if (!iFile.is_open()) {
		throw std::runtime_error("File to read could not open!");
	}

	iFile.read((char*)&maxNumberN, sizeof(maxNumberN));
	iFile.read((char*)&maxBitsK, sizeof(maxBitsK));
	iFile.read((char*)&bytesNeeded, sizeof(bytesNeeded));

	delete[] data;
	data = new unsigned char[bytesNeeded] {};
	iFile.read((char*)data, bytesNeeded);

	iFile.close();
}

void Multiset::writeToBinaryFile(const char* fileName) const {
	if (fileName == nullptr) {
		throw std::invalid_argument("Write to binary file function argument is empty!");
	}

	std::ofstream oFile(fileName, std::ios::binary);

	if (!oFile.is_open()) {
		throw std::runtime_error("File to write could not open!");
	}

	oFile.write((const char*)&maxNumberN, sizeof(maxNumberN));
	oFile.write((const char*)&maxBitsK, sizeof(maxBitsK));
	oFile.write((const char*)&bytesNeeded, sizeof(bytesNeeded));
	oFile.write((const char*)data, bytesNeeded);

	oFile.close();
}

//FRIEND FUNCTIONS
Multiset multisetsUnion(const Multiset& lhs, const Multiset& rhs) {
	unsigned int newMaxNumberN = std::max(lhs.maxNumberN, rhs.maxNumberN);
	unsigned int smallerMaxN = std::min(lhs.maxNumberN, rhs.maxNumberN);
	uint8_t newMaxBitsK = std::max(lhs.maxBitsK, rhs.maxBitsK);

	Multiset result = { newMaxNumberN, newMaxBitsK };

	for (unsigned int i = 0; i <= smallerMaxN; i++)
	{
		result.add(i, std::max(lhs.count(i), rhs.count(i)));
	}
	if (lhs.maxNumberN > rhs.maxNumberN) {
		for (unsigned int i = smallerMaxN + 1; i <= lhs.maxNumberN; i++)
		{
			result.add(i, lhs.count(i));
		}
	}
	else if (lhs.maxNumberN < rhs.maxNumberN) {
		for (unsigned int i = smallerMaxN + 1; i <= rhs.maxNumberN; i++)
		{
			result.add(i, rhs.count(i));
		}
	}

	return result;
}

Multiset multisetsIntersection(const Multiset& lhs, const Multiset& rhs) {
	if (&lhs == &rhs) {
		return lhs;
	}

	unsigned int newMinNumberN = std::min(lhs.maxNumberN, rhs.maxNumberN);
	uint8_t newMinBitsK = std::min(lhs.maxBitsK, rhs.maxBitsK);

	Multiset result = { newMinNumberN, newMinBitsK };

	for (unsigned int i = 0; i <= newMinNumberN; i++)
	{
		result.add(i, std::min(lhs.count(i), rhs.count(i)));
	}

	return result;
}

//lhs-rhs
Multiset multisetsDifference(const Multiset& lhs, const Multiset& rhs) {
	Multiset result = { lhs.maxNumberN, lhs.maxBitsK };

	if (&lhs == &rhs) {
		return result;
	}
	
	unsigned int smallerMaxN = std::min(lhs.maxNumberN, rhs.maxNumberN);

	for (unsigned int i = 0; i <= smallerMaxN; i++)
	{
		int diff = lhs.count(i) - rhs.count(i);
		if (diff <= 0) diff = 0;
		result.add(i, diff);
	}
	if (lhs.maxNumberN > rhs.maxNumberN) {
		for (unsigned int i = smallerMaxN + 1; i <= lhs.maxNumberN; i++)
		{
			result.add(i, lhs.count(i));
		}
	}

	return result;
}

Multiset multisetsAddition(const Multiset& multiset) {
	Multiset result = {multiset.maxNumberN, multiset.maxBitsK};

	for (unsigned int i = 0; i <= result.maxNumberN; i++)
	{
		result.add(i, (MultisetHelpers::powerOfTwo(result.maxBitsK) - 1 - multiset.count(i)));
	}

	return result;
}
