#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

// Tìm ước số nguyên tố lớn nhất của n
long long largestPrimeFactor(long long n) {
    long long largest = -1;
    
    while (n % 2 == 0) {
        largest = 2;
        n /= 2;
    }
    
    for (long long i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            largest = i;
            n /= i;
        }
    }
    
    if (n > 2) largest = n;
    
    return largest;
}

// Tính tổng các ước số thực sự của n (không bao gồm n)
long long sumOfDivisors(long long n) {
    if (n <= 1) return 0;
    
    long long sum = 1;
    
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum += i;
            if (i != n / i) {
                sum += n / i;
            }
        }
    }
    
    return sum;
}

// Kiểm tra số hoàn hảo
bool isPerfect(long long n) {
    return n > 1 && sumOfDivisors(n) == n;
}

// Tìm số hoàn hảo trong khoảng [start, end]
vector<long long> findPerfectInRange(long long start, long long end) {
    vector<long long> perfect;
    
    vector<long long> knownPerfect = {6, 28, 496, 8128, 33550336, 8589869056LL, 137438691328LL};
    
    for (long long num : knownPerfect) {
        if (num >= start && num <= end) {
            perfect.push_back(num);
        }
    }
    
    if (end > 137438691328LL) {
        cout << "Tim kiem so hoan hao lon trong khoang [" << start << ", " << end << "]...\n";
        
        // Sử dụng công thức Mersenne cho số lớn
        vector<int> largeMersennePrimes = {61, 89, 107, 127, 521, 607, 1279, 2203, 2281};
        
        for (int p : largeMersennePrimes) {
            if (p > 43) {
                if (p <= 62) {
                    long long perfectNum = (1LL << (p-1)) * ((1LL << p) - 1);
                    
                    if (perfectNum >= start && perfectNum <= end) {
                        perfect.push_back(perfectNum);
                    }
                }
            }
        }
        
        sort(perfect.begin(), perfect.end());
    }
    
    return perfect;
}

// Tìm tất cả số hoàn hảo từ 1 đến n
vector<long long> findPerfectNumbers(long long n) {
    vector<long long> perfect;
    
    // Các số hoàn hảo đã biết (tối ưu cho n nhỏ)
    vector<long long> knownPerfect = {6, 28, 496, 8128, 33550336, 8589869056LL, 137438691328LL};
    
    if (n <= 137438691328LL) {
        for (long long num : knownPerfect) {
            if (num <= n) {
                perfect.push_back(num);
            }
        }
    } else {
        // Với n lớn
        for (long long num : knownPerfect) {
            perfect.push_back(num);
        }
        
        // Tìm thêm bằng công thức Mersenne Prime
        vector<int> mersennePrimes = {2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127};
        
        for (int p : mersennePrimes) {
            if (p > 31) {
                long long mersenne = (1LL << p) - 1;
                long long perfectNum = (1LL << (p-1)) * mersenne;
                
                if (perfectNum <= n && perfectNum > 137438691328LL) {
                    bool isPrime = true;
                    for (long long i = 3; i * i <= mersenne && isPrime; i += 2) {
                        if (mersenne % i == 0) isPrime = false;
                    }
                    
                    if (isPrime) {
                        perfect.push_back(perfectNum);
                    }
                }
            }
        }
        
        sort(perfect.begin(), perfect.end());
    }
    
    return perfect;
}

long long calculateSum(const vector<int>& arr)
{
    long long sum = 0;
    for (int i = 0; i < arr.size(); i++)
        sum += arr[i];
    return sum;
}

int main() {
    // Du lieu dau vao
    int choice = 1;
    long long n = 10000;
    long long start = 1, end = 10000;
    
    cout << "=== CHUONG TRINH TIM SO HOAN HAO ===\n";
    
    if (choice == 1) {
        // Tìm ước số nguyên tố lớn nhất
        long long largestPrime = largestPrimeFactor(n);
        cout << "\n=== UOC SO NGUYEN TO LON NHAT ===\n";
        cout << "Uoc so nguyen to lon nhat cua " << n << ": " << largestPrime << endl;
        
        // Tìm số hoàn hảo từ 1 đến n
        cout << "\n=== SO HOAN HAO TU 1 DEN " << n << " ===\n";
        vector<long long> perfectNums = findPerfectNumbers(n);
        
        cout << "So luong: " << perfectNums.size() << endl;
        
        if (!perfectNums.empty()) {
            cout << "Danh sach: ";
            long long sum = 0;
            for (int i = 0; i < perfectNums.size(); i++) {
                cout << perfectNums[i];
                if (i < perfectNums.size() - 1) cout << ", ";
                sum += perfectNums[i];
            }
            cout << "\nTong cac so hoan hao: " << sum << endl;
        } else {
            cout << "Khong co so hoan hao nao trong khoang 1 den " << n << endl;
        }
    } else if (choice == 2) {
        if (start > end) {
            cout << "Khoang khong hop le!\n";
            return 1;
        }
        
        cout << "\n=== SO HOAN HAO TRONG KHOANG [" << start << ", " << end << "] ===\n";
        vector<long long> perfectNums = findPerfectInRange(start, end);
        
        cout << "So luong: " << perfectNums.size() << endl;
        
        if (!perfectNums.empty()) {
            cout << "Danh sach: ";
            long long sum = 0;
            for (int i = 0; i < perfectNums.size(); i++) {
                cout << perfectNums[i];
                if (i < perfectNums.size() - 1) cout << ", ";
                sum += perfectNums[i];
            }
            cout << "\nTong cac so hoan hao: " << sum << endl;
        } else {
            cout << "Khong co so hoan hao nao trong khoang [" << start << ", " << end << "]\n";
        }
        
    } else {
        cout << "Lua chon khong hop le!\n";
    }
    
    return 0;
}