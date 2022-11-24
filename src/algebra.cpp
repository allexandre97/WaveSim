#include <cmath>
#include <omp.h>

#include "algebra.hpp"

void Laplacian(vector<vector<float>>& STATE,
          vector<vector<float>>& LAPL,
          int DIMENSION,
          float dx4){
    
    int XP2;
    int YP2;
    int XM2;
    int YM2;

    #pragma omp parallel for
    for (int i = 0; i < DIMENSION; i++){
        XP2 = (i + 2) % DIMENSION;
        XM2 = (DIMENSION - (i - 2)) % DIMENSION;
        #pragma omp parallel for
        for (int j = 0; j < DIMENSION; j++){
            YP2 = (j + 2) % DIMENSION;
            YM2 = (DIMENSION - (j - 2)) % DIMENSION;
            LAPL[i][j] = (STATE[XP2][j] + STATE[XM2][j] - 2*STATE[i][j])/dx4 + (STATE[i][YP2] + STATE[i][YM2] - 2*STATE[i][j])/dx4;
        }
    }

}