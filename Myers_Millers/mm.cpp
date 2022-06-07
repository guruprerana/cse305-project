
#include <iostream>
#include <vector>
 
#define g
#define h

double gap(int k){
     //cost of a k-symbol indel (insertion or deletion)
     return g + h * k;
}

double bonus(int i, int j){

}


void mm_method(double g){
     std::vector<double> CC, DD;
     double e,c,s,t;

     CC.push_back(0.);
     t = g;
     
}

void compute_cost_C(int i, int j){
//minimum cost of a conversion of A_i to B_j
}

void compute_cost_D(int i, int j){
//minimum cost of a conversion of A_i to B_j that deletes a_i
}

void compute_cost_I(int i, int j){
//minimum cost of a conversion of A_i to B_j that inserts b_j
}

int main() {

char *sequence_A = "";
char *sequence_B = "";

int n = strlen(sequence_A);
int m = strlen(sequence_B);

//We define the scores : 
int match;
int mismatch;
int deletion;

int CC[];
int DD[];

//We initialize the cost matrix M
for (int i = 0; i < n; i++){
     M[i][0] = i*deletion;
}
return 0;
}