#include <vector>
using namespace std;

void Laplacian(vector<vector<float>>& STATE,
               vector<vector<vector<float>>>& GRAD,
               vector<vector<float>>& COMP1,
               vector<vector<float>>& COMP2,
               vector<vector<vector<float>>>& GRAD1,
               vector<vector<vector<float>>>& GRAD2,
               vector<vector<vector<vector<float>>>>& JACOB,
               vector<vector<float>>& LAPL,
               int DIMENSION,
               float dx);