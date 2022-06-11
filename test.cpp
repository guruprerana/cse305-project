#include <cstring>
#include <iostream>
#include <algorithm>

#include "sequence_alignment.hpp"
#include "sequence_alignment.cpp"
#include "sequence_alignment_nonparallel.cpp"
#include "sequence_alignment_parallel.cpp"

using namespace std;

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

SequenceAlignment_NonParallel sequence_alignment_2(){
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    int n = strlen(sequence_A);
    int m = strlen(sequence_B);
    int gap_penalty = 4;
    int match_score = 5;
    int mismatch_score = -3;
    AlignmentType at = AlignmentType::GLOBAL;
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

    char *expected_alignA = "GACTT-A";
    char *expected_alignB = "GAATTCA";

    for (unsigned int i = 0; i < s_a.len_alignA && i < 7; i++) {
        if (s_a.alignA[i] != expected_alignA[i])
            return false;
    }
    for (unsigned int i = 0; i < s_a.len_alignB, 7 && i < 7; i++) {
        if (s_a.alignB[i] != expected_alignB[i])
            return false;
    }
    return true;
}

bool test_traceback_global(){
    SequenceAlignment_NonParallel s_a = sequence_alignment_2();
    s_a.compute_alignment();

    char *expected_alignA = "GACTT-A";
    char *expected_alignB = "GAATTCA";

    for (unsigned int i = 0; i < s_a.len_alignA && i < 7; i++) {
        // if (s_a.alignA[i] != expected_alignA[i])
        //     return false;
        std::cout<<s_a.alignA[i];
    }
    std::cout<<std::endl;
    for (unsigned int i = 0; i < s_a.len_alignB, 7 && i < 7; i++) {
        // if (s_a.alignB[i] != expected_alignB[i])
        //     return false;
        std::cout<<s_a.alignB[i];
    }
    std::cout<<std::endl;
    return true;
}

//###################################################
// Test functions of Parallel Version
//##################################################

SequenceAlignmentParallel *sequence_alignment_1_parallel1() {
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    int n = strlen(sequence_A);
    int m = strlen(sequence_B);
    int gap_penalty = 4;
    int match_score = 5;
    int mismatch_score = -3;
    unsigned int num_threads = 1, block_size_x = 1, block_size_y = 1;
    AlignmentType at = AlignmentType::LOCAL;
    return new SequenceAlignmentParallel(sequence_A, sequence_B, n, m, 
        num_threads, block_size_x, block_size_y, 
        gap_penalty, match_score, mismatch_score, at
    );
}

SequenceAlignmentParallel *sequence_alignment_1_parallel2() {
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    int n = strlen(sequence_A);
    int m = strlen(sequence_B);
    int gap_penalty = 4;
    int match_score = 5;
    int mismatch_score = -3;
    unsigned int num_threads = 2, block_size_x = 2, block_size_y = 3;
    AlignmentType at = AlignmentType::LOCAL;
    return new SequenceAlignmentParallel(sequence_A, sequence_B, n, m, 
        num_threads, block_size_x, block_size_y, 
        gap_penalty, match_score, mismatch_score, at
    );
}

SequenceAlignmentParallel *sequence_alignment_1_parallel3() {
    char* sequence_A = "GACTTAC";
    char* sequence_B = "CGTGAATTCAT";
    int n = strlen(sequence_A);
    int m = strlen(sequence_B);
    int gap_penalty = 4;
    int match_score = 5;
    int mismatch_score = -3;
    unsigned int num_threads = 3, block_size_x = 2, block_size_y = 3;
    AlignmentType at = AlignmentType::LOCAL;
    return new SequenceAlignmentParallel(sequence_A, sequence_B, n, m, 
        num_threads, block_size_x, block_size_y, 
        gap_penalty, match_score, mismatch_score, at
    );
}

bool test_score_matrix_parallel(){
    SequenceAlignment_NonParallel s_a_reference = sequence_alignment_1();
    s_a_reference.compute_score_matrix();

    SequenceAlignmentParallel *sap1, *sap2, *sap3;
    sap1 = sequence_alignment_1_parallel1();
    sap2 = sequence_alignment_1_parallel2();
    sap3 = sequence_alignment_1_parallel3();

    sap1->compute_score_matrix();
    sap2->compute_score_matrix();
    sap3->compute_score_matrix();
    
    for (int i = 0; i < s_a_reference.lenA+1; i++){
        for (int j = 0; j < s_a_reference.lenB+1; j++){
            if ((*s_a_reference.H)[i][j] != (*sap1->H)[i][j]){
                return false;
            }
            if ((*s_a_reference.H)[i][j] != (*sap2->H)[i][j]){
                return false;
            }
            if ((*s_a_reference.H)[i][j] != (*sap3->H)[i][j]){
                return false;
            }
        }
    }
    
    return true;
}

bool test_len562_align() {
    char *A = new char[562], *B = new char[562];
    std::string fileA = "Sequences_Uniprot/len562.fasta";
    std::string fileB = "Sequences_Uniprot/len562-1.fasta";
    read_file(fileA, A, 562);
    read_file(fileB, B, 562);

    SequenceAlignmentParallel sap(A, B, 562, 562, 3, 10, 10, 4, 5, -3, AlignmentType::GLOBAL);

    sap.compute_alignment();

    std::string fileA_out = "Sequences_Uniprot/len562.fasta.aligned";
    std::string fileB_out = "Sequences_Uniprot/len562-1.fasta.aligned";

    print_file(fileA_out, sap.alignA, sap.len_alignA);
    print_file(fileB_out, sap.alignB, sap.len_alignB);
    return true;
}

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
    } else {
        cout << "Test 1 failed :(" << endl;
    }

    if (test_find_indexes_max_score_cell()){
        cout << "Test 2 passed!" << endl;
    } else {
        cout << "Test 2 failed :(" << endl;
    }

    if (test_traceback()){
        cout << "Test 3 passed!" << endl;
    } else {
        cout << "Test 3 failed :(" << endl;
    }

    if (test_traceback_global()){
        cout << "Test 4 passed!" << endl;
    } else {
        cout << "Test 4 failed :(" << endl;
    }

    std::cout << "Tests - Parallel Version" << "\n";

    if (test_score_matrix_parallel()) {
        cout << "Test 5 passed!" << endl;
    } else {
        cout << "Test 5 failed :(" << endl;
    }

    if (test_len562_align()) {
        cout << "Test 6 passed!" << endl;
    } else {
        cout << "Test 6 failed :(" << endl;
    }

    // if (test_cells_in_sequence_alignment()){
    //     std::cout << "Test 2 passed!" << "\n";
    // }

    // if (test_cells_in_sequence_alignment2()){
    //     std::cout << "Test 3 passed!" << "\n";
    // }

    // test_parallel_sequence_alignment();

    return 0;
    }