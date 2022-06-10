#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <limits>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>


int max_vector(std::vector<int> ints) {
    if (ints.size() == 0) {
        return std::numeric_limits<int>::min();
    }

    int max = ints[0];
    for (auto it = ints.begin(); it != ints.end(); ++it) {
        if (*it > max) {
            max = *it;
        }
    }

    return max;
}

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

void print_2d_vector(std::vector< std::vector<int> > H){
    for (int i = 0; i < H.size(); i++){
        for (int j = 0; j < H[i].size(); j++){
        std::cout << H[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

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
    AlignmentType at;

    std::vector<std::vector<int> > *H;
    std::vector<std::vector<TracebackDirection> > *traceback_matrix;


    int compute_match_score(char a_i, char b_j){
        if (a_i == b_j){
            return match_score;
        }
        else {
            return mismatch_score;
        }
    }

    void find_indexes_max_cell(std::vector< std::vector<int> > H, int &k, int &l){
    //We find the LAST cell with the max score
    int maximum = 0;
    for(int i = 0; i < H.size(); i++){
        for (int j = 0; j < H[0].size(); j++){
            if (H[i][j] > maximum){
                maximum = H[i][j];
                k = i;
                l = j;
            }
        }
    }
}

    // void print_alignment(int i, int j){
    //     if ((*H)[i][j] == 0){
    //         return;
    //     }
    //     else{
    //         if ((*traceback_matrix)[i][j] == TracebackDirection::MATCH){
    //             (*alignA).push_back((*A)[i - 1]);
    //             (*alignB).push_back((*B)[j - 1]);
    //         }
    //         else if ((*traceback_matrix)[i][j] == TracebackDirection::DELETION){
    //             (*alignA).push_back('-');
    //             (*alignB).push_back((*B)[j - 1]);
    //         }
    //         else ((*traceback_matrix)[i][j] == TracebackDirection::INSERTION){
    //             (*alignA).push_back((*A)[i - 1]);
    //             (*alignB).push_back('-');
    //         }
    //         print_alignment(i-1, j-1, align_a, align_b);
    //     }
    // }

};