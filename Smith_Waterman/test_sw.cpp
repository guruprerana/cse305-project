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
    char* sequence_A = "GACTTACTA";
    char* sequence_B = "CGTGAATTC";
    
    SequenceAlignment s_a(sequence_A, sequence_B, 9, 9, 3, 1, 1, 2, 3, -5);
    std::vector<Block> blocks_thread1_phase7;
    s_a.phase = 7;
    s_a.cells(1, blocks_thread1_phase7);

    std::vector<Block> expected_blocks_thread1_phase7;
    Block block1, block2, block3;
    block1.startX = 1;
    block1.startY = 7;
    block2.startX = 4;
    block2.startY = 4;
    block3.startX = 7;
    block3.startY = 1;
    expected_blocks_thread1_phase7.push_back(block1);
    expected_blocks_thread1_phase7.push_back(block2);
    expected_blocks_thread1_phase7.push_back(block3);


    for (int i = 0; i < blocks_thread1_phase7.size(); i++){
        bool test_1 = (blocks_thread1_phase7[i].startX == expected_blocks_thread1_phase7[i].startX);
        bool test_2 = (blocks_thread1_phase7[i].startY == expected_blocks_thread1_phase7[i].startY);
        if (!(test_1 and test_2)){
            return false;
        }
    }
    return true;
}

bool test_cells_in_sequence_alignment2() {
    char* sequence_A = "GACTTACTA";
    char* sequence_B = "CGTGAATTC";

    SequenceAlignment s_a2(sequence_A, sequence_B, 2000, 2000, 4, 3, 5, 2, 3, -5);
    std::vector<Block> blocks_thread2_phase7;
    s_a2.phase = 7;
    s_a2.cells(2, blocks_thread2_phase7);

    std::vector<Block> expected_blocks_thread2_phase7;
    Block block1, block2;
    block1.startX = 4;
    block1.startY = 26;
    block2.startX = 16;
    block2.startY = 6;
    expected_blocks_thread2_phase7.push_back(block1);
    expected_blocks_thread2_phase7.push_back(block2);


    for (int i = 0; i < blocks_thread2_phase7.size(); i++){
        std::cout << blocks_thread2_phase7[i].startX << " " << blocks_thread2_phase7[i].startY << std::endl;
        bool test_1 = (blocks_thread2_phase7[i].startX == expected_blocks_thread2_phase7[i].startX);
        bool test_2 = (blocks_thread2_phase7[i].startY == expected_blocks_thread2_phase7[i].startY);
        if (!(test_1 and test_2)){
            return false;
        }
    }
    return true;
}

int main(){

    // if (test_score_matrix()){
    //     cout << "Test 1 passed!" << endl;
    // }

    if (test_cells_in_sequence_alignment()){
        std::cout << "Test 2 passed!" << "\n";
    }

    if (test_cells_in_sequence_alignment2()){
        std::cout << "Test 3 passed!" << "\n";
    }

    return 0;
    }