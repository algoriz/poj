#include <iostream>
#include <fstream>

#define RUNPOJ(NUM) \
{\
    int poj##NUM(std::istream&, std::ostream&);\
    std::cout << "------ POJ #" #NUM " ------\n"; \
    poj##NUM(std::ifstream("input\\"#NUM".txt"), std::cout);\
    std::cout << "\n\n"; \
}

int main(int argc, char** argv)
{
    RUNPOJ(1093);
    return 0;
}
