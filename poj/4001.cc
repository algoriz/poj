#include <iostream>


struct point {
    int x, y;
};

std::istream& operator>> (std::istream& is, point& p){
    return (is >> p.y >> p.x);
}

class chinese_chess {
public:
    enum roles { GENERAL = 'G', ROOK = 'R', HORSE = 'H', CANNON = 'C' };

    chinese_chess(){
        /* board boundary */
        for (int i = 0; i < 11; ++i){
            _state[0][i] = 1;
            _state[11][i] = 1;
        }
        for (int i = 0; i < 12; ++i){
            _state[i][0] = 1;
            _state[i][10] = 1;
        }
        reset();
    }

    void reset() {
        _n = 0;
        for (int i = 1; i < 11; ++i){
            for (int j = 1; j < 10; ++j){
                _state[i][j] = 0;
            }
        }
    }

    void add_piece(int role, point p){
        _state[p.y][p.x] = role;
        _pieces[_n++] = p;
    }

    /* Test if the black general at bgp is checkmated */
    bool is_checkmate(point bgp){
        int checked[12][11];
        memcpy(checked, _state, sizeof(checked));

        for (int i = 0; i < _n; ++i){
            point& p = _pieces[i];
            int x = p.x;
            int y = p.y;
            
            switch (_state[y][x]) {
            case GENERAL:
                /* possible flying general */
                for (int f = v_prev(p), k = 3; k > f; --k){
                    checked[k][x] = 1;
                }
                break;
            
            case ROOK:
                for (int yl = v_prev(p) + 1, yu = v_next(p); yl < yu; ++yl){
                    checked[yl][x] = 1;
                }
                for (int xl = h_prev(p) + 1, xu = h_next(p); xl < xu; ++xl){
                    checked[y][xl] = 1;
                }
                break;
            
            case HORSE:{
                    point hp[8] = {
                            { x - 1, y - 2 }, { x + 1, y - 2 },
                            { x + 2, y - 1 }, { x + 2, y + 1 },
                            { x + 1, y + 2 }, { x - 1, y + 2 },
                            { x - 2, y + 1 }, { x - 2, y - 1 }
                    };
                    point hh[8] = {
                            { 0, -1 }, { 0, -1 },
                            { 1, 0 }, { 1, 0 },
                            { 0, 1 }, { 0, 1 },
                            { -1, 0 }, { -1, 0 }
                    };
                    for (int i = 0; i < 8; ++i){
                        checked[hp[i].y][hp[i].x] = is_valid(hp[i]) && !_state[y + hh[i].y][x + hh[i].x];
                    }
                }
                break;
            
            case CANNON:
            {
                point pp = { x, v_prev(p) };
                point ppp = { x, v_prev(ppp) };
                for (int k = ppp.y + 1; k < pp.y; ++k){
                    checked[k][x] = 1;
                }
            }

                {
                    point pp = { x, v_next(p) };
                    point ppp = { x, v_next(pp) };
                    for (int k = pp.y + 1; k < ppp.y; ++k){
                        checked[k][x] = 1;
                    }
                }

                {
                    point pp = { x, h_prev(p) };
                    point ppp = { x, h_prev(pp) };
                    for (int k = ppp.x + 1; k < pp.x; ++k){
                        checked[y][k] = 1;
                    }
                }

                {
                    point pp = { x, h_next(p) };
                    point ppp = { x, h_next(pp) };
                    for (int k = pp.x + 1; k < ppp.x; ++k){
                        checked[y][k] = 1;
                    }
                }
                break;
            }
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

        return checked[bgp.y - 1][bgp.x] && checked[bgp.y + 1][bgp.x]
            && checked[bgp.y][bgp.x + 1] && checked[bgp.y][bgp.x - 1];
    }

private:
    bool is_valid(point p){
        return p.x > 0 && p.x < 10 && p.y > 0 && p.y < 11;
    }

    int h_next(point p){
        for (int x = p.x + 1; x < 11; ++x){
            if (_state[p.y][x] != 0){
                return x;
            }
        }
        return 11;
    }

    int h_prev(point p){
        for (int x = p.x - 1; x > 0; --x){
            if (_state[p.y][x] != 0){
                return x;
            }
        }
        return 0;
    }

    int v_next(point p){
        for (int y = p.x + 1; y < 10; ++y){
            if (_state[y][p.x] != 0){
                return y;
            }
        }
        return 10;
    }

    int v_prev(point p){
        for (int y = p.x - 1; y > 0; --y){
            if (_state[y][p.x] != 0){
                return y;
            }
        }
        return 0;
    }

private:
    int _state[12][11];
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
