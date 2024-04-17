#include "Multiset.h"
#include<iostream>

namespace MultisetHelpers {
	unsigned char getMask(unsigned int num, unsigned int k) {
		unsigned char mask = (1 << k) - 1;
		return mask;
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
	bytesNeeded = ((maxNumberN+1) * maxBitsK + 7) / BYTE_SIZE; //+1 for the zero; +7 to round to the bigger number
}

//THE BIG FOUR
Multiset::Multiset(unsigned int maxNumberN, uint8_t maxBitsK) : maxNumberN(maxNumberN), maxBitsK(maxBitsK) {
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

Multiset::~Multiset() {
	free();
}

//CLASS FUNCTIONS
void Multiset::add(unsigned int num) {
	//out of range exception
	//if (num > maxNumberN) {
	//	return;
	//}

	//int byteIndex = (num * maxBitsK) / 8;
	//int bitIndex = (num * maxBitsK) % 8;

	//unsigned char mask = MultisetHelpers::getMask(num,maxBitsK);
	//unsigned char currentCount = count(num);

	//if (currentCount < ((int)mask)) {
	//	currentCount++;
	//	data[byteIndex] &= ~(mask << bitIndex); // Clear the old count bits
	//	data[byteIndex] |= (currentCount << bitIndex); // Set the updated count bits
	//}

	//if(num > maxNumberN) {
	//	return; // Out of range, do nothing
	//}

	//int bitIndex = (num * maxBitsK) % 8; // Calculate bit index within a byte
	//int fullByteIndex = (num * maxBitsK) / 8; // Calculate starting byte index

	//unsigned char mask = MultisetHelpers::getMask(maxNumberN, maxBitsK); // Get the mask for maxBitsK

	//// Calculate the number of bits remaining in the current byte
	//int bitsRemainingInCurrentByte = 8 - bitIndex;

	//// Extract the current count from the data array
	//unsigned int currentCount = 0;

	//// If the count spans across two bytes
	//if (bitsRemainingInCurrentByte < maxBitsK) {
	//	// Count bits split across two bytes
	//	unsigned char firstByte = data[fullByteIndex];
	//	unsigned char secondByte = data[fullByteIndex + 1];

	//	// Combine bits from both bytes to get the current count
	//	currentCount = ((secondByte & ((1 << (maxBitsK - bitsRemainingInCurrentByte)) - 1)) << bitsRemainingInCurrentByte) | (firstByte >> bitIndex);
	//}
	//else {
	//	// Count bits contained entirely within one byte
	//	currentCount = (data[fullByteIndex] >> bitIndex) & mask;
	//}

	//if (currentCount < mask) {
	//	// Increment count
	//	currentCount++;

	//	// Update the count in the data array
	//	if (bitsRemainingInCurrentByte >= maxBitsK) {
	//		// Entire count fits within the current byte
	//		data[fullByteIndex] &= ~(mask << bitIndex); // Clear old count bits
	//		data[fullByteIndex] |= (currentCount << bitIndex); // Set updated count bits
	//	}
	//	else {
	//		// Count spans across two bytes
	//		// Calculate split for the count
	//		unsigned int lowerPart = currentCount & ((1 << bitsRemainingInCurrentByte) - 1); // Lower part of the count within the first byte
	//		unsigned int upperPart = currentCount >> bitsRemainingInCurrentByte; // Upper part of the count within the second byte

	//		// Update the first byte
	//		data[fullByteIndex] &= ~(mask << bitIndex); // Clear old count bits in the first byte
	//		data[fullByteIndex] |= (lowerPart << bitIndex); // Set updated lower part of count bits

	//		// Update the second byte
	//		data[fullByteIndex + 1] &= ~(1 << (maxBitsK - bitsRemainingInCurrentByte) - 1); // Clear old count bits in the second byte
	//		data[fullByteIndex + 1] |= upperPart; // Set updated upper part of count bits
	//	}
	//}

	if (num > maxNumberN) {
		return; // Out of range, do nothing
	}

	int bitIndex = (num * maxBitsK) % 8; // Calculate bit index within a byte
	int fullByteIndex = (num * maxBitsK) / 8; // Calculate starting byte index

	unsigned char mask = MultisetHelpers::getMask(maxNumberN, maxBitsK); // Get the mask for maxBitsK

	// Calculate the number of bits remaining in the current byte
	int bitsRemainingInCurrentByte = 8 - bitIndex;

	// Extract the current count from the data array
	unsigned int currentCount = 0;

	// If the count spans across two bytes
	if (bitsRemainingInCurrentByte < maxBitsK) {
		// Count bits split across two bytes
		unsigned char firstByte = data[fullByteIndex];
		unsigned char secondByte = data[fullByteIndex + 1];

		// Combine bits from both bytes to get the current count
		unsigned int lowerPart = (firstByte >> bitIndex) & ((1 << bitsRemainingInCurrentByte) - 1); // Lower part of the count within the first byte
		unsigned int upperPart = secondByte & ((1 << (maxBitsK - bitsRemainingInCurrentByte)) - 1); // Upper part of the count within the second byte

		// Combine the lower and upper parts to get the complete count value
		currentCount = (upperPart << bitsRemainingInCurrentByte) | lowerPart;
	}
	else {
		// Count bits contained entirely within one byte
		currentCount = (data[fullByteIndex] >> bitIndex) & mask;
	}

	if (currentCount < mask) {
		// Increment count
		currentCount++;

		// Update the count in the data array
		if (bitsRemainingInCurrentByte >= maxBitsK) {
			// Entire count fits within the current byte
			data[fullByteIndex] &= ~(mask << bitIndex); // Clear old count bits
			data[fullByteIndex] |= (currentCount << bitIndex); // Set updated count bits
		}
		else {
			// Count spans across two bytes
			// Calculate split for the count
			unsigned int lowerPart = currentCount & ((1 << bitsRemainingInCurrentByte) - 1); // Lower part of the count within the first byte
			unsigned int upperPart = currentCount >> bitsRemainingInCurrentByte; // Upper part of the count within the second byte

			// Update the first byte
			data[fullByteIndex] &= ~(mask << bitIndex); // Clear old count bits in the first byte
			data[fullByteIndex] |= (lowerPart << bitIndex); // Set updated lower part of count bits

			// Update the second byte
			data[fullByteIndex + 1] &= ~((1 << (maxBitsK - bitsRemainingInCurrentByte)) - 1); // Clear old count bits in the second byte
			data[fullByteIndex + 1] |= upperPart; // Set updated upper part of count bits
		}
	}
}

int Multiset::count(unsigned int num) const {
	/*int byteIndex = (num * maxBitsK) / 8;
	int bitIndex = (num * maxBitsK) % 8;

	unsigned char mask = MultisetHelpers::getMask(num, maxBitsK);

	int c = (data[byteIndex] >> bitIndex) & mask;
	return c;*/

	int bitIndex = (num * maxBitsK) % 8; // Calculate bit index within a byte
	int fullByteIndex = (num * maxBitsK) / 8; // Calculate starting byte index

	unsigned char mask = MultisetHelpers::getMask(maxNumberN, maxBitsK); // Get the mask for maxBitsK

	// Calculate the number of bits remaining in the current byte
	int bitsRemainingInCurrentByte = 8 - bitIndex;

	// Initialize variables to store count bits
	unsigned int count = 0;

	// If the count bits span across two bytes
	if (bitsRemainingInCurrentByte < maxBitsK) {
		// Count bits split across two bytes
		unsigned char firstByte = data[fullByteIndex];
		unsigned char secondByte = data[fullByteIndex + 1];

		// Combine bits from both bytes to get the complete count value
		unsigned int lowerPart = (firstByte >> bitIndex) & ((1 << bitsRemainingInCurrentByte) - 1); // Lower part of the count within the first byte
		unsigned int upperPart = secondByte & ((1 << (maxBitsK - bitsRemainingInCurrentByte)) - 1); // Upper part of the count within the second byte

		// Combine the lower and upper parts to get the complete count value
		count = (upperPart << bitsRemainingInCurrentByte) | lowerPart;
	}
	else {
		// Count bits contained entirely within one byte
		count = (data[fullByteIndex] >> bitIndex) & mask;
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
	for (int i = 0; i <= maxNumberN; ++i) {

		if (count(i)>0) {
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;
}

void Multiset::printInMemory() const {
	for (int element = 0; element <= maxNumberN; ++element) {
		int bitOffset = ((element - 1) * maxBitsK) % 8;
		int byteIndex = ((element - 1) * maxBitsK) / 8;
		unsigned char byteValue = data[byteIndex];

		std::cout << "Element " << element << ": ";
		// Print the binary representation of the packed bits (k bits per element)
		for (int i = 0; i < maxBitsK; ++i) {
			bool bit = byteValue & (1 << (bitOffset + i));
			std::cout << (bit ? '1' : '0');
		}
		std::cout << std::endl;
	}

}