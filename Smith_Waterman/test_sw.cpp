#include "smith_waterman.cpp"

bool test_score_matrix(){
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    int n = strlen(sequence_A);
    int m = strlen(sequence_B);
    std::vector<std::vector<int> > *H;
    std::vector<std::vector<TracebackDirection> > *traceback_matrix;
    AlignmentType at = LOCAL;
    score_matrix(sequence_A, sequence_B, H, traceback_matrix, at);

    std::vector< std::vector<int> > H_exa = {{0, 0, 5, 1, 5, 1, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 1, 2, 1, 10, 6, 2, 0, 0, 5, 1},
                                        {0, 5, 1, 0, 0, 6, 7, 3, 0, 5, 1, 2},
                                        {0, 1, 2, 6, 2, 2, 3, 12, 8, 4, 2, 6},
                                        {0, 0, 0, 7, 3, 0, 0, 8, 17, 13, 9, 7},
                                        {0, 0, 0, 3, 4, 8, 5, 4, 13, 14, 18, 14},
                                        {0, 5, 1, 0, 0, 4, 5, 2, 9, 18, 14, 15}};

    for (int i = 1; i < n+1; i++){
        for (int j = 1; j < m+1; j++){
            if ((*H)[i][j] != H_exa[i][j]){
                return false;
            }
        }
    }
    return true;
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