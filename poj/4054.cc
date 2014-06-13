/* See the problem description:
 * http://poj.org/problem?id=4054
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;
struct word{
    const char* str;
    size_t length;
};

struct stricmptor{
    bool operator()(const word& w1, const word& w2) const {
        return (w1.length < w2.length)
            || (w1.length == w2.length && memcmp(w1.str, w2.str, w1.length) < 0);
    }
};

class abbrsolver{
public:
    typedef map<word, vector<int>, stricmptor> tkinf;

    void solve(istream& input, ostream& output){
        char text[4096] = { 0 };
        size_t length = 0;
        input.read(text, 4094);
        length = strlen(text);
        tokenize(text, length);

        int em = 0;
        size_t im = 0, jm = 0;
        size_t wc = words.size();
        for (size_t i = 0; i < wc; ++i){
            if (occur[i].size() < 2){ continue; }
            for (size_t j = i; j < wc; ++j){
                if (occur[j].size() < 2){ break; }
                int e = effectiveness(i, j+1);
                if (e > em || (e == em && j - i < jm - im)){
                    em = e;
                    im = i;
                    jm = j+1;
                }
                else if (e < 0){ break; }
            }
        }

        output << em << '\n';
        if (jm > im){
            for (size_t i = im; i < jm; ++i){
                output << (char)toupper(words[i].str[0]);
            }
            output << '\n';
        }
    }

    int effectiveness(int begin, int end){
        char abbr[4096];
        word w;
        w.str = abbr;
        w.length = 0;
        for (int i = begin; i < end; ++i){
            abbr[w.length] = words[i].str[0];
            ++w.length;
        }

        if (tokens.count(w) != 0){
            return -1; // not a valid abbr.
        }

        int o = occurrance(begin, end);
        if (o < 2){
            return -1; // occur only once
        }

        int tl = word_length(begin, end);
        return (o - 1)*(tl - end + begin) - end + begin;
    }

private:
    void tokenize(char* text, size_t length){
        word wd;
        char* p = text;
        char* pend = text + length;
        int iwd = 0;
        while (p < pend){
            while (p < pend && !isalpha(*p)){ ++p; }
            wd.str = p;
            wd.length = 0;
            while (p < pend && isalpha(*p)){
                *p = tolower(*p);
                ++p;
                ++wd.length;
            }
            //*p = 0;
            ++p;
            words.push_back(wd);
            tokens[wd].push_back(iwd);
            ++iwd;
        }

        occur.assign(iwd, vector<int>());
        for (tkinf::iterator it = tokens.begin(); it != tokens.end(); ++it){
            for (size_t i = 0; i < it->second.size(); ++i){
                occur[it->second[i]] = it->second;
            }
        }
    }

    int word_length(int begin, int end){
        int sum = 0;
        for (int i = begin; i < end; ++i){
            sum += words[i].length;
        }
        return sum;
    }

    int text_length(int begin, int end){
        return (words[end - 1].str - words[begin].str) + words[end - 1].length;
    }

    int occurrance(int begin, int end){
        if (end - begin == 1){
            return occur[begin].size();
        }

        vector<int> vi = occur[begin];
        {
            vector<int> ve;
            for (int j = begin + 1; j < end; ++j){
                seq_count(vi, occur[j], ve, j-begin);
                vi.clear();
                vi.swap(ve);
            }
        }

        const char* original = words[begin].str;
        int length = text_length(begin, end);
        for (size_t i = 0; i < vi.size();){
            if (memcmp(original, words[vi[i]].str, length) != 0){
                // not exactly same text
                vi.erase(vi.begin() + i);
            }
            else if (i > 0 && vi[i] < vi[i - 1] + end - begin){
                // overlapped
                vi.erase(vi.begin() + i);
            }
            else { ++i; }
        }
        return vi.size();
    }

    static void seq_count(const vector<int>& vi, const vector<int>& vj, vector<int>& ve, int k){
        int i = 0, im = vi.size();
        int j = 0, jm = vj.size();
        while (i < im && j < jm){
            while (i < im && vi[i] + k < vj[j]){ ++i; }
            if (i >= im){ break; }

            while (j < jm && vj[j] < vi[i] + k){ ++j; }
            if (j >= jm){ break; }

            // NOT checking overlap here, see occurrance()
            if (vi[i] + k == vj[j]){
                // store possible evidence
                ve.push_back(vi[i]);
                ++i;
                ++j;
            }
        }
    }

    tkinf tokens;
    vector<word> words;
    vector<vector<int>> occur;
};

int poj4054(std::istream& input, std::ostream& output){
    abbrsolver a;
    a.solve(input, output);
    return 0;
}

