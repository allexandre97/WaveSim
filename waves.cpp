#include <vector>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <omp.h>
#include "solver.hpp"
#include "builder.hpp"
#include "reader.hpp"

// DEFINE SOME NEEDED VARIABLES
int DIMENSION, NSTEPS, NOUT, NTHREADS;
std::string InFile = "";
std::string OutName = "";
double COURANT;
Params myPars;

void PrintHelp(){
    /*
    Prints some help to screen
    */
    std::cout <<
            "\nWelcome to WaveSim, a C++ 2d wave simulation engine!\n\n"
            "Example usage: WaveSim -f input.inp -t 2 --> Executes the program on two threads using input information from input.inp\n\n"
            "-d, --dimension        <n>: Set lateral dimension, i.e, if -d 500 a 500x500 space will be used.\n"
            "-n, --nsteps           <n>: Number of simulation steps.\n"
            "-o, --nout             <n>: Number of steps between file writes.\n"
            "-c, --courant        <val>: Courant number, affects speed of wave propagation. Cannot be bigger than 1.0.\n"
            "-f, --file      <filename>: Name of input file containing simulation parameters. ATTENTION! THE FILE PARAMETERS ARE OVERWRITTEN IF THEY ARE ALSO DEFINED WITH A FLAG!\n"
            "-w, --writefile <filename>: Name for the output files.\n"
            "-t, --nthreads         <n>: Number of threads to use.\n"
            "-h, --help                : Show this help.\n";
    exit(0);
}

void Summary(){

    /*
    Writes a summary of the simulation parameters, both to terminal and to a log file
    */

    std::string msg = fmt::format("\nWriting a Summary of the Selected Parameters\n\n"
                                  "Number of Threads        --> {}\n"
                                  "Simulation steps         --> {}\n"
                                  "Output frequency         --> {}\n"
                                  "Simulation Size          --> {}x{} points\n"
                                  "Courant Number           --> {}\n"
                                  "Kernel Types             --> {}\n"
                                  "Kernel Centers [X],[Y]   --> {}, {}\n"
                                  "Kernel Amplitudes        --> {}\n"
                                  "Kernel Frequencies       --> {}\n"
                                  "Kernel Decays            --> {}\n"
                                  "Output Names             --> {}.x\n\n",
                                  NTHREADS,
                                  NSTEPS, NOUT,
                                  DIMENSION, DIMENSION,
                                  COURANT,
                                  myPars.Kernels,
                                  myPars.X0, myPars.Y0,
                                  myPars.AMPLITUDE,
                                  myPars.FREQUENCY,
                                  myPars.DECAY,
                                  OutName);

    std::cout << msg << std::endl;

    ofstream myFile;
    myFile.open(OutName+".log", ios::out);
    myFile << msg;
    myFile.close();
}

void CatchMissing(){

    /*
    Catches if some needed parameter is missing.
    If something is missing it assigns a default value, when possible.
    */

    int miss = 0;
    if (!NTHREADS){
        std::cout << "Number of threads not found!\n"
                     "Using default value --> 1" << std::endl;
        NTHREADS = 1;
    }
    if (!NSTEPS){
        std::cout << "Number of steps not set!\n"
                     "Using Default value --> 1000" << std::endl;
        NSTEPS = 1000;
    }
    if (!NOUT){
        std::cout << "Output frequency not set!\n"
                     "Using Default value --> 10" << std::endl;
        NOUT = 10;
    }
    if (!DIMENSION){
        std::cout << "Simulation size not set!\n"
                     "Using Default value --> 250x250 points" << std::endl;
        DIMENSION = 250;
    }
    if (!COURANT){
        std::cout << "Courant number not set!\n"
                     "Using Default value --> 0.05" << std::endl;
        COURANT = 0.05F;
    }
    if (OutName == ""){
        std::cout << "Output name not set!\n"
                     "Using Default value --> out.x" << std::endl;
        OutName = "out";
    }
    if (myPars.Kernels.size() != myPars.NKernels || myPars.Kernels.size() < 1){
        std::cout << "Kernel types not set!\n"
                     "This is a mandatory parameter :(" << std::endl;
        miss = 1;
    }
    if (myPars.X0.size() != myPars.NKernels || myPars.Kernels.size() < 1){
        std::cout << "Kernel X Centers not set!\n"
                     "This is a mandatory parameter :(" << std::endl;
        miss = 1;
    }
    if (myPars.Y0.size() != myPars.NKernels || myPars.Kernels.size() < 1){
        std::cout << "Kernel Y Centers not set!\n"
                     "This is a mandatory parameter :(" << std::endl;
        miss = 1;
    }
    if (myPars.AMPLITUDE.size() != myPars.NKernels || myPars.Kernels.size() < 1){
        std::cout << "Kernel Amplitudes not set!\n"
                     "This is a mandatory parameter :(" << std::endl;
        miss = 1;
    }
    if (myPars.FREQUENCY.size() != myPars.NKernels || myPars.Kernels.size() < 1){   
        std::cout << "Kernel Frequencies not set!\n"
                     "This is a mandatory parameter :(" << std::endl;
        miss = 1;
    }
    if (myPars.DECAY.size() != myPars.NKernels || myPars.Kernels.size() < 1){
        int found = 0;
        for (auto &ker : myPars.Kernels){
            if (ker == "SINC"){
                found = 1;
                break;
            }
        }
        if (found == 1){
            std::cout << "Kernel Decays not set!\n"
                         "This is a mandatory parameter :(" << std::endl;
            miss = 1;
        }
    }
    if (miss == 1){
        std::cout << "Exiting..." << std::endl;
        exit(-1);
    }
}

