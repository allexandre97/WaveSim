#include <cstdint> 
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


struct BmpHeader {
	char bitmapSignatureBytes[2] = {'B', 'M'};
	uint32_t sizeOfBitmapFile; // total size of bitmap file
	uint32_t reservedBytes = 0;
	uint32_t pixelDataOffset = 54;
};

struct BmpInfoHeader {
	uint32_t sizeOfThisHeader = 40;
	int32_t width; // in pixels
	int32_t height; // in pixels
	uint16_t numberOfColorPlanes = 1; // must be 1
	uint16_t colorDepth = 24;
	uint32_t compressionMethod = 0;
	uint32_t rawBitmapDataSize = 0; // generally ignored
	int32_t horizontalResolution = 3780; // in pixel per meter
	int32_t verticalResolution = 3780; // in pixel per meter
	uint32_t colorTableEntries = 0;
	uint32_t importantColors = 0;
};

struct Pixel {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

int SaveBMP(string name, int DIMENSION, vector<vector<float>> STATE, vector<int> RGB);

