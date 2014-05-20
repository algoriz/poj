#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool issepa(int ch){
    return ch < 'a' || ch > 'z';
}
bool isword(int ch){
    return ch >= 'a' && ch <= 'z';
}

class tree_compresser {
public:
    struct node_t {
        const char* key;
        const char* begin;
        const char* end;
        node_t* left;
        node_t* right;
        int    index;
        
        const char* read_key(const char* s){
            left = right = 0;
            key = s;
            while (isword(*s)){ s++; }
            return begin = end = s;
        }
        
        size_t key_length() const {
            return begin - key;
        }

        size_t length() const {
            return end - key;
        }
    };

    void read(const string& input){
        tree_str = input;
        subtrees.clear();
        subtrees.reserve(1000);

        /* First pass, get all nodes */
        node_t node;
        for (const char* p = tree_str.c_str(); *p;){
            p = node.read_key(p);
            node.index = subtrees.size();
            subtrees.push_back(node);
            while (*p && issepa(*p)){ ++p; }
        }

        /* Second pass, get subtree boundaries */
        vector<node_t*> sp;
        sp.reserve(subtrees.size());
        node_t* current = &(subtrees[0]);
        for (const char* p = current->begin; *p; ){
            switch (*p)
            {
            case '(':
                sp.push_back(current);
                current->left = current + 1;
                ++current;
                p = current->begin;
                break;
            case ')':
                if (current->begin == p){
                    current->end = p;
                }
                sp.back()->end = ++p;
                sp.pop_back();
                break;
            case ',':
                if (current->begin == p){
                    current->end = p;
                }
                sp.back()->right = ++current;
                p = current->begin;
                break;
            default:
                ++p; /* Should never reach here */
            }
        }
    }

    void write_compressed(string& dst){
        dst.clear();
        dst.reserve(subtrees[0].length());

        print_index = 0;
        reusable_subtrees.clear();
        print(&subtrees[0], dst);
    }

private:
    void print(node_t* subtree, string& output){
        set<node_t, node_pred>::iterator it = reusable_subtrees.find(*subtree);
        if (it != reusable_subtrees.end()){
            char buf[16];
            _itoa(it->index, buf, 10);
            output += buf;
        }
        else {
            int index = ++print_index;
            output.append(subtree->key, subtree->key_length());

            if (subtree->left){
                output.append(1, '(');
                print(subtree->left, output);
                output.append(1, ',');
                print(subtree->right, output);
                output.append(1, ')');
            }

            node_t printed_node = *subtree;
            printed_node.index = index;
            reusable_subtrees.insert(printed_node);
        }
    }

    struct node_pred {
        bool operator() (const node_t& n1, const node_t& n2) const {
            int len1 = n1.length();
            int len2 = n2.length();
            if (len1 < len2){
                return true;
            }
            else if (len1 > len2){
                return false;
            }
            return memcmp(n1.key, n2.key, len1) < 0;
        }
    };

    string tree_str;

    /* Table of subtrees */
    vector<node_t> subtrees;
    
    int print_index;
    set<node_t, node_pred> reusable_subtrees;
};

int poj3843(std::istream& input, std::ostream& output){
    string line;
    line.reserve(102400);
    getline(input, line);
    int count = atoi(line.c_str());

    tree_compresser pd;
    for (int i = 0; i < count; ++i){
        getline(input, line);
        pd.read(line.c_str());
        pd.write_compressed(line);
        output << line << '\n';
    }
    return 0;
}
