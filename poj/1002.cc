/* See the problem description:
 * http://poj.org/problem?id=1002
 */
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string.h>
using namespace std;

struct counter_t
{
    counter_t() : count(0){}

    counter_t& operator++() {
        ++count;
        return *this;
    }

    int count;
};

int to_number(char* g){
    int number = 0;
    for (; *g; ++g){
        switch (*g){
        case '0':
            number *= 10;
            break;
        case '1':
            number *= 10;
            number += 1;
            break;
        case '2':
        case 'A':
        case 'B':
        case 'C':
            number *= 10;
            number += 2;
            break;
        case '3':
        case 'D':
        case 'E':
        case 'F':
            number *= 10;
            number += 3;
            break;
        case '4':
        case 'G':
        case 'H':
        case 'I':
            number *= 10;
            number += 4;
            break;
        case '5':
        case 'J':
        case 'K':
        case 'L':
            number *= 10;
            number += 5;
            break;
        case '6':
        case 'M':
        case 'N':
        case 'O':
            number *= 10;
            number += 6;
            break;
        case '7':
        case 'P':
        case 'R':
        case 'S':
            number *= 10;
            number += 7;
            break;
        case '8':
        case 'T':
        case 'U':
        case 'V':
            number *= 10;
            number += 8;
            break;
        case '9':
        case 'W':
        case 'X':
        case 'Y':
            number *= 10;
            number += 9;
            break;

        case '-':
        case '\r':
        case '\n':
            break;

        default:
            *g = 0; // error
            break;
        }
    }
    return number;
}

int poj1002(std::istream& input, std::ostream& output){
    map<int, counter_t> number_count;
    
    int lines;
    input >> lines;

    for (int i = 0; i < lines; ++i) {
        char line[64] = { 0 };
        input.getline(line, 62);
        if (line[6] == 0){
            --i;
            continue; // blank lines
        }

        ++number_count[to_number(line)];
    }

    bool no_duplicates = true;
    map<int, counter_t>::iterator it_end = number_count.end();
    for (map<int, counter_t>::iterator it = number_count.begin(); it != it_end; ++it) {
        char buf[16] = { 0 };
        char buf2[16] = "0000000";
        int count = it->second.count;
        if (count > 1) {
            no_duplicates = false;

            char* num = buf;
            _itoa(it->first, buf, 10);

            // For numbers start with zero's, string-int-string conversion loses
            // the heading zero's, thus needs a fix.
            if (buf[6] == 0) {
                int length = strlen(buf);
                memcpy(buf2 + 7 - length, buf, length);
                num = buf2;
            }

            output.write(num, 3);
            output.write("-", 1);
            output.write(num + 3, 4);
            output << ' ' << count << '\n';
        }
    }

    if (no_duplicates) {
        output << "No duplicates.\n";
    }
    return 0;
}
