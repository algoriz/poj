/* See the problem description:
 * http://poj.org/problem?id=1093
 * 
 * Key Observation:
 * Let
 *   PB(a, b) = Optimal paragraph badness for words within range [a, b)
 *   LB(a, b) = Optimal line badness for words within range [a, b)
 * then we have
 *   PB(a, b) = min{LB(a, j) + PB(j, b) | a < j <= b, len(a,j) <= K}
 *   (forward DP)
 * or
 *   PB(a, b) = min{PB(a, j) + LB(j, b) | a <= j < b, len(j,b) <= K}
 *   (backward DP)
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
    bool read_paragraph(istream& input){
        string line;
        getline(input, line);
        int k = atoi(line.c_str());
        if (k == 0){
            return false;
        }

        fmt_width = k;
        words.clear();
        do{
            getline(input, line);
        } while (tokenize(line, words) != 0);

        init();
        return true;
    }

    void pretty_print(ostream& output){
        pretty_print(output, 0, words.size());
        output << '\n';
    }

private:
    void pretty_print(ostream& output, int start, int end){
        opt& pb = PB(start, end);
        print_line(output, start, pb.lnbreak);
        if (pb.lnbreak != end){
            pretty_print(output, pb.lnbreak, end);
        }
    }

    void print_line(ostream& output, int start, int end){
        string* wd = &(words[start]);
        output << *wd++;

        /* Inserting spaces before other words */
        int count = end - start - 1;
        if (count > 0){
            int sp = fmt_width - len(start, end);
            int avg = 1 + sp / count; /* average spaces needed */
            int m = count - (sp % count);
            sp = avg;
            for (int i = 0; i < m; ++i){
                output.write(spaces.c_str(), sp);
                output << *wd++;
            }
            ++sp;
            for (int i = m; i < count; ++i){
                output.write(spaces.c_str(), sp);
                output << *wd++;
            }
        }

        output << '\n';
    }

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

            while (!issepa(*ge)){ ++ge; }
            words.push_back(std::string(gb, ge-gb));
            gb = ++ge;
        }
        return words.size() - wc;
    }

    static int line_badness(int fmt_width, int wdcount, int wdlength){
        int n = wdcount - 1;
        if (n == 0){
            return wdlength == fmt_width ? 0 : 500;
        }

        /* extra spaces needed */
        int sp = fmt_width - wdlength;
        int avg = sp / n;
        int odds = sp % n;
        /* (n - odds) * avg * avg + odds * (avg + 1) * (avg + 1) */
        return n*avg*avg + 2*odds*avg + odds;
    }

    /* The length of the line composed of words within range [start, end).
     * A single space as separator is counted.
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
        return start < end && len(start, end) <= fmt_width;
    }

    /* The line badness value for words in range [start, end) */
    int LB(int start, int end){
        int& lb = lbtable[addr(start, end)];
        if (lb == -1){
            lb = line_badness(fmt_width, end - start, len(start, end));
        }
        return lb;
    }

    /* The optimal paragraph badness value for words in range [start, end) */
    opt& PB(int start, int end){
        opt& pb = pbtable[addr(start, end)];
        if (pb.empty()){
            int lnb = end;
            int min_pb = 0x7FFFFFFF;

            if (is_valid_line(start, end)){
                min_pb = LB(start, end);
                pb.badness = min_pb;
                pb.lnbreak = end;
            }
            
            for (int b = start + 1; b < end && is_valid_line(start, b); ++b){
                int pb = LB(start, b) + PB(b, end).badness;
                /* Note that operator <= is used.
                 * Among two solutions with same badness, we choose the one
                 * that has a minor gap at the first position where two gap
                 * length differs.
                 */
                if (pb <= min_pb){
                    lnb = b;
                    min_pb = pb;
                }
            }
            pb.badness = min_pb;
            pb.lnbreak = lnb;
        }
        return pb;
    }

    void init(){
        spaces.assign(fmt_width, ' ');
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
    /* Target width of formated text */
    int fmt_width;
    vector<string> words;

    /* fmt_width spaces */
    string spaces;

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
    while (fmt.read_paragraph(input)){
        fmt.pretty_print(output);
    }
    return 0;
}
