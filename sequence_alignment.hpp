#ifndef SEQUENCE_ALIGNMENT_H
#define SEQUENCE_ALIGNMENT_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

#include "utils.cpp"

enum TracebackDirection {
    MATCH,
    INSERTION,
    DELETION,
    INVALID
};

enum AlignmentType {
    LOCAL,
    GLOBAL
};

struct Block {
    unsigned int startX, startY;
};

//######################################################################################
// Parent Class : Sequence alignment
//######################################################################################

class SequenceAlignment {
public:
    char *A;
    char *B;
    unsigned int lenA, lenB;
    int gap_penalty, match_score, mismatch_score;

    char *alignA;
    char *alignB;
    unsigned int len_alignA, len_alignB;
    AlignmentType at;

    std::vector<std::vector<int> > *H;
    std::vector<std::vector<TracebackDirection> > *traceback_matrix;

    virtual void compute_score_matrix() {};

    int compute_match_score(char a_i, char b_j);
    void find_indexes_max_score_cell(unsigned int &k, unsigned int &l);
    void compute_score_cell(unsigned int i, unsigned int j);
    void traceback(unsigned int i, unsigned int j);
    void compute_alignment();
};

//######################################################################################
// Sub-class : Non-parallel version of sequence alignment
//######################################################################################

class SequenceAlignment_NonParallel : public SequenceAlignment {
public:

    SequenceAlignment_NonParallel(
        char *A, char *B, 
        unsigned int lenA, unsigned int lenB,
        int gap_penalty, int match_score, int mismatch_score, AlignmentType at = AlignmentType::LOCAL
    );
    ~SequenceAlignment_NonParallel();
    void compute_score_matrix();
};

//######################################################################################
// Sub-class : Parallelized version of sequence alignment
//######################################################################################

class SequenceAlignmentParallel : public SequenceAlignment {
public:
    unsigned int num_threads, block_size_x, block_size_y;
    unsigned int n_blocks_A, n_blocks_B;

    std::vector<std::thread> threads;
    std::mutex *mutexes;
    std::condition_variable *cvs;

    std::atomic_int num_threads_finished;
    std::atomic_int phase;
    unsigned int num_phases;

    SequenceAlignmentParallel(
        char *A, char *B, 
        unsigned int lenA, unsigned int lenB,
        unsigned int num_threads, unsigned int block_size_x, unsigned int block_size_y,
        int gap_penalty, int match_score, int mismatch_score, AlignmentType at = AlignmentType::LOCAL
    );

    ~SequenceAlignmentParallel();

    void compute_score_matrix();
    void processor_compute(unsigned int processor_id);
    void cells(unsigned int processor_id, std::vector<Block> &blocks, unsigned int phase);
};

#endif
