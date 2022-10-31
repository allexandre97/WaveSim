#include <vector>
#include <fmt/core.h>
#include <fmt/ranges.h>
using namespace std;

void TwoWaySolver(int NSteps,
                  int NOut,
                  std::string OutName,
                  vector<vector<float>>& STATE,
                  int DIMENSION,
                  float C);