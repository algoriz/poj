/* See the problem description:
 * http://poj.org/problem?id=3843
 */

#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

class expr_eliminator {
public:
    struct expr_t {
        size_t hash_code;
        int    reg_index;
        const char* key;
        size_t keysize;
        size_t length;
        expr_t* left;       /* left child */
        expr_t* right;      /* right child */

        const char* read_key(const char* s){
            key = s;
            left = right = 0;
            hash_code = 0;
            keysize = 0;
            while (isword(*s)){
                hash_code <<= 8;
                hash_code |= (size_t)*s++;
                ++keysize;
            }
            return s;
        }
    };

    class expr_hashset{
    public:
        expr_hashset(){
            set_size(50000);
        }

        void set_size(size_t n){
            size = n;
            if (size > bucket.size()){
                vector<expr_t*> tmp;
                bucket.assign(size, tmp);
            }
        }

        void clear(){
            for (size_t i = 0; i < size; ++i){
                bucket[i].clear();
            }
        }

        expr_t* find(expr_t* expr) const {
            size_t index = (expr->hash_code + expr->length) % size;
            const vector<expr_t*>& l = bucket[index];
            for (index = 0; index < l.size(); ++index){
                expr_t* it = l[index];
                if (it->length == expr->length
                    && memcmp(it->key, expr->key, expr->length) == 0){
                    return it;
                }
            }
            return 0;
        }

        void insert(expr_t* expr) {
            size_t index = (expr->hash_code + expr->length) % size;
            bucket[index].push_back(expr);
        }

    private:
        size_t size;
        vector<vector<expr_t*>> bucket;
    };

    static bool issepa(int ch){ return ch < 'a' || ch > 'z'; }

    static bool isword(int ch){ return ch >= 'a' && ch <= 'z'; }

    expr_eliminator(){
        expr_pool.assign(50000, expr_t());
        next_alloc = &(expr_pool[0]);
        str.reserve(360000); // 50000 * (4+3) = 350000
    }

    size_t expr_size() const {
        return next_alloc - &expr_pool[0];
    }

    void clear() {
        str.clear();
        next_alloc = &expr_pool[0];
    }

    void read_expr(istream& input){
        clear();
        getline(input, str);

        expr_root = alloc_expr();
        parse(expr_root, str.c_str());
    }

    void write_expr(ostream& output){
        reg_index = 0;
        expr_reg.set_size(expr_size());
        expr_reg.clear();
        print(expr_root, output);
        output << '\n';
    }

private:
    expr_t* alloc_expr(){ return next_alloc++; }

    void alloc_reset(){ next_alloc = &expr_pool[0]; }

    /* Recursive expression parser */
    const char* parse(expr_t* expr, const char* src){
        src = expr->read_key(src);
        if (*src == '('){
            expr_t* left = alloc_expr();
            src = parse(left, ++src);
            expr_t* right = alloc_expr();
            src = parse(right, ++src);
            
            expr->left = left;
            expr->right = right;
            expr->length = ++src - expr->key;
            
            /* hash(expr) = hash(root) XOR hash(l-child) XOR hash(r-child) XOR (-length)
             */
            expr->hash_code ^= left->hash_code ^ right->hash_code;
        }
        else {
            expr->length = src - expr->key;
        }
        return src;
    }

    void print(expr_t* expr, ostream& output){
        expr_t* it = expr_reg.find(expr);
        if (it != 0){
            output << it->reg_index;
        }
        else {
            int index = ++reg_index;
            output.write(expr->key, expr->keysize);

            if (expr->left){
                output << '(';
                print(expr->left, output);
                output << ',';
                print(expr->right, output);
                output << ')';
            }

            expr->reg_index = index;
            expr_reg.insert(expr);
        }
    }
    
    string str;

    expr_t* next_alloc;
    vector<expr_t> expr_pool;
    expr_t* expr_root;

    int reg_index;
    expr_hashset expr_reg;
};

int poj3843(std::istream& input, std::ostream& output){
    string line;
    getline(input, line);
    int count = atoi(line.c_str());

    expr_eliminator eliminator;
    for (int i = 0; i < count; ++i){
        eliminator.read_expr(input);
        eliminator.write_expr(output);

        if (i == count - 1){
            break;
        }
    }
    return 0;
}
