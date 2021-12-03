
//https://www.fluentcpp.com/2021/03/05/stdindex_sequence-and-its-improvement-in-c20/
#include <iostream>
#include "CSVParser.h"
#include "CSVIterator.h"
int main() {
    std::ifstream file("input.csv");
    CSVParser<int,int,int,double> parser(file);
    for ( auto &g : parser ){
        std::cout<<g<<std::endl;
    }
    return 0;
}
