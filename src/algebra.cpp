#include <cmath>
#include <omp.h>
#include <iostream>

#include "algebra.hpp"

void Laplacian(vector<vector<float>>& STATE,
               vector<vector<float>>& LAPL,
               int DIMENSION,
               float dx4,
               string BOUNDARY){
    
    int XP2;
    int YP2;
    int XM2;
    int YM2;

    if (BOUNDARY == "PERIODIC"){
        #pragma omp parallel for
        for (int i = 0; i < DIMENSION; i++){
            XP2 = (i + 2) % DIMENSION;
            XM2 = (DIMENSION + (i - 2)) % DIMENSION;
            #pragma omp parallel for
            for (int j = 0; j < DIMENSION; j++){
                YP2 = (j + 2) % DIMENSION;
                YM2 = (DIMENSION + (j - 2)) % DIMENSION;
                LAPL[i][j] = (STATE[XP2][j] + STATE[XM2][j] - 2*STATE[i][j])/dx4 + (STATE[i][YP2] + STATE[i][YM2] - 2*STATE[i][j])/dx4;
            }
        }
    }

    if (BOUNDARY == "NEUMANN"){
        #pragma omp parallel for
        for (int i = 0; i < DIMENSION; i++){
            XP2 = (i + 2) * (1 - (int)(i + 2)/DIMENSION) + ( DIMENSION - 2 -(i+2)%DIMENSION )*((int)(i + 2)/DIMENSION);
            XM2 = abs(i - 2);
            #pragma omp parallel for
            for (int j = 0; j < DIMENSION; j++){
                YP2 = (j + 2) * (1 - (int)(j + 2)/DIMENSION) + ( DIMENSION - 2 -(j+2)%DIMENSION )*((int)(j + 2)/DIMENSION);
                YM2 = abs(j - 2);
                LAPL[i][j] = (STATE[XP2][j] + STATE[XM2][j] - 2*STATE[i][j])/dx4 + (STATE[i][YP2] + STATE[i][YM2] - 2*STATE[i][j])/dx4;
            }
        }
    }
}



