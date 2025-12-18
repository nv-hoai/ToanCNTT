#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<int> sieveOfEratosthenes(int n)
{
    vector<bool> isPrime(n, true);
    vector<int> primes;

    if (n <= 2)
        return primes;

    isPrime[0] = isPrime[1] = false;

    for (int i = 2; i * i < n; i++)
    {
        if (isPrime[i])
        {
            for (int j = i * i; j < n; j += i)
            {
                isPrime[j] = false;
            }
        }
    }

    for (int i = 2; i < n; i++)
    {
        if (isPrime[i])
        {
            primes.push_back(i);
        }
    }

    return primes;
}

bool isPrimeCheck(int n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int findNearestPrime(int n)
{
    int lower = n - 1, upper = n + 1;

    while (lower >= 2)
    {
        if (isPrimeCheck(lower))
            return lower;
        if (isPrimeCheck(upper))
            return upper;
        lower--;
        upper++;
    }
    
    while (isPrimeCheck(upper) == false)
    {
        upper++;
    }
    return upper;
}

// Tìm số nguyên tố trong khoảng [start, end]
vector<int> findPrimesInRange(int start, int end)
{
    vector<int> primes;
    
    if (start > end || end < 2)
        return primes;
    
    // Đảm bảo start >= 2
    if (start < 2)
        start = 2;
    
    // Sử dụng Sieve of Eratosthenes cho khoảng lớn
    if (end - start + 1 > 1000)
    {
        // Sieve từ 2 đến end, sau đó lọc [start, end]
        vector<int> allPrimes = sieveOfEratosthenes(end + 1);
        for (int prime : allPrimes)
        {
            if (prime >= start && prime <= end)
            {
                primes.push_back(prime);
            }
        }
    }
    else
    {
        // Kiểm tra từng số trong khoảng nhỏ
        for (int i = start; i <= end; i++)
        {
            if (isPrimeCheck(i))
            {
                primes.push_back(i);
            }
        }
    }
    
    return primes;
}

long long calculateSum(const vector<int>& arr)
{
    long long sum = 0;
    for (int i = 0; i < arr.size(); i++)
        sum += arr[i];
    return sum;
}

int main()
{
    // Du lieu dau vao
    int choice = 2;
    int n = 100;
    int start = 10, end = 50;

    cout << "=== CHUONG TRINH TIM SO NGUYEN TO ===\n";

    if (choice == 1)
    {
        if (n <= 2)
        {
            cout << "Khong co so nguyen to nho hon " << n << endl;
            return 0;
        }

        vector<int> primes = sieveOfEratosthenes(n);

        long long sum = calculateSum(primes);

        int nearestPrime = findNearestPrime(n);

        cout << "\nSo nguyen to nho hon " << n << " (" << primes.size() << " so):" << endl;
        for (int i = 0; i < primes.size(); i++)
        {
            cout << primes[i];
            if (i < primes.size() - 1)
                cout << ", ";
            if ((i + 1) % 10 == 0)
                cout << endl;
        }

        cout << "\n\nTong cac so nguyen to: " << sum << endl;
        cout << "So nguyen to gan " << n << " nhat: " << nearestPrime << endl;
    }
    else if (choice == 2)
    {

        if (start > end)
        {
            cout << "Khoang khong hop le!\n";
            return 1;
        }

        vector<int> primes = findPrimesInRange(start, end);

        long long sum = 0;
        for (int prime : primes)
        {
            sum += prime;
        }

        cout << "\nSo nguyen to trong khoang [" << start << ", " << end << "] (" << primes.size() << " so):" << endl;
        
        if (!primes.empty())
        {
            for (int i = 0; i < primes.size(); i++)
            {
                cout << primes[i];
                if (i < primes.size() - 1)
                    cout << ", ";
                if ((i + 1) % 10 == 0)
                    cout << endl;
            }
            cout << "\n\nTong cac so nguyen to: " << sum << endl;
            cout << "So nguyen to nho nhat: " << primes.front() << endl;
            cout << "So nguyen to lon nhat: " << primes.back() << endl;
        }
        else
        {
            cout << "Khong co so nguyen to nao trong khoang [" << start << ", " << end << "]\n";
        }
    }
    else
    {
        cout << "Lua chon khong hop le!\n";
    }

    return 0;
}
