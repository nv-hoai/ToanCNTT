#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;

void PhanRaSNT(vector<int>& p, vector<int>& a, int n)
{
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

int SoLuongUocSo(const vector<int>& a)
{
    int sum = 1;
    for (size_t j = 0; j < a.size(); j++)
    {
        sum *= (a[j] + 1);
    }
    return sum;
}

// Tinh tich cac uoc so theo modulo de tranh overflow
// Cong thuc: neu d chan thi tich = n^(d/2), neu d le thi tich = sqrt(n) * n^(d/2)
const long long MOD = 1e9 + 7;

long long powerMod(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

long long TichCacUocSo(int n, const vector<int>& a) {
    int d = SoLuongUocSo(a);
    // Tich cac uoc = n^(d/2) (neu d chan) hoac sqrt(n) * n^(d/2) (neu d le)
    // Dung modulo de tranh overflow
    long long result = powerMod(n, d / 2, MOD);
    if (d % 2 == 1) {
        // Neu d le, nhan them sqrt(n) (xap xi)
        long long sqrtn = (long long)sqrt(n);
        result = (result * sqrtn) % MOD;
    }
    return result;
}

void Print(const vector<int>& p, const vector<int>& a) {
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
    printf("Phan tich thua so nguyen to cua %d: ", n);
    Print(p, a);

    printf("\nSo luong uoc so: %d\n", SoLuongUocSo(a));
    printf("Tich cac uoc so cua %d (mod %lld) la: %lld\n", n, MOD, TichCacUocSo(n, a));
    return 0;
}
