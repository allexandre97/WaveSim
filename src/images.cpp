#include "images.hpp"

void Number2Color(float NUMBER, vector<int>& RGB){
    float A = -1*log(log((M_E-1)*0.5 + 1))/(0.5*0.5);

    RGB[2] = std::clamp(int(255*(0.5*exp(-A*NUMBER*NUMBER))), 0, 255);
    RGB[1] = std::clamp(int(255*(0.25F*NUMBER)), 0, 255);
    RGB[0] = std::clamp(int(255*log((M_E-1)*NUMBER + 1)), 0, 255);
}

void SaveBMP(string name, int DIMENSION, vector<vector<float>>& STATE, vector<int>& RGB){
	
    ofstream fout(name, ios::binary);

    BmpHeader bmpHeader;
    BmpInfoHeader bmpInfoHeader;
    Pixel pixel;

    bmpHeader.sizeOfBitmapFile = 54 + DIMENSION*DIMENSION*3;
    bmpInfoHeader.height       = DIMENSION;
    bmpInfoHeader.width        = DIMENSION;

	fout.write((char *) &bmpHeader, 14);
	fout.write((char *) &bmpInfoHeader, 40);

    for (int i = 0; i < DIMENSION; i++){
        for (int j = 0; j < DIMENSION; j++){
            Number2Color(STATE[i][j], RGB);
            pixel.red   = RGB[0];
            pixel.green = RGB[1];
            pixel.blue  = RGB[2];
            fout.write((char *) &pixel, 3);
        }
    }
    fout.close();

}