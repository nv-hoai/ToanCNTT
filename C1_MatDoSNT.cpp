#include <stdio.h>
#include <math.h>

int main() {
    // Du lieu dau vao
    double n = 1000;

    if (n <= 1) {
        printf("Gia tri n phai lon hon 1!\n");
        return 1;
    }

    double matDo = n / log(n);
    printf("Mat do so nguyen to tu 1 den %.0lf la: %.6lf\n", n, matDo);
    printf("So luong so nguyen to uoc tinh: %.0lf\n", matDo);

    return 0;
}