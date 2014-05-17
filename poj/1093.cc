/* See the problem description:
 * http://poj.org/problem?id=1093
 */
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class formatter {
private:
    struct opt {
        opt() :badness(-1), lnbreak(-1){}
        bool empty() const { return badness == -1; }

        /* optimal badness value */
        int badness;
        /* line break position to get the optimal badness value */
        int lnbreak;
    };

public:
    bool read(istream& input){
        string line;
        getline(input, line);
        int k = atoi(line.c_str());
        if (k == 0){
            return false;
        }

        K = k;
        words.clear();
        do{
            getline(input, line);
        } while (tokenize(line, words) != 0);

        init_dptables();
        return true;
    }

    void pretty_print(ostream& output){
        opt& pb = PB(0, words.size());
    }

private:
    /* Test whether the given character is a separator.
     * As stated in the problem description, words are consist of characters 
     * with ASCII codes between 33 and 126, thus characters with ASCII codes
     * out of range [33,126] are separators.
     */
    static bool issepa(int ch){
        return ch < 33 || ch > 126;
    }

    static size_t tokenize(const string& line, vector<string>& words){
        size_t wc = words.size();
        const char* gb = line.c_str();
        const char* const end = gb + line.length();
        const char* ge;
        while (gb < end){
            while (issepa(*gb)){ ++gb; }
            
            if (gb >= end){ break; }
            ge = gb + 1;
            if (ge >= end){ break; }

            while (!issepa(*ge)){ ++ge; }
            words.push_back(std::string(gb, ge-gb));
            gb = ++ge;
        }
        return words.size() - wc;
    }

    static int line_badness(int K, int wdcount, int wdlength){
        int n = wdcount - 1;
        if (n == 0){
            return 500;
        }

        /* extra spaces needed */
        int sp = K - wdlength;
        int avg = sp / n;
        int odds = sp % n;
        /* (n - odds) * avg * avg + odds * (avg + 1) * (avg + 1) */
        return n*avg*avg + 2*odds*avg + odds;
    }

    /* The total length of words in range [start, end) as a single line without
     * inserting extra spaces.
     */
    int len(int start, int end){
        int& wl = wltable[addr(start, end)];
        if (wl == 0){
            for (int i = start; i < end; ++i){
                wl += words[i].length();
            }
            wl += end - start - 1; /* for W words, (W-1) spaces are needed */
        }
        return wl;
    }

    bool is_valid_line(int start, int end){
        return start < end && len(start, end) <= K;
    }

    /* The line badness value for words in range [start, end) */
    int LB(int start, int end){
        int& lb = lbtable[addr(start, end)];
        if (lb == -1){
            lb = line_badness(K, end - start, len(start, end));
        }
        return lb;
    }

    /* The paragraph badness value for words in range [start, end) */
    opt& PB(int start, int end){
        opt& pb = pbtable[addr(start, end)];
        if (pb.empty()){
            if (is_valid_line(start, end)){
                pb.badness = LB(start, end);
                pb.lnbreak = end;
            }
            else {
                int lnb = end - 1;
                int min_pb = 0x7FFFFFFF;
                for (int b = end - 1; b > start && is_valid_line(b, end); --b){
                    int pb = PB(start, b).badness + LB(b, end);
                    if (pb < min_pb){
                        lnb = b;
                        min_pb = pb;
                    }
                }
                pb.badness = min_pb;
                pb.lnbreak = lnb;
            }
        }
        return pb;
    }

    void init_dptables(){
        N = (int)words.size();
        
        int n = N*(N + 1) / 2;
        wlbuf.assign(n, 0);
        wltable = &(wlbuf[0]);
        lbbuf.assign(n, -1);
        lbtable = &(lbbuf[0]);
        pbbuf.assign(n, opt());
        pbtable = &(pbbuf[0]);
    }

    /* linear address in tables
     * maps pair (x, y) to linear address space [0, (N^2+N)/2 - 1]
     *  where: 0 <= x < N and x < y <= N
     */
    int addr(int x, int y){
        return x + y*(y - 1) / 2;
    }

private:
    /* length of line */
    int K;
    vector<string> words;
    
    /* DP tables */
    int N;
    vector<int> wlbuf;
    int* wltable; /* word length */
    vector<int> lbbuf;
    int* lbtable; /* line badness */
    vector<opt> pbbuf;
    opt* pbtable; /* paragraph badness */
};

int poj1093(std::istream& input, std::ostream& output){
    formatter fmt;
    while (fmt.read(input)){
        fmt.pretty_print(output);
    }
    return 0;
}