void ProcessArgs(int argc, char** argv){

    /*
    Processes arguments passed to function from command line
    */

    const char* const short_opts = "d:n:o:c:f:w:t:h";
    const option long_opts[] = {
            {"dimension", required_argument, nullptr, 'd'},
            {"nsteps",    required_argument, nullptr, 'n'},
            {"nout",      required_argument, nullptr, 'o'},
            {"courant",   required_argument, nullptr, 'c'},
            {"file",      required_argument, nullptr, 'f'},
            {"writefile", required_argument, nullptr, 'w'},
            {"nthreads",  required_argument, nullptr, 't'},
            {"help",      no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
    };

    while (true){

        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt){

            case 'd':
                DIMENSION = std::stoi(optarg);
                break;

            case 'n':
                NSTEPS = std::stoi(optarg);
                break;

            case 'o':
                NOUT = std::stoi(optarg);
                break;

            case 'c':
                COURANT = std::stof(optarg);
                if (COURANT > 1.0F){
                    COURANT = 0.99F;
                    std::cout << "Courant number was bigger than one!" << std::endl;
                    std::cout << "Courant number set to " << COURANT << std::endl;
                }
                break;
            
            case 'f':
                InFile = std::string(optarg);
                break;
            
            case 'w':
                OutName = std::string(optarg);
                break;

            case 't':
                NTHREADS = std::stoi(optarg);
                break;

            case 'h': // -h or --help
            case '?': // Unrecognized option
                PrintHelp();
                break;

            default:
                break;
        }
    }
}

int main(int argc, char* argv[]){

    ProcessArgs(argc, argv); //Read command line options

    // Read input file
    if (InFile != ""){
        myPars = ReadFile(InFile);
        if (!DIMENSION){
            DIMENSION = myPars.DIMENSION;
        }
        if (!NSTEPS){
            NSTEPS = myPars.NSTEPS;
        }
        if (!NOUT){
            NOUT = myPars.NOUT;
        }
        if (!COURANT){
            COURANT = myPars.COURANT;
            if (COURANT > 1.0F){
                COURANT = 0.99F;
                std::cout << "Courant number was bigger than one!" << std::endl;
                std::cout << "Courant number set to " << COURANT << std::endl;
            }
        }
    }

    CatchMissing();
    ofstream LogFile; // Create log file
    Summary();

    omp_set_num_threads(NTHREADS); // Set number of OMP threads

    // Build simulation space
    vector<vector<float>> STATE = Build(DIMENSION, 
                                        myPars.NKernels, 
                                        myPars.Kernels, 
                                        myPars.X0, 
                                        myPars.Y0, 
                                        myPars.AMPLITUDE, 
                                        myPars.FREQUENCY, 
                                        myPars.DECAY,
                                        OutName);
    
    // Solve simulation
    TwoWaySolver(NSTEPS, NOUT, OutName, STATE, DIMENSION, COURANT);

    return 0;
}

