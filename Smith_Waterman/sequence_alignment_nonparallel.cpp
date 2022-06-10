#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <limits>
#include "sequence_alignment.cpp"

using namespace std;

#define G   4 //penalty associated to an indel (insertion or deletion)


class SequenceAlignment_NonParallel : public SequenceAlignment {
public:

SequenceAlignment_NonParallel(
        char *A, char *B, 
        unsigned int lenA, unsigned int lenB,
        int gap_penalty, int match_score, int mismatch_score, AlignmentType at = AlignmentType::LOCAL) {

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
        }

void compute_score_matrix() {
    //We compute the score Matrix
    for (int i = 1; i < lenA+1; i++){
        for (int j = 1; j < lenB+1; j++){
            int match_mismatch = (*H)[i-1][j-1] + compute_match_score(A[i-1], B[j-1]);
            int deletion = (*H)[i-1][j] - this->gap_penalty;
            int insertion = (*H)[i][j-1] - this->gap_penalty;

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
    }
}
};


// int main(){

//     char* sequence_A = "GACTTAC";
//     char* sequence_B = "CGTGAATTCAT";
//     int n = strlen(sequence_A);
//     int m = strlen(sequence_B);

//     SequenceAlignment_NonParallel s_a(sequence_A, sequence_B, n, m, 4, 5, -3);
    

//     std::cout << sequence_A << "\n";

//     s_a.compute_score_matrix();

//     // std::vector< std::vector<int> > H_exact = 
//     for (int i = 0; i < (*s_a.H).size(); i++){
//         for (int j = 0; j < s_a.H[i].size(); j++){
//         std::cout << (*s_a.H)[i][j] << " ";
//         }
//         cout << std::endl;
//     }

//     return 0;
//     }



// int find_best_cell(std::vector< std::vector<int> > H, int &best_i, int &best_j, int current_i, int current_j){
//     //We find the best cell : we return 0 if it is the diagonal one, 1 if it is the left one and 2 if it is the top one.
//     //We also store the indices best_i, best_j in the referenced arguments
//     int score_diagonal = H[current_i-1][current_j-1];
//     int score_left = H[current_i][current_j-1];
//     int score_top = H[current_i-1][current_j];

//     int best_score = score_diagonal;
//     best_i = current_i-1;
//     best_j = current_j-1;

//     if (score_left > best_score){
//             best_i = current_i;
//             best_j = current_j-1;
//             best_score = score_left;
//             return 1;
//     }

//     if (score_top > best_score){
//             best_i = current_i-1;
//             best_j = current_j;
//             best_score = score_top;
//             return 2;
//     }

//     return 0;
// }

// void compute_optimal_alignment(std::vector< std::vector<int> > H, int i, int j, char align1 = "", char align2 = ""){
//     if (H[i][j] == 0){
//         return ;
//     }
//     int best_i, best_j;
//     int best_position = find_best_cell(H, best_i, best_j, i, j);
//     //Diagonal
//     if (best_position == 0){
//         align1.pushback(sequence_A[]);
//         align2.pushback(sequence_B[]);
//     }
//     //Left
//     if (best_position == 1){
//         align1.pushback(sequence_A[]);
//         align2.pushback(sequence_B[]);
//     }
//     //Top
//     if (best_position == 2){
//         align1.pushback(sequence_A[]);
//         align2.pushback(sequence_B[]);
//     }
//     compute_optimal_alignment(H, best_i, best_j, align1, align2); 

// }

// char* smith_waterman(char *sequence_A, char *sequence_B){
//     std::vector< std::vector<int> > H = score_matrix(sequence_A, sequence_B);
//     //We compute the indexes of the LAST max cell in H 
//     int k, l;
//     find_indexes_max_cell(H, k, l);
//     //We backtrace
//     char* alignment = compute_optimal_alignment(H);
//     return alignment;
// }