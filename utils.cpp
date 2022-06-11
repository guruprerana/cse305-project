#include <iostream>
#include <vector>
#include <limits>

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

void print_2d_vector(std::vector< std::vector<int> > H){
    for (int i = 0; i < H.size(); i++){
        for (int j = 0; j < H[i].size(); j++){
        std::cout << H[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout<< std::endl;
}

void reverse_array(char *X, unsigned int lenX) {
    for (unsigned int i = 0, j = lenX - 1; i < lenX/2; i++, j--) {     
        char temp = X[i];  
        X[i] = X[j];  
        X[j] = temp;  
    }
}
