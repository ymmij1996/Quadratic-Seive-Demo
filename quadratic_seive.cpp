// compile: g++ quadratic_seive.cpp -o quadratic_seive
#include <bits/stdc++.h>
//sudo apt update
//sudo apt install libboost-all-dev
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost::multiprecision;

vector<int> primes;
cpp_int N, rootN;
int M, B, B_prime_num = 0, B_ori_prime_num = 0;

struct Row {
    boost::dynamic_bitset<> exp_mod2;   // factor exponents mod 2
    boost::dynamic_bitset<> comb;       // tracks smooth number combination
};

cpp_int isqrt(const cpp_int &n) {
    if (n == 0) return 0;
    cpp_int x = n;
    cpp_int y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

int calc_B(const cpp_int& N) {
    // Estimate ln(N) ~ digits * ln(10)
    size_t digits = N.str().size();
    double logN = digits * log(10.0);
    double loglogN = log(logN);
    double B = exp(0.75 * sqrt(logN * loglogN));
    return (int)B + 1;
}

void factor_smooth(vector<cpp_int>& x_list, vector<cpp_int>& fx_list, vector<Row>& mat) {
    cpp_int start = rootN + 1;
    cpp_int curr_end = rootN + M;
    cpp_int end = rootN + 2 * M;

    for (cpp_int x = start;; x++) {
        if (x >= end) {
            break;
        }
        if (x >= curr_end && x_list.size() <= B_ori_prime_num + 10) {
            //cout << "Too few relations, extend attempt range." << endl;
            curr_end += M;
        }

        cpp_int fx = x * x - N, fx_val = fx;
        Row row;

        row.exp_mod2.resize(B_prime_num, 0);
        for (int i = 0; i < B_prime_num && fx_val > 1; i++) {
            int p = primes[i];

            while (fx_val % p == 0) {
                fx_val /= p;
                row.exp_mod2[i] ^= 1;
            }
        }
        //cout << "x=" << x << ", f(x)=" << fx << endl;
        if (fx_val == 1) {
            x_list.push_back(x);
            fx_list.push_back(fx);
            mat.push_back(row);
        }
    }
}

vector<cpp_int> gauss_elimi_xor(vector<cpp_int>& x_list, vector<cpp_int>& fx_list, vector<Row>& mat) {
    int rowi = 0;

    for (int i = 0; i < x_list.size(); i++) {
        mat[i].comb.resize(x_list.size());
        mat[i].comb.reset();
        mat[i].comb[i] = 1;
    }

    // Gaussian elimination over GF(2)
    for (int col = 0; col < B_prime_num && rowi < mat.size(); col++) {
        int pivot = -1;
        for (int i = rowi; i < mat.size(); i++) {
            if (mat[i].exp_mod2[col]) { pivot = i; break; }
        }
        if (pivot == -1) continue;
        swap(mat[rowi], mat[pivot]);
        for (int i = 0; i < mat.size(); i++) {
            if (i != rowi && mat[i].exp_mod2[col]) {
                mat[i].exp_mod2 ^= mat[rowi].exp_mod2;
                mat[i].comb ^= mat[rowi].comb;
            }
        }
        rowi++;
    }

    function<cpp_int(cpp_int, cpp_int)> gcd = [](cpp_int a, cpp_int b) -> cpp_int {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b != 0) {
            cpp_int tmp = a % b;
            a = b;
            b = tmp;
        }
        return a;
    };

    // find the row that is 0
    for (int i = 0; i < mat.size(); i++) {
        if (mat[i].exp_mod2.none() && mat[i].comb.any()) {
            cpp_int fx_product = 1;
            cpp_int X = 1, Y = 1;
            for (int j = 0; j < mat.size(); j++) {
                if (mat[i].comb.test(j)) {
                    fx_product *= fx_list[j];
                    X *= x_list[j];
                }
            }
            Y = isqrt(fx_product); // this should be perfect square
            //assert(Y*Y == fx_product);
            
            cpp_int sol;
            sol = gcd(N, X - Y);
            if (sol != N && sol != 1) {
                return {sol, N / sol};
            }
            sol = gcd(N, X + Y);
            if (sol != N && sol != 1) {
                return {sol, N / sol};
            }
        }
    }
    return {};
}

int main() {
    ifstream file("target.txt");
    vector<cpp_int> target {0, 0};
    char split;
    string line;

    if (!file.is_open()) {
        cerr << "Failed to open file\n";
        return 1;
    }

    {
        ifstream file_p("prime.txt");
        if (!file_p) {
            cerr << "Failed to open prime file\n";
            return 1;
        }

        while (getline(file_p, line)) {
            stringstream ss(line);
            string token;
            while (getline(ss, token, ',')) {
                if (!token.empty()) {
                    primes.push_back(stoi(token));
                }
            }
        }
    }

    cout << "Load " << primes.size() << " primes..." << endl << endl;

    while (getline(file, line)) {
        if (line.length() >=2 && line[0] == '/' && line[1] == '/') continue;
        bool found = false;
        stringstream ss(line);
        char split;
        B_ori_prime_num = 0;

        if (!(ss >> target[0] >> split >> target[1] && split == '*')) {
            cerr << "Invalid line format: " << line << "\n";
            continue;
        }
        
        auto start = chrono::high_resolution_clock::now();
        N = target[0] * target[1];
        int origin_B = calc_B(N);
        rootN = isqrt(N);
        
        cout << "N=" << N << endl;
        cout << "--------------------" << endl;

        for (int B_scale = 1; B_scale <= 2; B_scale++) {
            B = origin_B * B_scale;
            M = B;
            B_prime_num = 0;
            while (B_prime_num < primes.size() && primes[B_prime_num] <= B) { // binary search could be faster
                B_prime_num++;
            }
            if (B_ori_prime_num == 0) B_ori_prime_num = B_prime_num;

            cout << "B=" << B << endl;

            vector<cpp_int> x_list;
            vector<cpp_int> fx_list;
            vector<Row> mat; // start to prepare matrix early

            factor_smooth(x_list, fx_list, mat);

            cout << "Found " << x_list.size() << " smooth number." << endl;

            // XOR Gaussian elimination
            vector<cpp_int> sol = gauss_elimi_xor(x_list, fx_list, mat);

            if (!sol.empty()) {
                cout << "--------------------" << endl;
                cout << N << " = " << sol[0] << " * " << sol[1] << endl;
                found = true;
                break;
            }
        }
        if (!found) cout << "No solution for B: " << B << endl;
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        cout << "Execution time: " << (int)elapsed_seconds.count() << " seconds" << endl << endl << endl;
    }
    file.close();
    return 0;
}

