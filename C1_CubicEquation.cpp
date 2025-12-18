#include <iostream>
#include <cmath>
#include <complex>
#include <iomanip>

#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
using Complex = complex<double>;

class CubicSolver
{
private:
    double a, b, c, d;
    const double EPS = 1e-9;

    void solveCubic()
    {
        if (abs(a) < EPS)
        {
            cout << "Khong phai phuong trinh bac 3!\n";
            return;
        }

        double p = b / a;
        double q = c / a;
        double r = d / a;

        double P = q - p * p / 3;
        double Q = (2 * p * p * p - 9 * p * q + 27 * r) / 27;

        double discriminant = Q * Q / 4 + P * P * P / 27;

        cout << fixed << setprecision(6);

        if (discriminant > EPS)
        {

            double sqrt_disc = sqrt(discriminant);
            double u = cbrt(-Q / 2 + sqrt_disc);
            double v = cbrt(-Q / 2 - sqrt_disc);

            double x1 = u + v - p / 3;

            double real_part = -(u + v) / 2 - p / 3;
            double imag_part = sqrt(3) / 2 * (u - v);

            cout << "1 nghiem thuc: x1 = " << x1 << "\n";
            cout << "2 nghiem phuc: x2 = " << real_part << " + " << imag_part << "i\n";
            cout << "                x3 = " << real_part << " - " << imag_part << "i\n";
        }
        else if (abs(discriminant) <= EPS)
        {

            if (abs(Q) < EPS)
            {

                double x = -p / 3;
                cout << "3 nghiem bang nhau: x = " << x << "\n";
            }
            else
            {

                double x1 = 3 * Q / P - p / 3;
                double x2 = -3 * Q / (2 * P) - p / 3;
                cout << "Nghiem don: x1 = " << x1 << "\n";
                cout << "Nghiem kep: x2 = x3 = " << x2 << "\n";
            }
        }
        else
        {

            double rho = sqrt(-P * P * P / 27);
            double theta = acos(-Q / (2 * rho));

            double x1 = 2 * cbrt(rho) * cos(theta / 3) - p / 3;
            double x2 = 2 * cbrt(rho) * cos((theta + 2 * M_PI) / 3) - p / 3;
            double x3 = 2 * cbrt(rho) * cos((theta + 4 * M_PI) / 3) - p / 3;

            cout << "3 nghiem thuc phan biet:\n";
            cout << "x1 = " << x1 << "\n";
            cout << "x2 = " << x2 << "\n";
            cout << "x3 = " << x3 << "\n";
        }
    }

public:
    void solve(double ax, double bx, double cx, double dx)
    {
        a = ax;
        b = bx;
        c = cx;
        d = dx;

        cout << "\nPhuong trinh: " << a << "x³ ";
        if (b >= 0)
            cout << "+ " << b << "x² ";
        else
            cout << "- " << -b << "x² ";
        if (c >= 0)
            cout << "+ " << c << "x ";
        else
            cout << "- " << -c << "x ";
        if (d >= 0)
            cout << "+ " << d << " = 0\n\n";
        else
            cout << "- " << -d << " = 0\n\n";

        solveCubic();
    }
};

int main()
{
    CubicSolver solver;
    solver.solve(1, 2, 3, 4);
    return 0;
}
