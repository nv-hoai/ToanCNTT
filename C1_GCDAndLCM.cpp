#include <stdio.h>

int gcd_recursive(int a, int b)
{
    if (b == 0)
        return a;
    return gcd_recursive(b, a % b);
}

int gcd_iterative(int a, int b)
{
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int gcd_subtraction(int a, int b)
{
    while (a != b) {
        if (a > b)
            a = a - b;
        else
            b = b - a;
    }
    return a;
}

int lcm_gcd(int a, int b)
{
    return (a * b) / gcd_recursive(a, b);
}

int lcm_addition(int a, int b)
{
    int max = (a > b) ? a : b;
    int lcm = max;
    while (lcm % a != 0 || lcm % b != 0) {
        lcm++;
    }
    return lcm;
}

int main()
{
    // Du lieu dau vao
    int a = 48, b = 18;
    
    printf("\n=== UOC CHUNG LON NHAT (GCD) ===\n");
    printf("GCD(%d, %d) - De quy Euclid: %d\n", a, b, gcd_recursive(a, b));
    printf("GCD(%d, %d) - Lap Euclid: %d\n", a, b, gcd_iterative(a, b));
    printf("GCD(%d, %d) - Phep tru: %d\n", a, b, gcd_subtraction(a, b));
    
    printf("\n=== BOI CHUNG NHO NHAT (LCM) ===\n");
    printf("LCM(%d, %d) - Dung GCD: %d\n", a, b, lcm_gcd(a, b));
    printf("LCM(%d, %d) - Cong lap: %d\n", a, b, lcm_addition(a, b));
    
    return 0;
}