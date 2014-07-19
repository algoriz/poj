#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

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
    clock_t t = clock();\
    poj##NUM(std::ifstream(input), std::ofstream(output, std::ios::trunc));\
    t = clock() - t;\
    t *= 1000;\
    t /= CLOCKS_PER_SEC;\
    int ln = file_compare(keys, output);\
    if (ln == 0){\
        std::cout << "POJ #" #NUM " PASSED, running time: " << t << "ms\n";\
    } else{\
        std::cout << "POJ #" #NUM " WRONG ANSWER (case line: " << ln << "), running time: " << t << "ms\n";\
    }\
}
#endif

#define RUNPOJ_FILE(NUM, input, output, keys) {\
    int poj##NUM(std::istream&, std::ostream&);\
    clock_t t = clock();\
    poj##NUM(std::ifstream(input), std::ofstream(output, std::ios::trunc));\
    t = clock() - t;\
    t *= 1000;\
    t /= CLOCKS_PER_SEC;\
    int ln = file_compare(keys, output);\
    if (ln == 0){\
        std::cout << "POJ #" #NUM " PASSED, running time: " << t << "ms\n";\
    } else{\
        std::cout << "POJ #" #NUM " WRONG ANSWER (case line: " << ln << "), running time: " << t << "ms\n";\
    }\
}

#define RUN(NUM) do \
    {\
        if (argc == 4){\
            RUNPOJ_FILE(NUM, argv[1], argv[2], argv[3]);\
        }\
        else{\
            RUNPOJ(NUM);\
        }\
    } while (false)

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
    RUN(4001);
    return 0;
}
