#include <stdio.h>
#include <vector>
using namespace std;

int LaSNT(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

int LaSoHoanHao(int n) {
    if (n <= 1) return 0;

    int sum = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum += i;
            if (i != n / i) sum += n / i;
        }
    }
    return sum == n;
}

int main() {
    // Du lieu dau vao
    int m = 1, n = 100;

    vector<int> primes, perfects;
    
    for (int x = m; x <= n; x++) {
        if (LaSNT(x))
            primes.push_back(x);
        if (LaSoHoanHao(x))
            perfects.push_back(x);
    }

    printf("\nCac so nguyen to trong khoang [%d, %d]:\n", m, n);
    if (!primes.empty()) {
        for (size_t i = 0; i < primes.size(); i++)
            printf("%d ", primes[i]);
        printf("\nTong cong: %zu so\n", primes.size());
    } else {
        printf("Khong co\n");
    }

    printf("\nCac so hoan hao trong khoang [%d, %d]:\n", m, n);
    if (!perfects.empty()) {
        for (size_t i = 0; i < perfects.size(); i++)
            printf("%d ", perfects[i]);
        printf("\nTong cong: %zu so\n", perfects.size());
    } else {
        printf("Khong co\n");
    }

    return 0;
}