/* See the problem description:
 * http://poj.org/problem?id=3844
 * 
 * Key Observation:
 *   sum{Am, Am+1, ..., An} MOD d = 0
 * <=>
 *   sum{A1, A2, ..., Am-1} MOD d = sum{A1, A2, ..., An} MOD d
 *   
 * Let R(n) = sum{A1, A2, ..., An} MOD d, then for all substrings {Am, ..., An}
 * that the sum of which is dividable by d, we get a pair (m, n) that satisfies
 * R(m-1) = R(n), with R(0) = 0 defined.
 */

#include <iostream>
#include <string>

using namespace std;

int r[1000000];
long long solve(std::istream& input){
    int d, n;
    input >> d >> n;
    memset(r, 0, sizeof(int)*d);

    int v, sum = 0;
    while (n--){
        input >> v;
        ++r[sum = (sum + v) % d];
    }

    long long count = r[0];
    for (int i = 0; i < d; ++i){
        count += r[i] * ((long long)r[i] - 1) / 2;
    }
    return count;
}

int poj3844(std::istream& input, std::ostream& output){
    string line;
    getline(input, line);
    int count = atoi(line.c_str());

    for (int i = 0; i < count; ++i){
        output << solve(input) << '\n';
    }
    return 0;
}
