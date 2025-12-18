#include <stdio.h>
#include <vector>
using namespace std;

long long intPow(long long base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) result *= base;
    return result;
}

void PhanRaSNT(vector<int>& p, vector<int>& a, int n) {
    p.clear();
    a.clear();
    
    if (n % 2 == 0) {
        p.push_back(2);
        int count = 0;
        while (n % 2 == 0) {
            count++;
            n /= 2;
        }
        a.push_back(count);
    }
    
    for (int x = 3; x * x <= n; x += 2) {
        if (n % x == 0) {
            p.push_back(x);
            int count = 0;
            while (n % x == 0) {
                count++;
                n /= x;
            }
            a.push_back(count);
        }
    }
    
    if (n > 1) {
        p.push_back(n);
        a.push_back(1);
    }
}

long long TongCacUocSo(const vector<int>& p, const vector<int>& a) {
    long long sum = 1;
    for (size_t j = 0; j < p.size(); j++) {
        sum *= (intPow(p[j], a[j] + 1) - 1) / (p[j] - 1);
    }
    return sum;
}

void Print(const vector<int>& p, const vector<int>& a) {
    printf("%d = ", p.empty() ? 1 : p[0]);
    for (size_t j = 0; j < p.size(); j++) {
        printf("%d^%d", p[j], a[j]);
        if (j + 1 < p.size()) printf(" * ");
    }
}

int main() {
    vector<int> p, a;
    // Du lieu dau vao
    int n = 360;

    if (n <= 1) {
        printf("So %d khong co phan tich thua so nguyen to!\n", n);
        return 0;
    }

    PhanRaSNT(p, a, n);
    Print(p, a);

    printf("\nTong cac uoc so cua %d la: %lld\n", n, TongCacUocSo(p, a));
    return 0;
}
