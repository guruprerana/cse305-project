#include <string>
#include <iostream>
#include <fstream>

#include "sequence_alignment.hpp"
#include "sequence_alignment.cpp"
#include "sequence_alignment_nonparallel.cpp"
#include "sequence_alignment_parallel.cpp"

int main(int argc, char *argv[]) {
    std::string fileA, fileB, alignment_type, fileA_align, fileB_align;
    unsigned int num_threads, block_size_x, block_size_y, lenA, lenB;
    int gap_penalty, match_score, mismatch_score;

    std::cout<<"Welcome! Sequence alignment program initiation..."<<std::endl;
    std::cout<<"Enter the filename of the first sequence to align: ";
    std::cin>>fileA;

    std::cout<<"Enter the length of the first sequence: ";
    std::cin>>lenA;

    std::cout<<"Enter the filename of the second sequence to align: ";
    std::cin>>fileB;

    std::cout<<"Enter the length of the second sequence: ";
    std::cin>>lenB;

    std::cout<<"Gap penalty: ";
    std::cin>>gap_penalty;

    std::cout<<"Match score: ";
    std::cin>>match_score;

    std::cout<<"Mismatch score: ";
    std::cin>>mismatch_score;

    std::cout<<"Alignment type (local or global): ";
    std::cin>>alignment_type;

    std::cout<<"Number of threads: ";
    std::cin>>num_threads;

    std::cout<<"Block size along the first sequence: ";
    std::cin>>block_size_x;

    std::cout<<"Block size along the second sequence: ";
    std::cin>>block_size_y;

    char *A = new char[lenA];
    char *B = new char[lenB];

    read_file(fileA, A, lenA);
    read_file(fileB, B, lenB);

    AlignmentType at;
    if (alignment_type.compare("local") == 0)
        at = AlignmentType::LOCAL;
    else
        at = AlignmentType::GLOBAL;

    SequenceAlignment *sa;
    if (num_threads <= 1) {
        sa = new SequenceAlignment_NonParallel(A, B, lenA, lenB,
            gap_penalty, match_score, mismatch_score, at
        );
    } else {
        sa = new SequenceAlignmentParallel(A, B, lenA, lenB,
            num_threads, block_size_x, block_size_y,
            gap_penalty, match_score, mismatch_score, at
        );
    }

    sa->compute_alignment();
    std::cout<<"Sequences aligned!"<<std::endl;

    std::cout<<"Enter the filename to store the aligned first sequence: ";
    std::cin>>fileA_align;

    std::cout<<"Enter the filename to store the aligned second sequence: ";
    std::cin>>fileB_align;

    print_file(fileA_align, sa->alignA, sa->len_alignA);
    print_file(fileB_align, sa->alignB, sa->len_alignB);

    delete A;
    delete B;
    delete sa;
    return 0;
}
