#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "sequence_alignment.hpp"
#include "sequence_alignment.cpp"
#include "sequence_alignment_nonparallel.cpp"
#include "sequence_alignment_parallel.cpp"


void generate_threads_vs_size_data() {
    std::ofstream output;
    output.open("benchmarking/threads_vs_size.csv");

    char *A = new char[5083], *B = new char[4779];

    std::string fileA = "Sequences_Uniprot/len-5083.fasta";
    std::string fileB = "Sequences_Uniprot/len-4779.fasta";
    read_file(fileA, A, 5083);
    read_file(fileB, B, 4779);

    std::cout<<",";
    for (int size = 500; size < 2001; size += 500) {
        std::cout<<size<<",";
    }
    std::cout<<std::endl;

    for (int thread = 7; thread < 11; thread++) {
        std::cout<<thread<<",";
        for (int size = 500; size < 2001; size += 500) {
            SequenceAlignmentParallel sap(A, B, size, size, thread, 25, 25, 4, 5, -3);

            auto start = std::chrono::steady_clock::now();
            sap.compute_score_matrix();
            auto finish = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
            std::cout<<elapsed<<",";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    output.close();
}

void generate_size_vs_block_size_data() {
    std::ofstream output;
    output.open("benchmarking/threads_vs_size.csv");

    char *A = new char[5083], *B = new char[4779];

    std::string fileA = "Sequences_Uniprot/len-5083.fasta";
    std::string fileB = "Sequences_Uniprot/len-4779.fasta";
    read_file(fileA, A, 5083);
    read_file(fileB, B, 4779);

    std::cout<<",";
    for (int size = 500; size < 2001; size += 500) {
        std::cout<<size<<",";
    }
    std::cout<<std::endl;

    for (int block_size = 5; block_size < 11; block_size += 1) {
        std::cout<<block_size<<",";
        for (int size = 500; size < 2001; size += 500) {
            SequenceAlignmentParallel sap(A, B, size, size, 8, block_size, block_size, 4, 5, -3);

            auto start = std::chrono::steady_clock::now();
            sap.compute_score_matrix();
            auto finish = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
            std::cout<<elapsed<<",";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    output.close();
}

int main() {
    generate_size_vs_block_size_data();

    return 0;
}
