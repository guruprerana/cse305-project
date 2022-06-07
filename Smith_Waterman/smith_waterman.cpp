#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <limits>
using namespace std;

#define G   4 //penalty associated to an indel (insertion or deletion)

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

int match_score(char a_i, char b_j){
    if (a_i == b_j){
        return 5;
    }
    else {
        return -3;
    }
}

void score_matrix(
    char *sub_A, 
    char* sub_B,
    std::vector<std::vector<int>> *H,
    std::vector<std::vector<TracebackDirection>> *traceback_matrix, 
    AlignmentType at = AlignmentType::LOCAL
) {
    //We compute the score Matrix
    const int n = strlen(sub_A);
    const int m = strlen(sub_B);

    //We instantiate a 2d vector with default values 0: 
    int default_value = 0;
    H = new std::vector<std::vector<int>>(n+1, std::vector<int>(m+1, default_value));
    traceback_matrix = new std::vector<std::vector<TracebackDirection>>(n+1, std::vector<TracebackDirection>(m+1, TracebackDirection::INVALID));

    for (int i = 1; i < n+1; i++){
        for (int j = 1; j < m+1; j++){
            int match_mismatch = (*H)[i-1][j-1] + match_score(sub_A[i-1], sub_B[j-1]);
            int deletion = (*H)[i-1][j] - G;
            int insertion = (*H)[i][j-1] - G;

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

// int main(){

//     char* sequence_A = "GACTTAC";
//     char* sequence_B = "CGTGAATTCAT";

//     int n = strlen(sequence_A);
//     int m = strlen(sequence_B);

//     std::cout << sequence_A << "\n";
//     std::vector< std::vector<int> > H = score_matrix(sequence_A, sequence_B);

//     // std::vector< std::vector<int> > H_exact = 
//     for (int i = 0; i < H.size(); i++){
//         for (int j = 0; j < H[i].size(); j++){
//         cout << H[i][j] << " ";
//         }
//         cout << std::endl;
//     }

//     int k, l;
//     find_indexes_max_cell(H, k,l);
//     cout << k << l;

//     return 0;
//     }

// int main(){

//     char* sequence_A = "AB";
//     char* sequence_B = "BA";

//     int n = strlen(sequence_A);
//     int m = strlen(sequence_B);
//     std::cout << sequence_A << "\n";

//     }
