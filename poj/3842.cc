/* See the problem description:
 * http://poj.org/problem?id=3842
 */


#include <iostream>
#include <cmath>
#include <string>
#include <set>
#include <vector>

using namespace std;

bool isprime(int number){
    if (number < 2){
        return false;
    }

    for (int q = 2; q*q <= number; ++q){
        if (number % q == 0){
            return false;
        }
    }
    return true;
}

class arrange {
public:
    arrange(){
        elements.reserve(8);
    }

    size_t visit(const string& el){
        primes.clear();
        int add = 0;
        if (el.find('2') != string::npos){
            ++add;
        }
        if (el.find('5') != string::npos){
            ++add;
        }
        elements = el;
        string buf;
        buf.reserve(8);
        recursive_visit(buf, elements);
        return primes.size() + add;
    }

private:
    void recursive_visit(string& l, string& r){
        if (r.length() == 1){
            l.append(1, r[0]);
            access(l);
            l.erase(l.length() - 1, 1);
        }
        else{
            int rsize = r.size();
            for (int i = 0; i < rsize; ++i){
                char ch = r[i];
                l.append(1, ch);          // push ch
                r.erase(i, 1);
                access(l);
                recursive_visit(l, r);
                r.insert(i, 1, ch);
                l.erase(l.length() - 1, 1); // pop ch
            }
        }
    }

    void access(const string& value){
        switch (value[value.length()-1]){
        case '1':
        case '3':
        case '7':
        case '9':
            int n = atoi(value.c_str());
            if (isprime(n)){
                primes.insert(n);
            }
            break;
        }
    }

private:
    string elements;
    set<int> primes;
};

int poj3842(std::istream& input, std::ostream& output){
    string line;
    getline(input, line);
    int cases = atoi(line.c_str());

    arrange ar;
    while (cases--){
        getline(input, line);
        output << ar.visit(line) << '\n';
    }

    return 0;
}
