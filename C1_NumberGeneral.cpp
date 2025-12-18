#include <stdio.h>
#include <vector>
using namespace std;

int isPrime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

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

    if (n > 2) {
        p.push_back(n);
        a.push_back(1);
    }
}

int SoNTGanNhat(int m)
{
    int lower = m - 1, upper = m + 1;
    while (lower >= 2) {
        if (isPrime(lower)) {
            return lower;
        }
        if (isPrime(upper)) {
            return upper;
        }
        lower--;
        upper++;
    }
    
    while (isPrime(upper) == 0) {
        upper++;
    }
    return upper;
}

int isPerfect(int n) {
    int sum = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            if (i * i != n)
                sum += i + n / i;
            else
                sum += i;
        }
    }
    return n > 1 && sum == n;
}

void FindPerfectNumbersSmallerThanM(int m) {
    printf("Cac so hoan hao nho hon %d: ", m);
    int found = 0;
    for (int i = 2; i < m; i++) {
        if (isPerfect(i)) {
            printf("%d ", i);
            found = 1;
        }
    }
    if (!found) printf("Khong co");
    printf("\n");
}

int FindPrimesInRange(int a, int m) {
    int sum = 0;
    printf("Cac so nguyen to trong khoang %d den %d: ", a, m);
    for (int i = a; i <= m; i++) {
        if (isPrime(i)) {
            printf("%d ", i);
            sum += i;
        }
    }
    printf("\nTong cac so nguyen to trong khoang %d den %d la: %d\n", a, m, sum);
    return sum;
}

void Print(const vector<int>& p, const vector<int>& a)
{
    for (size_t j = 0; j < p.size(); j++) {
        printf("%d^%d", p[j], a[j]);
        if (j + 1 < p.size()) printf(".");
    }
}

int main()
{
    vector<int> p, a;
    // Du lieu dau vao
    int n = 360;
    int aInput = 5;

    if (n <= 1) {
        printf("Khong co phan tich so nguyen to!\n");
        return 0;
    }

    PhanRaSNT(p, a, n);
    Print(p, a);

    int m = p.back();
    printf("\nUoc so nguyen to lon nhat la: %d\n", m);

    int nearestPrime = SoNTGanNhat(m);
    if (nearestPrime != -1) {
        printf("So nguyen to gan nhat voi %d la: %d\n", m, nearestPrime);
    } else {
        printf("Khong tim thay so nguyen to gan voi %d\n", m);
    }

    FindPerfectNumbersSmallerThanM(m);

    FindPrimesInRange(aInput, m);

    return 0;
}
