#include <iostream>
#include <map>
#include <set>
#include <vector>

class ddl_tokenizer {
    char _buf[1024];
    int _p;
public:
    ddl_tokenizer() : _p(0){
        _buf[1022] = ' ';
        _buf[1023] = 0;
    }

    void reset() { _p = 0; memset(_buf, ' ', 1022); }
    char* buf() { return _buf; }
    size_t buf_size() { return sizeof(_buf) - 2; }

    char* next() {
        while (_buf[_p] == ' '){ ++_p; }
        char* token = _buf + _p;
        while (_buf[_p] != ' '){ ++_p; }
        return token;
    }
};

class ddl_interpreter {
    enum { BADVALUE = 0x7FFFFFFF };
    enum { CMD_DCL, CMD_ASGN, CMD_GOTO, CMD_ADD, CMD_END };
public:
    void read_program(std::istream& input) {
        init(_unref, 0, 256);
        init(_regs, BADVALUE, 256);

        ddl_tokenizer tokens;
        input.getline(tokens.buf(), tokens.buf_size());
        int lines = atoi(tokens.next());

        int cmd, op1, op2;
        for (int i = 0; i < lines; ++i) {
            tokens.reset();
            input.getline(tokens.buf(), tokens.buf_size());
            op1 = op2 = 0;

            char* t = tokens.next();
            if (_strnicmp(t, "DCL", 3) == 0){
                /* Dcl X */
                cmd = CMD_DCL;
                op1 = *tokens.next();
            }
            else if (_strnicmp(t, "GOTO", 4) == 0){
                cmd = CMD_GOTO;
                char* v = tokens.next();
                if (isdigit(*v)){
                    /* Goto Label */
                    op2 = 0;
                    op1 = atoi(v);
                }
                else {
                    /* Goto X Label */
                    op2 = *v;
                    op1 = atoi(tokens.next());
                }
            }
            else if (_strnicmp(t, "INC", 3) == 0){
                /* Inc X */
                cmd = CMD_ADD;
                op1 = *tokens.next();
                op2 = 1;
            }
            else if (_strnicmp(t, "DEC", 3) == 0){
                /* Dec X */
                cmd = CMD_ADD;
                op1 = *tokens.next();
                op2 = -1;
            }
            else if (_strnicmp(t, "END", 3) == 0){
                cmd = CMD_END;
            }
            else {
                /* X = value */
                cmd = CMD_ASGN;
                op1 = *t;
                tokens.next();
                op2 = atoi(tokens.next());
            }

            _cmd[i] = cmd;
            _op1[i] = op1;
            _op2[i] = op2;
        }
    }

    void run(std::ostream& output) {
        _output = &output;
        _pc = 0;
        while (_pc != -1){
            execute();
        }
    }

private:
    bool dcl(int id){
        if (_unref[id]){
            return false;
        }
        _unref[id] = 1;
        _regs[id] = 0;
        return true;
    }

    bool assign(int id, int val){
        if (!exist(id)){
            return false;
        }
        _regs[id] = val;
        _unref[id] = 0;
        return true;
    }

    bool add(int id, int val){
        if (!exist(id)){
            return false;
        }
        _regs[id] = get(id) + val;
        _unref[id] = 0;
        return true;
    }

    bool exist(int id) const {
        return (_regs[id] != BADVALUE) || (_unref[id] != 0);
    }

    int get(int id) const {
        return _unref[id] ? 0 : _regs[id];
    }

    /* unreferenced variable */
    void err1(){
        *_output << _pc + 1 << " 1\n";
    }

    /* undeclared variable */
    void err2(){
        *_output << _pc + 1 << " 2\n";
    }

    void execute(){
        int op1 = _op1[_pc];
        int op2 = _op2[_pc];
        switch (_cmd[_pc])
        {
        case CMD_DCL:
            if (!dcl(op1)){
                err1();
            }
            break;
        case CMD_ASGN:
            if (!assign(op1, op2)){
                err2();
            }
            break;
        case CMD_GOTO:
            if (op2 == 0){ /* unconditional jump */
                _pc = op1 - 1;
                return;
            }

            if (!exist(op2)){
                err2();
            }
            else if (get(op2) > 0){
                _pc = op1 - 1;
                return;
            }
            break;
        case CMD_ADD:
            if (!add(op1, op2)){
                err2();
            }
            break;
        case CMD_END:
            _pc = -1;
            return;
        }
        ++_pc;
    }

private:
    static void init(int* p, int val, int n){
        int* end = p + n;
        while (p != end){
            *p++ = val;
        }
    }

private:
    int _pc;            /* program counter */
    int _cmd[102];      /* instructions */
    int _op1[102];      /* 1st operand */
    int _op2[102];      /* 2nd operand */
    int _regs[256];     /* registers that store variables in use */
    int _unref[256];    /* unreferenced variables */
    std::ostream* _output;
};

int poj1684(std::istream& input, std::ostream& output){
    ddl_interpreter u;
    ddl_tokenizer t;
    input.getline(t.buf(), t.buf_size());
    int n = atoi(t.next());

    for (int i = 0; i < n; ++i){
        output << i + 1 << '\n';
        u.read_program(input);
        u.run(output);
    }
    return 0;
}

