#include <cmath>
#include <omp.h>

#include "algebra.hpp"

void Gradient(vector<vector<float>>& STATE,
              vector<vector<vector<float>>>& GRAD,
              int DIMENSION,
              float dx2){

    /*
    Computes the gradient of a scalar function in 2D Euclidean Space.
    */
    
    // Compute gradient for all points except bounds
    #pragma omp parallel for
    for (int i = 1; i < DIMENSION - 1; i++){

        // Bounds for Y except corners
        GRAD[i][0][0]           = (STATE[i+1][0] - STATE[i-1][0])/dx2;
        GRAD[i][DIMENSION-1][0] = (STATE[i+1][DIMENSION-1] - STATE[i-1][DIMENSION-1])/dx2;
        GRAD[i][0][1]           = (STATE[i][1] - STATE[i][DIMENSION-1])/dx2;
        GRAD[i][DIMENSION-1][1] = (STATE[i][0] - STATE[i][DIMENSION-2])/dx2; 
        
        #pragma omp parallel for
        for (int j = 1; j < DIMENSION - 1; j++){

            // Bounds for X except corners
            GRAD[0][j][1]           = (STATE[0][j+1] - STATE[0][j-1])/dx2;
            GRAD[DIMENSION-1][j][1] = (STATE[DIMENSION-1][j+1] - STATE[DIMENSION-1][j-1])/dx2;
            GRAD[0][j][0]           = (STATE[1][j] - STATE[DIMENSION-1][j])/dx2;
            GRAD[DIMENSION-1][j][0] = (STATE[0][j] - STATE[DIMENSION-2][j])/dx2; 

            // Central points
            GRAD[i][j][0] = (STATE[i+1][j] - STATE[i-1][j])/dx2;
            GRAD[i][j][1] = (STATE[i][j+1] - STATE[i][j-1])/dx2;
        }
    }

    // Compute corners

    GRAD[0][0][0] = (STATE[1][0] - STATE[DIMENSION-1][0])/dx2;
    GRAD[0][0][1] = (STATE[0][1] - STATE[0][DIMENSION-1])/dx2;

    GRAD[0][DIMENSION-1][0] = (STATE[1][DIMENSION-1] - STATE[DIMENSION-1][DIMENSION-1])/dx2;
    GRAD[0][DIMENSION-1][1] = (STATE[0][0] - STATE[0][DIMENSION-2])/dx2;

    GRAD[DIMENSION-1][0][0] = (STATE[0][0] - STATE[DIMENSION-2][0])/dx2;
    GRAD[DIMENSION-1][0][1] = (STATE[DIMENSION-1][1] - STATE[DIMENSION-1][DIMENSION-2])/dx2;

    GRAD[DIMENSION-1][DIMENSION-1][0] = (STATE[0][DIMENSION-1] - STATE[DIMENSION-2][DIMENSION-1])/dx2;
    GRAD[DIMENSION-1][DIMENSION-1][1] = (STATE[DIMENSION-1][0] - STATE[DIMENSION-1][DIMENSION-2])/dx2;
}

void Jacobian(vector<vector<float>>& COMP1,
              vector<vector<float>>& COMP2,
              vector<vector<vector<float>>>& GRAD1,
              vector<vector<vector<float>>>& GRAD2,
              vector<vector<vector<vector<float>>>>& JACOB,
              int DIMENSION,
              float dx){

    /*
    Computes the Jacobian of a vector function in 2D space.
    Jacobiant is the Gradient of each vector component.
    */

    Gradient(COMP1, GRAD1, DIMENSION, 1); // Gradient of component 1
    Gradient(COMP2, GRAD2, DIMENSION, 1); // Gradient of component 2

    // Store in output array
    #pragma omp parallel for
    for (int i = 0; i < DIMENSION; i++){
        #pragma omp parallel for
        for (int j = 0; j < DIMENSION; j++){

            JACOB[i][j][0][0] = GRAD1[i][j][0];
            JACOB[i][j][0][1] = GRAD1[i][j][1];
            JACOB[i][j][1][0] = GRAD2[i][j][0];
            JACOB[i][j][1][1] = GRAD2[i][j][1];
        }
    }
}

void Trace(vector<vector<vector<vector<float>>>>& JACOB,
           vector<vector<float>>& TRACE,
           int DIMENSION){
    
    /*
    Computes the Trace of a Tensorial function in 2d space.
    */

    #pragma omp parallel for
    for (int i = 0; i < DIMENSION; i++){
        #pragma omp parallel for
        for (int j = 0; j < DIMENSION; j++){
            TRACE[i][j] = JACOB[i][j][0][0] + JACOB[i][j][1][1]; 
        }
    }

}

void Laplacian(vector<vector<float>>& STATE,
               vector<vector<vector<float>>>& GRAD,
               vector<vector<float>>& COMP1,
               vector<vector<float>>& COMP2,
               vector<vector<vector<float>>>& GRAD1,
               vector<vector<vector<float>>>& GRAD2,
               vector<vector<vector<vector<float>>>>& JACOB,
               vector<vector<float>>& LAPL,
               int DIMENSION,
               float dx){
    
    /*
    Computes the Laplacian of a scalar function in 2d space.
    */

    Gradient(STATE, GRAD, DIMENSION, dx); // Calculate the gradient of scalar function

    // Separate gradient in its two components
    #pragma parallel for
    for (int i = 0; i <  DIMENSION; i++){
        #pragma parallel for
        for (int j = 0; j < DIMENSION; j++){
            COMP1[i][j] = GRAD[i][j][0];
            COMP2[i][j] = GRAD[i][j][1];
        }
    }

    // Calculate the Jacobian via the gradient of the components
    Jacobian(COMP1, COMP2, GRAD1, GRAD2, JACOB, DIMENSION, dx);

    // Laplacian is the trace of the Jacobian
    Trace(JACOB, LAPL, DIMENSION);

}