#include "ofstream12.h"
using namespace std;
// Private Methods

void ofstream12::init()
{
	for (size_t i = 0; i < fBufferSize; i++) 
	{
		fBuffer[i] = 0;
	}
	
	fByteIndex = 0;
	fBitIndex = 7;
} // initialize data members

void ofstream12::completeWriteBit() 
{
	// Uses fByteIndex and fBitIndex
	fBitIndex--;

	if (fBitIndex < 0)
	{
		fByteIndex++;
		fBitIndex = 7;

		if (fByteIndex == fBufferSize)
		{
			flush();
		}
	}
} // complete write

void ofstream12::writeBit0()
{
	// Uses fByteIndex and fBitIndex
	completeWriteBit();
} // write 0

void ofstream12::writeBit1() 
{
	// Uses fByteIndex and fBitIndex
	fBuffer[fByteIndex] += 1 << fBitIndex;
	completeWriteBit();
} // write 1

// Public Methods

ofstream12::ofstream12(const char* aFileName, size_t aBufferSize)
{
	fBufferSize = aBufferSize; // Default = 128
	fBuffer = new unsigned char[fBufferSize];
	
	init(); // Clean buffer
	open(aFileName); // If not provided, default = nullptr
} // Constructor

ofstream12::~ofstream12()
{
	close();
	delete [] fBuffer;
} // Destructor

void ofstream12::open(const char* aFileName)
{
	if (aFileName)
	{
		fOStream.open(aFileName, ofstream::binary);
	}
	
}// Open file passed in.

void ofstream12::close()
{
	flush();
	fOStream.close();
}// Close ofstream associated file.

bool ofstream12::good() const
{
	return fOStream.good();
}// Check the ofstream is working.

bool ofstream12::isOpen() const
{
	return fOStream.is_open();
}// Check the ofstream is open for output.

void ofstream12::flush()
{
	fOStream.write((char*)fBuffer, fByteIndex + (fBitIndex % 7 ? 1 : 0));
	init();
} // Empty the buffer.

ofstream12& ofstream12::operator<<(size_t aValue)
{
	// aValue to hex/binary.
	// Output binary with writeBit1() or writeBit0()
	// Will have to buffer 4 bits as it supports 8 bit output not 12.
	
	for (size_t i = 0; i < 12; i++)
	{
		// If the final binary digit in aValue is 1
		// The & bit operator will return 1 and writeBit1() is called
		// else the bit operator returns 0 and writeBit0() is called 
		// and the binary digits are shifted to the right.
		if (aValue & 0x01) // Checking lowest bit
		{
			writeBit1();
		}
		else
		{
			writeBit0();
		}
		aValue >>= 1; // Shift bits to the right by 1.
	}
	return *this;
} // Output bits.