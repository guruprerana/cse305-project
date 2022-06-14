#include "sequence_alignment.hpp"

SequenceAlignment_NonParallel::SequenceAlignment_NonParallel(
    char *A, char *B, 
    unsigned int lenA, unsigned int lenB,
    int gap_penalty, int match_score, int mismatch_score, 
    AlignmentType at) {

    this->A = A;
    this->B = B;
    this->lenA = lenA;
    this->lenB = lenB;
    this->gap_penalty = gap_penalty;
    this->match_score = match_score;
    this->mismatch_score = mismatch_score;
    this->at = at;

    //We instantiate a 2d vector with default values 0: 
    int default_value = 0;
    H = new std::vector<std::vector<int> >(lenA+1, std::vector<int>(lenB+1, default_value));
    traceback_matrix = new std::vector<std::vector<TracebackDirection> >(
        lenA+1, 
        std::vector<TracebackDirection>(lenB+1, TracebackDirection::INVALID)
    );

    if (at == AlignmentType::GLOBAL) {
        for (unsigned int i = 1; i < lenA + 1; ++i) {
            (*traceback_matrix)[i][0] = TracebackDirection::INSERTION;
        }
        for (unsigned int j = 1; j < lenB + 1; ++j) {
            (*traceback_matrix)[0][j] = TracebackDirection::DELETION;
        }
    }

    this->alignA = new char[lenA + lenB + 2];
    this->alignB = new char[lenA + lenB + 2];
    this->len_alignA = 0;
    this->len_alignB = 0;
}

SequenceAlignment_NonParallel::~SequenceAlignment_NonParallel() {
        delete H;
        delete traceback_matrix;
        delete alignA;
        delete alignB;
    }

void SequenceAlignment_NonParallel::compute_score_matrix() {
    //We compute the score Matrix
    for (int i = 1; i < lenA+1; i++){
        for (int j = 1; j < lenB+1; j++){
            compute_score_cell(i, j);
        }
    }
}
