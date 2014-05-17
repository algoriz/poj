/* See the problem description:
 * http://poj.org/problem?id=1060
 */
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class polynomial{
public:
    polynomial() : _len(0){ _onz[0] = -1; }
    
    polynomial(const polynomial& another) : _len(another._len){
        memcpy(_onz, another._onz, 4 + _len*sizeof(int));
    }

    polynomial& operator= (const polynomial& another){
        _len = another._len;
        memcpy(_onz, another._onz, 4 + _len*sizeof(int));
        return *this;
    }

    int degree() const { return _onz[0]; }

    void scan(const std::string& polystr){
        const char* g = polystr.c_str();
        skipspaces(g);
        int d = atoi(g) - 1;
        
        _len = 0;
        nextword(g);
        for (int e = d; e >= 0; --e){
            if (*g == '1'){
                push_back(e);
            }
            g += 2;
        }
        _onz[_len] = -1;
    }

    void print(std::string& polystr) const {
        polystr.clear();
        if (_len <= 0){
            return; /* empty polynomial */
        }

        int d = degree();
        char buf[16];
        polystr.append(_itoa(d+1, buf, 10));

        int e = d;
        for (int i = 0; e >= 0 && i < _len; --e){
            if (_onz[i] == e){
                polystr.append(" 1", 2);
                ++i;
            }
            else{
                polystr.append(" 0", 2);
            }
        }

        while (e >= 0){
            polystr.append(" 0", 2);
            --e;
        }
    }

    /* left shift */
    polynomial& operator<<= (int n){
        if (n >= 0){
            for (int i = 0; i < _len; ++i){
                _onz[i] += n;
            }
        }
        return *this;
    }

    polynomial& operator+= (const polynomial& r){
        if (this == &r){
            _len = 0;
            return *this;
        }
        if (r._len == 0){
            return *this;
        }
        if (_len == 0){
            *this = r;
            return *this;
        }

        /* Merge l and r into *this */
        const polynomial l = *this;
        _len = 0;

        const int* le = l._onz;
        const int* le_end = le + l._len;
        const int* re = r._onz;
        const int* re_end = re + r._len;
        while (le < le_end && re < re_end)
        {
            if (*le > *re){
                push_back(*le);
                ++le;
            }
            else if (*le < *re){
                push_back(*re);
                ++re;
            }
            else {
                ++le;
                ++re;
            }
        }

        for (; le < le_end; ++le){
            push_back(*le);
        }
        for (; re < re_end; ++re){
            push_back(*re);
        }
        _onz[_len] = -1;
        return *this;
    }

    polynomial& operator-= (const polynomial& r){
        return operator+=(r);
    }

    polynomial& operator*= (const polynomial& r){
        const polynomial* pa;
        const polynomial* pb;
        if (_len > r._len){
            pa = this; // No need to check if &r == this
            pb = &r;
        }
        else {
            pa = &r;
            pb = this;
        }

        polynomial result;
        for (int j = 0; j < pb->_len; ++j){
            polynomial tmp = *pa;
            tmp <<= pb->_onz[j];
            result += (tmp);
        }
        *this = result;
        return *this;
    }

    polynomial& operator/= (const polynomial& r){
        if (this == &r){
            _len = 0;
            return *this;
        }

        while (degree() >= r.degree()){
            int q = degree() - r.degree();
            polynomial tmp = r;
            tmp <<= q;
            *this -= tmp;
        }
        return *this;
    }

private:
    static void skipnonspaces(const char*& g){
        while (*g != ' '){ ++g; }
    }
    static void skipspaces(const char*& g){
        while (*g == ' '){ ++g; }
    }
    static void nextword(const char*& g){
        skipnonspaces(g);
        skipspaces(g);
    }

    void push_back(int e) {
        _onz[_len++] = e;
    }

    /* Number of nonzero coefficients */
    int _len;

    /* Orders with nonzero coefficient
     * assert(_onz[_len] == -1)
     */
    int _onz[2048];
};

int poj1060(istream& input, ostream& output){
    string line;
    line.reserve(2048);
    getline(input, line);

    int cases;
    cases = atoi(line.c_str());

    polynomial a, b, c;
    for (int i = 0; i < cases; ++i){
        getline(input, line);
        a.scan(line);
        getline(input, line);
        b.scan(line);
        getline(input, line);
        c.scan(line);

        polynomial result = a;
        result *= b;
        result /= c;
        result.print(line);
        output << line << '\n';
    }
    return 0;
}
