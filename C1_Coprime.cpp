#include <stdio.h>
#include <stdlib.h>

// Ham tinh Euler's totient (phi) - so luong so nguyen to cung voi n
int eulerPhi(int n)
{
    int result = n;
    for (int p = 2; p * p <= n; p++)
    {
        if (n % p == 0)
        {
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

// Ham kiem tra 2 so co nguyen to cung nhau khong (GCD = 1)
int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Liet ke va dem cac so nguyen to cung voi n
int numberAndListCoprime(int n)
{
    int count = 0;
    printf("Cac so nguyen to cung voi %d: ", n);
    for (int i = 1; i < n; i++)
    {
        if (gcd(i, n) == 1)
        {
            printf("%d ", i);
            count++;
        }
    }
    printf("\n");
    return count;
}

int main()
{
    // Du lieu dau vao
    int n = 15;

    printf("=== PHUONG PHAP 1: Liet ke va dem ===\n");
    int count1 = numberAndListCoprime(n);
    printf("So luong so nguyen to cung nho hon %d la: %d\n\n", n, count1);

    printf("=== PHUONG PHAP 2: Tinh truc tiep bang Euler Phi ===\n");
    int count2 = eulerPhi(n);
    printf("So luong so nguyen to cung nho hon %d la: %d\n", n, count2);

    return 0;
}
