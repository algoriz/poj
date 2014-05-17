/* See the problem description:
 * http://poj.org/problem?id=1059
 */
#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

struct jump {
    int start;
    int end;

    bool operator< (const jump& another) const {
        return start < another.start;
    }
};

std::istream& operator>> (std::istream& in, jump& j){
    in >> j.start >> j.end;
    return in;
}

class chutes_ladders {
public:
    chutes_ladders() : players(0){}

    void read_throws(std::istream& input){
        throws.clear();
        throws.reserve(1000);
        int val;
        input >> val;
        while (val != 0){
            throws.push_back(val);
            input >> val;
        }
    }

    bool read_game(std::istream& input){
        reset_game();
        input >> players;
        if (players == 0 || input.fail() || input.eof()){
            return false;
        }

        jump j;
        input >> j;
        while (j.start != 0 || j.end != 0){
            jumps.insert(j);
            input >> j;
        }

        int val;
        input >> val;
        while (val != 0){
            if (val > 0){
                extra_turns.insert(val);
            }
            else {
                miss_turns.insert(-val);
            }
            input >> val;
        }
        return true;
    }

    int run_game(){
        std::vector<int> counter;
        counter.assign(players, 0);
        std::vector<int> sleep;
        sleep.assign(players, 0);
        
        jump j;
        int winner = -1;
        const int* d = throws.data();
        while (winner < 0){
            for (int i = 0; i < players; ++i){
                if (sleep[i] == 1){
                    sleep[i] = 0;
                    continue;
                }

                int pos = counter[i] + *d++;
                if (pos > 100){
                    continue;
                }
                counter[i] = pos;
                while (extra_turns.count(counter[i])){
                    counter[i] += *d++;
                }

                if (miss_turns.count(counter[i])){
                    sleep[i] = 1;
                    continue;
                }
                
                j.start = counter[i];
                for (std::set<jump>::iterator it = jumps.find(j);
                     it != jumps.end();
                     it = jumps.find(j)){
                    j.start = it->end;
                }
                counter[i] = j.start;

                if (counter[i] == 100){
                    winner = i + 1;
                    break;
                }
            }
        }
        return winner;
    }

    void reset_game(){
        players = 0;
        jumps.clear();
        miss_turns.clear();
        extra_turns.clear();
    }

private:
    std::vector<int> throws;
    int players;
    std::set<jump> jumps;
    std::set<int> miss_turns;
    std::set<int> extra_turns;
};

int poj1059(istream& input, ostream& output)
{
    chutes_ladders game;
    game.read_throws(input);
    while (game.read_game(input)){
        output << game.run_game() << '\n';
    }
    return 0;
}
