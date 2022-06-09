//#include "smith_waterman.cpp"
#include "sequence_alignment_parallel.cpp"

// bool test_score_matrix(){
//     char* sequence_A = "GACTTAC";
//     char* sequence_B = "CGTGAATTCAT";
//     int n = strlen(sequence_A);
//     int m = strlen(sequence_B);
//     std::vector<std::vector<int> > *H;
//     std::vector<std::vector<TracebackDirection> > *traceback_matrix;

//     //We instantiate a 2d vector with default values 0: 
//     int default_value = 0;
//     H = new std::vector<std::vector<int> >(n+1, std::vector<int>(m+1, default_value));
//     traceback_matrix = new std::vector<std::vector<TracebackDirection> >(n+1, std::vector<TracebackDirection>(m+1, TracebackDirection::INVALID));

//     AlignmentType at = AlignmentType::LOCAL;
//     score_matrix(sequence_A, sequence_B, H, traceback_matrix, at);

//     std::vector< std::vector<int> > H_exa = {
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 5, 1, 5, 1, 0, 0, 0, 0, 0, 0},
//         {0, 0, 1, 2, 1, 10, 6, 2, 0, 0, 5, 1},
//         {0, 5, 1, 0, 0, 6, 7, 3, 0, 5, 1, 2},
//         {0, 1, 2, 6, 2, 2, 3, 12, 8, 4, 2, 6},
//         {0, 0, 0, 7, 3, 0, 0, 8, 17, 13, 9, 7},
//         {0, 0, 0, 3, 4, 8, 5, 4, 13, 14, 18, 14},
//         {0, 5, 1, 0, 0, 4, 5, 2, 9, 18, 14, 15}
//     };

//     for (int i = 1; i < n+1; i++){
//         for (int j = 1; j < m+1; j++){
//             if ((*H)[i][j] != H_exa[i][j]){
//                 return false;
//             }
//         }
//     }

//     delete H;
//     delete traceback_matrix;
//     return true;
// }

bool test_cells_in_sequence_alignment(){
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    
    SequenceAlignment s_a(sequence_A, sequence_B, 3, 1, 1, 2, 3, -5);
    std::vector<Block> blocks;
    s_a.cells(7, blocks);

    for (int i = 0; i < blocks.size(); i++){
        std::cout << blocks[i].startX << blocks[i].startY;
    }
    return true;
}

int main(){

    // if (test_score_matrix()){
    //     cout << "Test 1 passed!" << endl;
    // }

    test_cells_in_sequence_alignment();

    // for (int i = 0; i < H.size(); i++){
    //     for (int j = 0; j < H[i].size(); j++){
    //     cout << H[i][j];
    //     }
    //     cout << std::endl;
    // }
    return 0;
    }