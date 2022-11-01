#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

#include "saver.hpp"

struct BmpHeader {
    /*
    Struct with the basic info needed to build a bitmap header.
    Also has a method to write it to a file
    */
	char bitmapSignatureBytes[2] = {'B', 'M'};
	uint32_t sizeOfBitmapFile; // total size of bitmap file
	uint32_t reservedBytes = 0;
	uint32_t pixelDataOffset = 54;

    void save_on_file(std::ofstream& fout) {
        fout.write(this->bitmapSignatureBytes, 2);
        fout.write((char*)&this->sizeOfBitmapFile, sizeof(uint32_t));
        fout.write((char*)&this->reservedBytes, sizeof(uint32_t));
        fout.write((char*)&this->pixelDataOffset, sizeof(uint32_t));
    }

};

struct BmpInfoHeader {
    /*
    Struct with the metadata of the bitmap file.
    Has a method to write it to file
    */

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

    void save_on_file(std::ofstream& fout) {
        fout.write((char*)&this->sizeOfThisHeader, sizeof(uint32_t));
        fout.write((char*)&this->width, sizeof(int32_t));
        fout.write((char*)&this->height, sizeof(int32_t));
        fout.write((char*)&this->numberOfColorPlanes, sizeof(uint16_t));
        fout.write((char*)&this->colorDepth, sizeof(uint16_t));
        fout.write((char*)&this->compressionMethod, sizeof(uint32_t));
        fout.write((char*)&this->rawBitmapDataSize, sizeof(uint32_t));
        fout.write((char*)&this->horizontalResolution, sizeof(int32_t));
        fout.write((char*)&this->verticalResolution, sizeof(int32_t));
        fout.write((char*)&this->colorTableEntries, sizeof(uint32_t));
        fout.write((char*)&this->importantColors, sizeof(uint32_t));
    } 
};

struct Pixel {
    /*
    Struct to define bgr values of a bitmap pixel.
    Has a method to wirte to file.
    */
	uint8_t blue;
	uint8_t green;
	uint8_t red;

    void save_on_file(std::ofstream& fout) {
        fout.write((char*)&this->blue, sizeof(uint8_t));
        fout.write((char*)&this->green, sizeof(uint8_t));
        fout.write((char*)&this->red, sizeof(uint8_t));
    }


};

void Number2Color(float NUMBER, vector<int>& RGB){
    /*
    An awful colormap :)
    */
    float A = -1*log(log((M_E-1)*0.5 + 1))/(0.5*0.5);

    RGB[2] = std::clamp(int(255*(0.5*exp(-A*NUMBER*NUMBER))), 0, 255);
    RGB[1] = std::clamp(int(255*(0.25F*NUMBER)), 0, 255);
    RGB[0] = std::clamp(int(255*log((M_E-1)*NUMBER + 1)), 0, 255);
}




void SaveBMP(string name, int DIMENSION, vector<vector<float>>& STATE, vector<int>& RGB){

    /*
    Method to save a simulation state to a bitmap.
    */
	
    ofstream fout(name, ios::binary); //Open file

    BmpHeader bmpHeader;
    BmpInfoHeader bmpInfoHeader; // Create necessary structs
    Pixel pixel;

    // Save necessary info on structs.
    bmpHeader.sizeOfBitmapFile = 54 + DIMENSION*DIMENSION*3;
    bmpInfoHeader.height       = DIMENSION;
    bmpInfoHeader.width        = DIMENSION;

    bmpHeader.save_on_file(fout);
    bmpInfoHeader.save_on_file(fout); // Save headers

    // Iterate over each value and save to pixel
    for (int i = 0; i < DIMENSION; i++){
        for (int j = 0; j < DIMENSION; j++){
            Number2Color(STATE[j][i], RGB); // Conversion to color is done here
            pixel.red   = RGB[0];
            pixel.green = RGB[1];
            pixel.blue  = RGB[2];
            pixel.save_on_file(fout); // Saving
        }
    }
    fout.close(); // Closing

}

void SaveFile(string FNAME, int FNUMBER, vector<vector<float>>& STATE, int DIMENSION, vector<int>& BGR){

    // Parent method to save a state to file

    string num  = to_string(FNUMBER);
    string namebmp = "./FRAMES/"+FNAME+"_"+num+".bmp"; // Define file names
    string namedat = "./FRAMES/"+FNAME+"_"+num+".dat";

    std::ofstream myFile; // Create and open data file
    myFile.open(namedat, std::ios::out);

    // Iterate over state and save to data file
    for (int i = 0; i < DIMENSION; i++){
        for (int j = 0; j < DIMENSION; j++){
            if (j < DIMENSION -1){
                myFile << STATE[i][j] << ",";
            }
            else{
                myFile << STATE[i][j] << "\n";
            }
        }
    }
 
    myFile.close();

    SaveBMP(namebmp, DIMENSION, STATE, BGR); // Save bitmap

}
