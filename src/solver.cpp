#include <iostream>
#include <fstream>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include "algebra.hpp"
#include "saver.hpp"

#include "solver.hpp"

void FirstStep(vector<vector<float>>& STATE,
               vector<vector<float>>& LAPL,
               vector<vector<float>>& NEWSTATE,
               int DIMENSION,
               float dt,
               float c){

    /*
    Method to calculate the first simulation step
    */
    
    #pragma omp parallel for
    for (int i = 0; i < DIMENSION; i++){
        #pragma omp parallel for
        for (int j = 0; j < DIMENSION; j++){
            float result;
            result = STATE[i][j] + 0.5*(dt*dt*c*c)*LAPL[i][j];
            NEWSTATE[i][j] = result;
        }
    }
}

void NthStep(vector<vector<float>>& STATE,
             vector<vector<float>>& PREVSTATE,
             vector<vector<float>>& LAPL,
             vector<vector<float>>& NEWSTATE,
             int STEP,
             int DIMENSION,
             float dt,
             float c){

    /*
    Method to calculate the N-th simulation step
    */
    
    int frame = STEP % 250;
    #pragma omp parallel for
    for (int i = 0; i < DIMENSION; i++){
        #pragma omp parallel for
        for (int j = 0; j < DIMENSION; j++){
            float result;
            result            = 2*STATE[i][j] - PREVSTATE[i][j] + (dt*dt*c*c)*LAPL[i][j];
            NEWSTATE[i][j]    = result;
        }
    }
}

void Iterate(int NSteps,
             int NOut,
             std::string OutName,
             std::string msg,
             int DIMENSION,
             float dx,
             float dt,
             float c,
             vector<int>& BGR,
             vector<vector<float>>& STATE,
             vector<vector<float>>& OLDSTATE,
             vector<vector<float>>& NEWSTATE,
             vector<vector<float>>& LAPL
             ){
    
    /*
    Method to iterate over the simulation steps.
    */
    
    std::ofstream LogFile;
    LogFile.open(OutName+".log", std::ios_base::app);

    int nout = 1;
    int percent;
    for (int n = 1; n < NSteps+1; n++){

        Laplacian(STATE, LAPL, DIMENSION, dx*4);
        NthStep(STATE, OLDSTATE, LAPL, NEWSTATE, n, DIMENSION, dt, c);

        OLDSTATE = STATE;
        STATE = NEWSTATE;

        percent = 100*(n+1)/(NSteps+1);

        msg = fmt::format("Computing step --> {}%", percent);
        std::cout << msg+"\r";
        LogFile << msg+"\n";

        if (n % NOut == 0){
            SaveFile(OutName, nout, STATE, DIMENSION, BGR);
            nout += 1;
        }

    }
    msg = "\n#~#~#~#~#~ FINISHING SIMULATION ~#~#~#~#~#\n";
    std::cout << msg << std::endl;
    LogFile << msg;
    LogFile.close();
}

void TwoWaySolver(int NSteps,
                  int NOut,
                  std::string OutName,
                  vector<vector<float>>& STATE,
                  int DIMENSION,
                  float C){
    
    /*
    Parent method to call the solver. 
    Allocates all needed memory and solves the equation in time.
    */

    int folder = system("mkdir ./FRAMES/");

    std::ofstream LogFile;
    LogFile.open(OutName+".log", std::ios_base::app);
    std::string msg;

    msg = "#~#~#~#~#~ ALLOCATING ALL REQUESTED MEMORY ~#~#~#~#~#\n\n";
    std::cout << msg << std::endl;
    LogFile << msg;

    float c  = 1.0F;
    float dx = 1.0F;
    float dt = C*dx/c;
    
    vector<int> BGR(3);
    vector<vector<float>> OLDSTATE(DIMENSION);
    vector<vector<float>> NEWSTATE(DIMENSION);
    vector<vector<float>> LAPL(DIMENSION);


    for (int i = 0; i < DIMENSION; i++){

        vector<float> tmpOLDS(DIMENSION);
        vector<float> tmpNEWS(DIMENSION);
        vector<float> tmpLAPL(DIMENSION);

        OLDSTATE[i] = tmpOLDS;
        NEWSTATE[i] = tmpNEWS;
        LAPL[i]     = tmpLAPL;

    }


    msg = "#~#~#~#~#~ SIMULATION START ~#~#~#~#~#\n\n";
    std::cout << msg << std::endl;
    LogFile << msg+"\n";
    
    msg = "Computing Step --> 0%";
    std::cout << msg+"\r";
    LogFile << msg+"\n";
    
    Laplacian(STATE, LAPL, DIMENSION, dx*4);
    FirstStep(STATE, LAPL, NEWSTATE, DIMENSION, dt, c);

    SaveFile(OutName, 1, STATE, DIMENSION, BGR);

    OLDSTATE = STATE;
    STATE    = NEWSTATE;

    LogFile.close();

    Iterate(NSteps, NOut, OutName, msg, DIMENSION, dx, dt, c, BGR, STATE, OLDSTATE, NEWSTATE, LAPL);
}

