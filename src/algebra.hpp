#include <vector>
#include <string>
using namespace std;

void Laplacian(vector<vector<float>>& STATE,
               vector<vector<float>>& LAPL,
               int DIMENSION,
               float dx4,
               string BOUNDARY);