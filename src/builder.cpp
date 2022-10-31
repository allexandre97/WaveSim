#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include "builder.hpp"

vector<float> BuildSpace(int DIMENSION){
    /*
    Creates an array of the desired number of points from -1 to +1
    */
    float dx = 2.0F/(DIMENSION-1);
    vector<float> X(DIMENSION);
    for (int i = 0; i < DIMENSION; i++){
        X[i] = -1.0F+ i*dx;
    }
    return X;
}

void BuildSinc(vector<float> X,
               int DIMENSION,
               float X0,
               float Y0,
               float AMPLITUDE,
               float NU,
               float DECAY,
               vector<vector<float>>& OUT){
    
    /*
    Builds a function sinc(x) = exp(-decay*x^2) * (Amplitude*sin(2*pi*nu*x)) / (2*pi*nu*x)
    */

    #pragma omp parallel for
    for (int i = 0; i < DIMENSION; i++){
        float x = X[i];
        #pragma omp parallel for
        for (int j = 0; j < DIMENSION; j++){
            float y = X[j];
            float r = sqrt(pow((x-X0),2) + pow((y-Y0),2));
            OUT[i][j] += AMPLITUDE*(sin(2*M_PI*NU*r)/(2*M_PI*NU*r))*exp(-DECAY*r*r);
        }
    }
}

void BuildSin(vector<float> X,
              int DIMENSION,
              float X0,
              float Y0,
              float AMPLITUDE,
              float NU,
              vector<vector<float>>& OUT){
    /*
    Builds a function sin(2*pi*nux) 
    */
    #pragma omp parallel for
    for (int i = 0; i < DIMENSION; i++){
        float x = X[i];
        #pragma omp parallel for
        for (int j = 0; j < DIMENSION; j++){
            float y = X[j];
            float r = sqrt(pow(x-X0, 2) + pow(y-Y0, 2));
            OUT[i][j] += cos(2*M_PI*NU*r);
        }
    }
}

void Normalize(vector<vector<float>>& OUT, int DIMENSION){

    /*
    Normalize the output space so min = 0, max = 1
    */
    
    float min, max;
    min = OUT[0][0];
    for (int i = 0; i < DIMENSION; i++){
        for (int j = 0; j < DIMENSION; j++){
            if (OUT[i][j] < min){
                min = OUT[i][j];
            }
        }
    }

    if (min < 0.0F){
        min *= -1.0F;
    }

    for (int i = 0; i < DIMENSION; i++){
        for (int j = 0; j < DIMENSION; j++){
            OUT[i][j] = (OUT[i][j] + min);
        }
    }


    max = OUT[0][0];
    for (int i = 0; i < DIMENSION; i++){
        for (int j = 0; j < DIMENSION; j++){
            if (OUT[i][j] > max){
                max = OUT[i][j];
            }
        }
    }

    for (int i = 0; i < DIMENSION; i++){
        for (int j = 0; j < DIMENSION; j++){
            OUT[i][j] = OUT[i][j]/max;
        }
    }
    
   

}

vector<vector<float>> Build(int DIMENSION,
                            int NKERNELS,
                            vector<string> KERNELS,
                            vector<float> X0,
                            vector<float> Y0,
                            vector<float> AMPLITUDE,
                            vector<float> FREQUENCY,
                            vector<float> DECAY,
                            std::string OutName){
    /*
    Builds the starting space configuration based on the parsed arguments.
    */
    

    // Open and write to log file
    std::ofstream LogFile;
    LogFile.open(OutName+".log", std::ios_base::app);
    std::string msg;

    msg = fmt::format("#~#~#~#~#~ BUILDING {}x{} SIMULATION SPACE ~#~#~#~#~#\n\n", DIMENSION, DIMENSION);
    std::cout << msg << std::endl;
    LogFile << msg;

    vector<float> X;
    X = BuildSpace(DIMENSION);

    // Initialize array for starting space
    vector<vector<float>> OUT(DIMENSION);
    for (int i = 0; i < DIMENSION; i++){
        vector<float> tmp (DIMENSION);
        OUT[i] = tmp;
    }

    // Iterate over number of kernels and add them to starting space
    int nsin, nsinc;
    nsin  = 0;
    nsinc = 0;
    for (int n = 0; n < NKERNELS; n++){
        
        vector<vector<float>> fun;

        string kernel = KERNELS[n];

        if (kernel == "SIN"){
            msg = fmt::format("BUILDING KERNEL #{}.\n"
                              "Type:      {}\n"
                              "Center: {},{}\n"
                              "Amplitude: {}\n"
                              "Frequency: {}\n\n",
                              n+1, kernel, X0[n], Y0[n], AMPLITUDE[n], FREQUENCY[n]);
            std::cout << msg << std::endl;
            LogFile << msg;
            BuildSin(X, DIMENSION, X0[n], Y0[n], AMPLITUDE[n], FREQUENCY[n], OUT);
        }
        else if (kernel == "SINC"){
            msg = fmt::format("BUILDING KERNEL #{}.\n"
                              "Type:      {}\n"
                              "Center:    {} , {}\n"
                              "Amplitude: {}\n"
                              "Frequency: {}\n"
                              "Decay:     {}\n\n",
                              n+1, kernel, X0[n], Y0[n], AMPLITUDE[n], FREQUENCY[n], DECAY[nsinc]);
            std::cout << msg << std::endl;
            LogFile << msg;
            BuildSinc(X, DIMENSION, X0[n], Y0[n], AMPLITUDE[n], FREQUENCY[n], DECAY[nsinc], OUT);
            nsinc += 1;
        }
    }

    msg = "#~#~#~#~#~ NORMALIZING SIMULATION SPACE. ~#~#~#~#~#\n\n";
    std::cout << msg << std::endl;
    LogFile << msg;

    Normalize(OUT, DIMENSION);

    msg = "#~#~#~#~#~ BUILDING DONE ~#~#~#~#~#\n\n";
    std::cout << msg << std::endl;
    LogFile << msg;
    LogFile.close();

    //BuildVisual(DIMENSION, OUT);

    return OUT;
}
