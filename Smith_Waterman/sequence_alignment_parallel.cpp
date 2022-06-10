#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <limits>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "sequence_alignment.cpp"

class SequenceAlignmentParallel {
public:
    char *A;
    char *B;
    unsigned int lenA, lenB;

    unsigned int num_threads, block_size_x, block_size_y;
    unsigned int n_blocks_A, n_blocks_B;

    int gap_penalty, match_score, mismatch_score;

    AlignmentType at;

    std::vector<std::vector<int> > *H;
    std::vector<std::vector<TracebackDirection> > *traceback_matrix;

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
        int gap_penalty, int match_score, int mismatch_score, AlignmentType at = AlignmentType::LOCAL) {
        
        this->A = A;
        this->B = B;
        this->lenA = lenA;
        this->lenB = lenB;
        this->num_threads = num_threads;
        mutexes = new std::mutex[num_threads];
        cvs = new std::condition_variable[num_threads];
        this->block_size_x = block_size_x;
        this->block_size_y = block_size_y;
        this->gap_penalty = gap_penalty;
        this->match_score = match_score;
        this->mismatch_score = mismatch_score;
        this->at = at;

        //We instantiate a 2d vector with default values 0: 
        int default_value = 0;
        H = new std::vector<std::vector<int>>(lenA+1, std::vector<int>(lenB+1, default_value));
        traceback_matrix = new std::vector<std::vector<TracebackDirection>>(
            lenA+1, 
            std::vector<TracebackDirection>(lenB+1, TracebackDirection::INVALID)
        );

        std::atomic_init(&num_threads_finished, 0);
        std::atomic_init(&phase, 1);

        if (lenA % block_size_x == 0)
            n_blocks_A = lenA / block_size_x;
        else
            n_blocks_A = (lenA / block_size_x) + 1;

        if (lenB % block_size_y == 0)
            n_blocks_B = lenB / block_size_y;
        else
            n_blocks_B = (lenB / block_size_y) + 1;

        num_phases = n_blocks_A + n_blocks_B - 1; // to be computed based on m, n, block_size
    }

    ~SequenceAlignmentParallel() {
        delete H;
        delete traceback_matrix;
        delete mutexes;
        //delete cvs;
    }

    int compute_match_score(char a_i, char b_j) {
        if (a_i == b_j) {
            return match_score;
        } else {
            return mismatch_score;
        }
    }

    void compute_score_matrix() {
        for (unsigned int i = 0; i < num_threads; ++i) {
            threads.push_back(std::thread(&SequenceAlignment::processor_compute, this, i+1));
        }

        for (unsigned int i = 0; i < num_threads; ++i) {
            if (threads[i].joinable())
                threads[i].join();
        }
    }

    void processor_compute(unsigned int processor_id) {
        unsigned int local_phase = 1;
        while (local_phase <= num_phases) {
            std::vector<Block> blocks_to_compute;
            cells(processor_id, blocks_to_compute, local_phase);

            for (auto b = blocks_to_compute.begin(); b != blocks_to_compute.end(); ++b) {
                Block block = *b;
                for (unsigned int i = block.startX; i < std::min(block.startX + block_size_x, lenA + 1); ++i) {
                    for (unsigned int j = block.startY; j < std::min(block.startY + block_size_y, lenB + 1); ++j) {
                        compute_score_cell(i, j);
                    }
                }
            }

            local_phase++;
            int fetched = num_threads_finished.fetch_add(1);
            if (fetched == num_threads - 1) {
                int current_val = fetched + 1;
                while (!num_threads_finished.compare_exchange_weak(current_val, 0));
                phase.fetch_add(1);
                
                for (unsigned int i = 0; i < num_threads; i++) {
                    cvs[i].notify_one();
                }
                continue;
            }
            std::unique_lock<std::mutex> lk(mutexes[processor_id - 1]);
            while (phase != local_phase)
                cvs[processor_id - 1].wait(lk);
        }
    }

    void compute_score_cell(unsigned int i, unsigned int j) {
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

    void cells(unsigned int processor_id, std::vector<Block> &blocks, unsigned int phase) {
        //This function returns 
        //Careful : to account for the first line of zeros and first columns of 
        //Zeros
        if (processor_id > num_threads){
            std::cout << "Invalid : processor_id is greater than the number of threads.\n";
            return;
        }
        // for (int i = (processor_id-1)*block_size_x; i < lenA + 1; i += (num_threads*block_size_x)){
        //     //std::cout << "i = " << i << "\n";
        //     int it = i/(num_threads*block_size_x);
        //     //std::cout << "it = " << it << "\n";
        //     int j = 1 + (phase.load() - 1 - it*num_threads) * block_size_y;
        //     //std::cout << "j = " << j << "\n";
        //     Block new_block;
        //     new_block.startX = i;
        //     new_block.startY = j;
        //     blocks.push_back(new_block);
        // }

        int i = 1 + (processor_id - 1) * block_size_x;
        int j = 1 + (phase - 1 - processor_id + 1) * block_size_y;

        while (i < lenA + 1 && j >= 1) {
            Block new_block;
            new_block.startX = i;
            new_block.startY = j;
            blocks.push_back(new_block);

            i += num_threads * block_size_x;
            j -= num_threads * block_size_y;
        }

        if (i < lenA + 1 && j >= 1) {
            Block new_block;
            new_block.startX = i;
            new_block.startY = j;
            blocks.push_back(new_block);
        }
    }

};
