#include <iostream>
#include <vector>
#include <limits>
#include <fstream>

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
    for (std::vector<std::vector<int>>::size_type i = 0; i < H.size(); i++){
        for (std::vector<std::vector<int>>::size_type j = 0; j < H[i].size(); j++){
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

void read_file(std::string &filename, char *arr, unsigned int len) {
    std::ifstream stream;
    stream.open(filename);

    for (unsigned int i = 0; i < len; ++i) {
        if (stream.eof())
            break;
        arr[i] = stream.get();
    }

    stream.close();
}

void print_file(std::string &filename, char *arr, unsigned int len) {
    std::ofstream stream;
    stream.open(filename);

    for (unsigned int i = 0; i < len; ++i) {
        stream<<arr[i];
    }

    stream.close();
}
