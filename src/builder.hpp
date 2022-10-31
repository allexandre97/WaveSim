#include <vector>
using namespace std;

vector<vector<float>> Build(int DIMENSION,
                            int NKERNELS,
                            vector<string> KERNELS,
                            vector<float> X0,
                            vector<float> Y0,
                            vector<float> AMPLITUDE,
                            vector<float> FREQUENCY,
                            vector<float> DECAY,
                            std::string OutName);