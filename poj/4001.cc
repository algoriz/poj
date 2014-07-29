#include <iostream>

struct point {
    int y, x;
};

std::istream& operator>> (std::istream& is, point& p){
    return (is >> p.y >> p.x);
}

class chinese_chess {
public:
    enum roles { GENERAL = 'G', ROOK = 'R', HORSE = 'H', CANNON = 'C' };

    chinese_chess(){
        reset();
    }

    void reset() {
        _n = 0;
        memset(_board, 0, sizeof(_board));
    }

    void add_piece(int role, point p){
        _board[p.y][p.x] = role;
        _pieces[_n++] = p;
    }

    /* Test if the black general at bgp is checkmated */
    bool is_checkmate(point bgp){
        int checked[12][11];
        memset(checked, 0, sizeof(checked));
        /* board boundary */
        for (int i = 0; i < 11; ++i){
            checked[0][i] = 1;
            checked[11][i] = 1;
        }
        for (int i = 0; i < 12; ++i){
            checked[i][0] = 1;
            checked[i][10] = 1;
        }

        /* No move out of palace */
        checked[4][4] = 1;
        checked[4][5] = 1;
        checked[4][6] = 1;
        checked[1][3] = 1;
        checked[2][3] = 1;
        checked[3][3] = 1;
        checked[1][7] = 1;
        checked[2][7] = 1;
        checked[3][7] = 1;

        for (int i = 0; i < _n; ++i){
            point& p = _pieces[i];
            int x = p.x;
            int y = p.y;
            
            switch (_board[y][x]) {
            case GENERAL:
                /* possible flying general */
                for (int f = v_prev(p), k = 3; k >= f; --k){
                    checked[k][x] = 1;
                }
                break;
            
            case ROOK:
                for (int yp = v_prev(p); yp != y; ++yp){
                    checked[yp][x] = 1;
                }
                for (int yp = v_next(p); yp != y; --yp){
                    checked[yp][x] = 1;
                }
                for (int xp = h_prev(p); xp != x; ++xp){
                    checked[y][xp] = 1;
                }
                for (int xp = h_next(p); xp != x; --xp){
                    checked[y][xp] = 1;
                }
                break;
            
            case HORSE:{
                    point hp[8] = {
                            { y - 2, x - 1 }, { y - 2, x + 1 },
                            { y - 1, x + 2 }, { y + 1, x + 2 },
                            { y + 2, x + 1 }, { y + 2, x - 1 },
                            { y + 1, x - 2 }, { y - 1, x - 2 }
                    };
                    point hh[8] = {
                            { -1, 0 }, { -1, 0 },
                            { 0, 1 }, { 0, 1 },
                            { 1, 0 }, { 1, 0 },
                            { 0, -1 }, { 0, -1 }
                    };
                    for (int i = 0; i < 8; ++i){
                        if (is_valid(hp[i]) && !_board[y + hh[i].y][x + hh[i].x]){
                            checked[hp[i].y][hp[i].x] = 1;
                        }
                    }
                }
                break;
            
            case CANNON:
                {
                    point pp = { v_prev(p), x };
                    point ppp = { v_prev(pp), x };
                    for (int k = ppp.y; k != pp.y; ++k){
                        checked[k][x] = 1;
                    }
                }

                {
                    point pp = { v_next(p), x };
                    point ppp = { v_next(pp), x };
                    for (int k = ppp.y; k != pp.y; --k){
                        checked[k][x] = 1;
                    }
                }

                {
                    point pp = { y, h_prev(p) };
                    point ppp = { y, h_prev(pp) };
                    for (int k = ppp.x; k != pp.x; ++k){
                        checked[y][k] = 1;
                    }
                }

                {
                    point pp = { y, h_next(p) };
                    point ppp = { y, h_next(pp) };
                    for (int k = ppp.x; k != pp.x; --k){
                        checked[y][k] = 1;
                    }
                }
                break;
            }
        }

        return checked[bgp.y][bgp.x]
            && checked[bgp.y - 1][bgp.x] && checked[bgp.y + 1][bgp.x]
            && checked[bgp.y][bgp.x + 1] && checked[bgp.y][bgp.x - 1];
    }

private:
    bool is_valid(point p){
        return p.x > 0 && p.x < 10 && p.y > 0 && p.y < 11;
    }

    int h_next(point p){
        for (int x = p.x + 1; x < 11; ++x){
            if (_board[p.y][x] != 0){
                return x;
            }
        }
        return 11;
    }

    int h_prev(point p){
        for (int x = p.x - 1; x > 0; --x){
            if (_board[p.y][x] != 0){
                return x;
            }
        }
        return 0;
    }

    int v_next(point p){
        for (int y = p.y + 1; y < 10; ++y){
            if (_board[y][p.x] != 0){
                return y;
            }
        }
        return 10;
    }

    int v_prev(point p){
        for (int y = p.y - 1; y > 0; --y){
            if (_board[y][p.x] != 0){
                return y;
            }
        }
        return 0;
    }

private:
    int _board[12][11];
    point _pieces[8]; /* Red */
    int _n;
};

int poj4001(std::istream& input, std::ostream& output) {
    chinese_chess game;

    int red;
    point bgp;
    char role;
    point p;
    while (true){
        input >> red >> bgp;
        if (red == 0){
            break;
        }

        game.reset();
        for (int i = 0; i < red; ++i){
            input >> role >> p;
            game.add_piece(role, p);
        }
        output << (game.is_checkmate(bgp) ? "YES\n" : "NO\n");
    }
    return 0;
}
