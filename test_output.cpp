#include "sequence_alignment_nonparallel.cpp"

//###################################################
// Example 1 of Sequence Alignment
//##################################################

SequenceAlignment_NonParallel sequence_alignment_1(){
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    int n = strlen(sequence_A);
    int m = strlen(sequence_B);
    int gap_penalty = 4;
    int match_score = 5;
    int mismatch_score = -3;
    AlignmentType at = AlignmentType::LOCAL;
    SequenceAlignment_NonParallel s_a(sequence_A, sequence_B, n, m, gap_penalty, match_score, mismatch_score, at);
    return s_a;
}

//###################################################
// Test functions of Non-Parallel Version
//##################################################

bool test_score_matrix(){
    SequenceAlignment_NonParallel s_a = sequence_alignment_1();
    s_a.compute_score_matrix();

    std::vector< std::vector<int> > H_exa = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 5, 1, 5, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 2, 1, 10, 6, 2, 0, 0, 5, 1},
        {0, 5, 1, 0, 0, 6, 7, 3, 0, 5, 1, 2},
        {0, 1, 2, 6, 2, 2, 3, 12, 8, 4, 2, 6},
        {0, 0, 0, 7, 3, 0, 0, 8, 17, 13, 9, 7},
        {0, 0, 0, 3, 4, 8, 5, 4, 13, 14, 18, 14},
        {0, 5, 1, 0, 0, 4, 5, 2, 9, 18, 14, 15}
    };

    for (int i = 0; i < s_a.lenA+1; i++){
        for (int j = 0; j < s_a.lenB+1; j++){
            if ((*s_a.H)[i][j] != H_exa[i][j]){
                return false;
            }
        }
    }
    
    return true;
}

bool test_find_indexes_max_score_cell(){
    SequenceAlignment_NonParallel s_a = sequence_alignment_1();
    s_a.compute_score_matrix();
    unsigned int k, l; //We initialize the coordinates
    s_a.find_indexes_max_score_cell(k, l);
    int k_exa = 6;
    int l_exa = 10;
    return (k == k_exa) && (l == l_exa); 
}

bool test_traceback(){
    SequenceAlignment_NonParallel s_a = sequence_alignment_1();
    s_a.compute_alignment();

    for (int i = 0; i < s_a.len_alignA; i++) {
        std::cout<<s_a.alignA[i];
    }
    std::cout<<std::endl;
    for (int i = 0; i < s_a.len_alignB; i++) {
        std::cout<<s_a.alignB[i];
    }
    return true;
}

//###################################################
// Test functions of Parallel Version
//##################################################

// bool test_cells_in_sequence_alignment(){
//     char* sequence_A = "GACTTACTA";
//     char* sequence_B = "CGTGAATTC";
//     int n = strlen(sequence_A);
//     int m = strlen(sequence_B);
//     int gap_penalty = 4;
//     int match_score = 5;
//     int mismatch_score = -3;
//     AlignmentType at = AlignmentType::LOCAL;
//     SequenceAlignmentParallel s_a(sequence_A, sequence_B, 9, 9, 3, 1, 1, 2, 3, -5);
//     std::vector<Block> blocks_thread1_phase7;
//     s_a.phase = 7;
//     s_a.cells(1, blocks_thread1_phase7, 7);

//     std::vector<Block> expected_blocks_thread1_phase7;
//     Block block1, block2, block3;
//     block1.startX = 1;
//     block1.startY = 7;
//     block2.startX = 4;
//     block2.startY = 4;
//     block3.startX = 7;
//     block3.startY = 1;
//     expected_blocks_thread1_phase7.push_back(block1);
//     expected_blocks_thread1_phase7.push_back(block2);
//     expected_blocks_thread1_phase7.push_back(block3);


//     for (int i = 0; i < blocks_thread1_phase7.size(); i++){
//         bool test_1 = (blocks_thread1_phase7[i].startX == expected_blocks_thread1_phase7[i].startX);
//         bool test_2 = (blocks_thread1_phase7[i].startY == expected_blocks_thread1_phase7[i].startY);
//         if (!(test_1 and test_2)){
//             return false;
//         }
//     }
//     return true;
// }

// bool test_cells_in_sequence_alignment2() {
//     char* sequence_A = "GACTTACTA";
//     char* sequence_B = "CGTGAATTC";

//     SequenceAlignment_NonParallel s_a2(sequence_A, sequence_B, 2000, 2000, 4, 3, 5, 2, 3, -5);
//     std::vector<Block> blocks_thread2_phase7;
//     s_a2.phase = 7;
//     s_a2.cells(2, blocks_thread2_phase7, 7);

//     std::vector<Block> expected_blocks_thread2_phase7;
//     Block block1, block2;
//     block1.startX = 4;
//     block1.startY = 26;
//     block2.startX = 16;
//     block2.startY = 6;
//     expected_blocks_thread2_phase7.push_back(block1);
//     expected_blocks_thread2_phase7.push_back(block2);


//     for (int i = 0; i < blocks_thread2_phase7.size(); i++){
//         std::cout << blocks_thread2_phase7[i].startX << " " << blocks_thread2_phase7[i].startY << std::endl;
//         bool test_1 = (blocks_thread2_phase7[i].startX == expected_blocks_thread2_phase7[i].startX);
//         bool test_2 = (blocks_thread2_phase7[i].startY == expected_blocks_thread2_phase7[i].startY);
//         if (!(test_1 and test_2)){
//             return false;
//         }
//     }
//     return true;
// }

// // bool test_parallel_sequence_alignment() {
// //     char* A = "GACTTACTA";
// //     char* B = "CGTGAATTC";

// //     SequenceAlignment sa(A, B, 9, 9, 3, 3, 3, 2, 3, -5, AlignmentType::GLOBAL);
// //     sa.compute_score_matrix();
// //     // for (int i = 0; i < 10; i++) {
// //     //     for (int j = 0; j < 10; j++) {
// //     //         std::cout << (*(sa.H))[i][j] << " ";
// //     //     }
// //     //     std::cout << std::endl;
// //     // }    
// // }

int main(){

    std::cout << "Tests - Non-Parallel Version" << "\n";

    if (test_score_matrix()){
        cout << "Test 1 passed!" << endl;
    }

    if (test_find_indexes_max_score_cell()){
        cout << "Test 2 passed!" << endl;
    }

    if (test_traceback()){
        cout << "Test 3 passed!" << endl;
    }

    std::cout << "Tests - Parallel Version" << "\n";

    // if (test_cells_in_sequence_alignment()){
    //     std::cout << "Test 2 passed!" << "\n";
    // }

    // if (test_cells_in_sequence_alignment2()){
    //     std::cout << "Test 3 passed!" << "\n";
    // }

    // test_parallel_sequence_alignment();

    return 0;
    }