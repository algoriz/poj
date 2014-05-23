#include <iostream>
#include <fstream>
#include <string>

#ifdef _DEBUG
#define RUNPOJ(NUM) {\
    int poj##NUM(std::istream&, std::ostream&);\
    const char* input = "input\\"#NUM".txt";\
    poj##NUM(std::ifstream(input), std::cout);\
}

#else
#define RUNPOJ(NUM) {\
    int poj##NUM(std::istream&, std::ostream&);\
    const char* input = "input\\"#NUM".txt";\
    const char* output = "output\\"#NUM".mine.txt";\
    const char* keys = "output\\"#NUM".txt";\
    poj##NUM(std::ifstream(input), std::ofstream(output, std::ios::trunc));\
    int ln = file_compare(keys, output);\
    if (ln == 0){\
        std::cout << "POJ #" #NUM " PASSED\n";\
    } else{\
        std::cout << "POJ #" #NUM " WRONG ANSWER (case line: " << ln << ")\n";\
    }\
}
#endif

int file_compare(const char* file1, const char* file2){
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    if (!f1.is_open() || !f2.is_open()){
        return 1;
    }

    std::string line1, line2;
    for (int ln = 1; !f1.eof() && !f2.eof(); ++ln){
        line1.clear();
        line2.clear();
        std::getline(f1, line1);
        std::getline(f2, line2);

        if (line1 != line2){
            return ln;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    RUNPOJ(3844);
    return 0;
}
