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

    for (unsigned int i = 0; i < s_a.lenA+1; i++){
        for (unsigned int j = 0; j < s_a.lenB+1; j++){
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
    unsigned int k_exa = 6;
    unsigned int l_exa = 10;
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
    for (unsigned int i = 0; i < s_a.len_alignB && i < 7; i++) {
        if (s_a.alignB[i] != expected_alignB[i])
            return false;
    }
    return true;
}

bool test_traceback_global(){
    SequenceAlignment_NonParallel s_a = sequence_alignment_2();
    s_a.compute_alignment();

    char *expected_alignA = "---GACTT-AC";
    char *expected_alignB = "CGTGAATTCAT";

    for (unsigned int i = 0; i < s_a.len_alignA && i < 11; i++) {
        if (s_a.alignA[i] != expected_alignA[i])
            return false;
    }
    for (unsigned int i = 0; i < s_a.len_alignB && i < 11; i++) {
        if (s_a.alignB[i] != expected_alignB[i])
            return false;
    }

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

bool test_score_matrix_parallel() {
    SequenceAlignment_NonParallel s_a_reference = sequence_alignment_1();
    s_a_reference.compute_score_matrix();

    SequenceAlignmentParallel *sap1, *sap2, *sap3;
    sap1 = sequence_alignment_1_parallel1();
    sap2 = sequence_alignment_1_parallel2();
    sap3 = sequence_alignment_1_parallel3();

    sap1->compute_score_matrix();
    sap2->compute_score_matrix();
    sap3->compute_score_matrix();
    
    for (unsigned int i = 0; i < s_a_reference.lenA+1; i++){
        for (unsigned int j = 0; j < s_a_reference.lenB+1; j++){
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

int main() {

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

    return 0;
}