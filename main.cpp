#include <iostream>
#include <vector>
#include "CSVIterator.h"
int main() {
    std::ifstream file("inpsut.csv");
    CSVParser<int,int,int,double> parser(file);
    for ( auto &g : parser ){
        std::cout<<g<<std::endl;
    }
    return 0;
}
