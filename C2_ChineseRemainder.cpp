#include <iostream>
#include <vector>
using namespace std;

long long extgcd(long long a, long long b, long long &x, long long &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long gcd = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

long long modInverse(long long a, long long m)
{
    long long x, y;
    long long gcd = extgcd(a, m, x, y);
    if (gcd != 1)
        return -1;
    return (x % m + m) % m;
}

long long chineseRemainder(vector<long long> &remainders, vector<long long> &moduli)
{
    int n = remainders.size();

    long long M = 1;
    for (int i = 0; i < n; i++)
    {
        M *= moduli[i];
    }

    long long result = 0;

    for (int i = 0; i < n; i++)
    {
        long long Mi = M / moduli[i];
        long long yi = modInverse(Mi, moduli[i]);

        if (yi == -1)
        {
            cout << "Khong ton tai nghiem (cac moduli khong nguyen to cung nhau)" << endl;
            return -1;
        }

        result = (result + remainders[i] * Mi * yi) % M;
    }

    return (result + M) % M;
}

bool isValidSystem(vector<long long> &remainders, vector<long long> &moduli)
{
    int n = remainders.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            long long x, y;
            long long gcd = extgcd(moduli[i], moduli[j], x, y);
            if ((remainders[i] - remainders[j]) % gcd != 0)
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    // Du lieu dau vao: He dong du
    // x ≡ 2 (mod 3)
    // x ≡ 3 (mod 5)
    // x ≡ 2 (mod 7)
    vector<long long> remainders = {2, 3, 2};
    vector<long long> moduli = {3, 5, 7};
    int n = remainders.size();

    cout << "\nHe dong du can giai:" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "x ≡ " << remainders[i] << " (mod " << moduli[i] << ")" << endl;
    }

    if (!isValidSystem(remainders, moduli))
    {
        cout << "\nHe dong du khong co nghiem!" << endl;
        return 0;
    }

    long long solution = chineseRemainder(remainders, moduli);

    if (solution != -1)
    {

        long long M = 1;
        for (int i = 0; i < n; i++)
        {
            M *= moduli[i];
        }

        cout << "\nNghiem cua he dong du:" << endl;
        cout << "x ≡ " << solution << " (mod " << M << ")" << endl;
        cout << "x = " << solution << " + " << M << "k (k ∈ Z)" << endl;

        cout << "\nKiem tra nghiem:" << endl;
        bool correct = true;
        for (int i = 0; i < n; i++)
        {
            long long check = solution % moduli[i];
            cout << solution << " ≡ " << check << " (mod " << moduli[i] << ")";
            if (check == remainders[i])
            {
                cout << " ✓" << endl;
            }
            else
            {
                cout << " ✗" << endl;
                correct = false;
            }
        }

        if (correct)
        {
            cout << "\nNghiem dung!" << endl;
        }
    }

    return 0;
}
