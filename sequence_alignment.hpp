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

    /**
     * Computes the score matrix H and and the traceback matrix T and updates the class attributes H and traceback_matrix.
     */
    virtual void compute_score_matrix() {};

    /**
     * Computes the match/mismatch score of a cell.
     *
     * @param a_i : ith residue of the sequence A.
     * @param b_j : jth residue of the sequence B.
     * @return int value evaluating match score.
     */
    int compute_match_score(char a_i, char b_j);

    /**
     * Computes the indexes of the score with the maximum score and store them in the referenced parameters.
     *
     * @param k referenced unsigned integer to store the row index of the max score cell.
     * @param l referenced unsigned integer to store the column index of the max score cell.
     */
    void find_indexes_max_score_cell(unsigned int &k, unsigned int &l);

    /**
     * Computes the score of a cell.
     *
     * @param i row index of the cell.
     * @param j column index of the cell.
     */
    void compute_score_cell(unsigned int i, unsigned int j);

    /**
     * Computes the optimal alignment from the traceback matrix and updates the alignA and alignB class attributes.
     *
     * @param i row index on where to start the traceback.
     * @param j column index on where to start the traceback.
     */
    void traceback(unsigned int i, unsigned int j);

    /**
     * Computes the optimal alignment of a SequenceAlignment Object.
     *
     */
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
    int num_phases;

    SequenceAlignmentParallel(
        char *A, char *B, 
        unsigned int lenA, unsigned int lenB,
        unsigned int num_threads, unsigned int block_size_x, unsigned int block_size_y,
        int gap_penalty, int match_score, int mismatch_score, AlignmentType at = AlignmentType::LOCAL
    );

    ~SequenceAlignmentParallel();

    void compute_score_matrix();

    /**
     * Computes all the cells for a given processor.
     * 
     * @param processor_id the processor id.
     */
    void processor_compute(unsigned int processor_id);

    /**
     * Computes the first cell of each block that will be computed 
     * by a processor for a given phase.
     *
     * @param processor_id the processor id.
     * @param blocks a referenced vector of blocks to return the computed cells inside.
     * @param phase the numbered phase.
     */
    void cells(unsigned int processor_id, std::vector<Block> &blocks, unsigned int phase);
};

#endif
