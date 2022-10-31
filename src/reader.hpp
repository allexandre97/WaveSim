#include <vector>
#include <string>

using namespace std;

struct Params{
    int NKernels;
    int NSTEPS;
    int NOUT;
    int DIMENSION;
    float COURANT;
    vector<string> Kernels;
    vector<float> X0;
    vector<float> Y0;
    vector<float> AMPLITUDE;
    vector<float> FREQUENCY;
    vector<float> DECAY;
};

Params ReadFile(string FileName);


