#include "sequence_alignment.hpp"

//######################################################################################
// Parent Class : Sequence alignment
//######################################################################################

int SequenceAlignment::compute_match_score(char a_i, char b_j) {
    if (a_i == b_j) {
        return match_score;
    } else {
        return mismatch_score;
    }
}

void SequenceAlignment::find_indexes_max_score_cell(unsigned int &k, unsigned int &l){
    //We find the LAST cell with the max score
    int maximum = 0;
    for (int i = 0; i < lenA + 1; i++) {
        for (int j = 0; j < lenB + 1; j++) {
            if ((*H)[i][j] > maximum){
                maximum = (*H)[i][j];
                k = i;
                l = j;
            }
        }
    }
}

void SequenceAlignment::compute_score_cell(unsigned int i, unsigned int j) {
    // add safety bounds on i and j

    int match_mismatch = (*H)[i-1][j-1] + compute_match_score(A[i-1], B[j-1]);
    int insertion = (*H)[i-1][j] - gap_penalty;
    int deletion = (*H)[i][j-1] - gap_penalty;

    int score_ij;
    TracebackDirection dir;

    if (at == AlignmentType::LOCAL) {
        std::vector<int> max_v;
        max_v.push_back(0);
        max_v.push_back(match_mismatch);
        max_v.push_back(deletion);
        max_v.push_back(insertion);
        score_ij = max_vector(max_v);
    } else {
        std::vector<int> max_v;
        max_v.push_back(match_mismatch);
        max_v.push_back(deletion);
        max_v.push_back(insertion);
        score_ij = max_vector(max_v);
    }

    if (score_ij == 0 && at == AlignmentType::LOCAL) {
        dir = TracebackDirection::INVALID;
    } else if (score_ij == match_mismatch) {
        dir = TracebackDirection::MATCH;
    } else if (score_ij == deletion) {
        dir = TracebackDirection::DELETION;
    } else if (score_ij == insertion) {
        dir = TracebackDirection::INSERTION;
    } else {
        dir = TracebackDirection::INVALID;
    }

    (*H)[i][j] = score_ij;
    (*traceback_matrix)[i][j] = dir;
}

void SequenceAlignment::traceback(unsigned int i, unsigned int j){
    while (true) {
        if (at == AlignmentType::LOCAL && (*H)[i][j] == 0)
            break;
        // else if (at == AlignmentType::GLOBAL && (i == 0 || j == 0))
        //     break;

        if ((*traceback_matrix)[i][j] == TracebackDirection::MATCH){
            alignA[len_alignA] = A[i-1];
            alignB[len_alignB] = B[j-1];
            i--;
            j--;
        }
        else if ((*traceback_matrix)[i][j] == TracebackDirection::DELETION){
            alignA[len_alignA] = '-';
            alignB[len_alignB] = B[j-1];
            j--;
        }
        else if ((*traceback_matrix)[i][j] == TracebackDirection::INSERTION) {
            alignA[len_alignA] = A[i-1];
            alignB[len_alignB] = '-';
            i--;
        } else {
            break;
        }

        len_alignA++;
        len_alignB++;
    }
    reverse_array(alignA, len_alignA);
    reverse_array(alignB, len_alignB);
}

void SequenceAlignment::compute_alignment() {
    compute_score_matrix();
    unsigned int alignA_start, alignB_start;
    if (at == AlignmentType::LOCAL) {
        find_indexes_max_score_cell(alignA_start, alignB_start);
    } else {
        alignA_start = lenA;
        alignB_start = lenB;
    }
    traceback(alignA_start, alignB_start);
}
