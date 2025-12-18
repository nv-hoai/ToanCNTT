#include <stdio.h>
#include <vector>
using namespace std;

vector<int> sieve(int a, int b) {
    vector<int> res;
    if (b < 2) return res;
    
    vector<bool> prime(b + 1, true);
    prime[0] = prime[1] = false;
    
    for (int p = 2; p * p <= b; p++) {
        if (prime[p]) {
            for (int i = p * p; i <= b; i += p)
                prime[i] = false;
        }
    }
    
    if (a < 2) a = 2;
    for (int p = a; p <= b; p++) {
        if (prime[p]) {
            res.push_back(p);
        }
    }
    return res;
}

int main() {
    // Du lieu dau vao
    int a = 10, b = 50;
    
    vector<int> primes = sieve(a, b);
    
    printf("Cac so nguyen to trong khoang [%d, %d]:\n", a, b);
    for (size_t i = 0; i < primes.size(); i++) {
        printf("%d ", primes[i]);
    }
    printf("\nTong cong co %zu so nguyen to\n", primes.size());
    
    return 0;
}