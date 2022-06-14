#include "sequence_alignment.hpp"

SequenceAlignmentParallel::SequenceAlignmentParallel(
    char *A, char *B, 
    unsigned int lenA, unsigned int lenB,
    unsigned int num_threads, unsigned int block_size_x, unsigned int block_size_y,
    int gap_penalty, int match_score, int mismatch_score, AlignmentType at) {
    
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

    if (at == AlignmentType::GLOBAL) {
        for (unsigned int i = 1; i < lenA + 1; ++i) {
            (*traceback_matrix)[i][0] = TracebackDirection::INSERTION;
        }
        for (unsigned int j = 1; j < lenB + 1; ++j) {
            (*traceback_matrix)[0][j] = TracebackDirection::DELETION;
        }
    }

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

    this->alignA = new char[lenA + lenB + 2];
    this->alignB = new char[lenA + lenB + 2];
    this->len_alignA = 0;
    this->len_alignB = 0;
}

SequenceAlignmentParallel::~SequenceAlignmentParallel() {
    delete H;
    delete traceback_matrix;
    delete mutexes;
    delete alignA;
    delete alignB;
    //delete cvs;
}

void SequenceAlignmentParallel::compute_score_matrix() {
    for (unsigned int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(&SequenceAlignmentParallel::processor_compute, this, i+1));
    }

    for (unsigned int i = 0; i < num_threads; ++i) {
        if (threads[i].joinable())
            threads[i].join();
    }
}

void SequenceAlignmentParallel::processor_compute(unsigned int processor_id) {
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
        unsigned int fetched = num_threads_finished.fetch_add(1);
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
        while (phase.load() != local_phase)
            cvs[processor_id - 1].wait(lk);
    }
}

void SequenceAlignmentParallel::cells(unsigned int processor_id, std::vector<Block> &blocks, unsigned int phase) {
    //This function returns 
    //Careful : to account for the first line of zeros and first columns of 
    //Zeros
    if (processor_id > num_threads){
        std::cout << "Invalid : processor_id is greater than the number of threads.\n";
        return;
    }

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
