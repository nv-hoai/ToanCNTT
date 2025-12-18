#include <stdio.h>
#include <vector>
#include <algorithm>
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

void LietKeUocSo(int n)
{
    vector<int> divisors;
    for (int i = 1; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            divisors.push_back(i);
            if (i != n / i)
                divisors.push_back(n / i);
        }
    }
    
    sort(divisors.begin(), divisors.end());
    for (size_t i = 0; i < divisors.size(); i++) {
        printf("%d ", divisors[i]);
    }
}

void Print(const vector<int>& p, const vector<int>& a)
{
    printf("%d = ", p.empty() ? 1 : p[0]);
    for (size_t j = 0; j < p.size(); j++)
    {
        printf("%d^%d", p[j], a[j]);
        if (j + 1 < p.size()) printf(" * ");
    }
}

int main()
{
    vector<int> p, a;
    // Du lieu dau vao
    int n = 360;

    if (n <= 1) {
        printf("So %d khong co phan tich thua so nguyen to!\n", n);
        return 0;
    }

    PhanRaSNT(p, a, n);
    Print(p, a);

    printf("\n");
    printf("So luong cac uoc so cua %d la: %d", n, SoLuongUocSo(a));

    printf("\n");
    printf("Liet ke cac uoc so: ");
    LietKeUocSo(n);
    printf("\n");
    return 0;
}