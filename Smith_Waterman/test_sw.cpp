#include "smith_waterman.cpp"

bool test_score_matrix(){
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    int n = strlen(sequence_A);
    int m = strlen(sequence_B);
    std::vector< std::vector<int> > H = score_matrix(sequence_A, sequence_B);

    std::vector< std::vector<int> > H_exa;
    return (H == H_exa);
}

int main(){

    if (test_score_matrix()){
        cout << "Test 1 passed!";
    }

    // for (int i = 0; i < H.size(); i++){
    //     for (int j = 0; j < H[i].size(); j++){
    //     cout << H[i][j];
    //     }
    //     cout << std::endl;
    // }
    return 0;
    }