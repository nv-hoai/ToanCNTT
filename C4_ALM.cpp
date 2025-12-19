#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double f(double x, double y, double z)
{
    return (x - 1) * (x - 1) + (y - 2) * (y - 2) + (z - 1) * (z - 1) + 0.1 * sin(x + y + z);
}

double h(double x, double y, double z)
{
    return 2 * x - y + z - 3;
}

double df_dx(double x, double y, double z)
{
    return 2*x + 0.1*cos(x + y + z) - 2;
}

double df_dy(double x, double y, double z)
{
    return 2*y + 0.1*cos(x + y + z) - 4;
}

double df_dz(double x, double y, double z)
{
    return 2*z + 0.1*cos(x + y + z) - 2;
}

const double dh_dx = 2.0;
const double dh_dy = -1.0;
const double dh_dz = 1.0;

// Ham Augmented Lagrangian: L(x, lambda, rho) = f(x) - lambda*h(x) + (rho/2)*h(x)^2
// Tham so:
//   x, y, z: Bien
//   lambda: Nhan tu Lagrange
//   rho: Tham so penalty
double augmented_lagrangian(double x, double y, double z, double lambda, double rho)
{
    double h_val = h(x, y, z);
    return f(x, y, z) - lambda * h_val + 0.5 * rho * h_val * h_val;
}

// Gradient cua Augmented Lagrangian
// Tham so:
//   x, y, z: Bien
//   lambda: Nhan tu Lagrange
//   rho: Tham so penalty
//   gx, gy, gz: Gradient (output, truyen theo tham chieu)
void grad_augmented_lagrangian(double x, double y, double z, double lambda, double rho,
                               double &gx, double &gy, double &gz)
{
    double h_val = h(x, y, z);
    gx = df_dx(x, y, z) + (rho * h_val - lambda) * dh_dx;
    gy = df_dy(x, y, z) + (rho * h_val - lambda) * dh_dy;
    gz = df_dz(x, y, z) + (rho * h_val - lambda) * dh_dz;
}

// Ham giai bai toan con (minimize Augmented Lagrangian)
// Tham so:
//   x, y, z: Bien can toi uu (truyen theo tham chieu)
//   lambda: Nhan tu Lagrange
//   rho: Tham so penalty (he so phat)
void solve_subproblem(double &x, double &y, double &z, double lambda, double rho)
{
    const int max_iter = 5000;  // So vong lap toi da cho subproblem
    const double tol = 1e-8;    // Nguong sai so de dung
    double alpha = 0.1;         // Buoc nhay ban dau (line search)

    for (int i = 0; i < max_iter; i++)
    {
        double gx, gy, gz;
        grad_augmented_lagrangian(x, y, z, lambda, rho, gx, gy, gz);

        double norm_g = sqrt(gx * gx + gy * gy + gz * gz);
        if (norm_g < tol)
            break;

        double L_old = augmented_lagrangian(x, y, z, lambda, rho);
        double alpha_temp = alpha;
        bool found = false;

        for (int ls = 0; ls < 30; ls++)
        {
            double x_new = x - alpha_temp * gx;
            double y_new = y - alpha_temp * gy;
            double z_new = z - alpha_temp * gz;

            double L_new = augmented_lagrangian(x_new, y_new, z_new, lambda, rho);

            if (L_new < L_old - 0.0001 * alpha_temp * norm_g * norm_g)
            {
                x = x_new;
                y = y_new;
                z = z_new;
                alpha = min(1.0, alpha_temp * 1.1);
                found = true;
                break;
            }
            alpha_temp *= 0.5;
        }

        if (!found)
        {
            alpha *= 0.5;
            if (alpha < 1e-12)
                break;
        }
    }
}

int main()
{
    cout << fixed << setprecision(6);
    cout << "========== PHUONG PHAP LAGRANGE TANG CUONG ==========" << endl;
    cout << "Bai toan: Cuc tieu hoa f(x,y,z) = (x-1)^2 + (y-2)^2 + (z-1)^2 + 0.1*sin(x+y+z)" << endl;
    cout << "          voi rang buoc: h(x,y,z) = 2x - y + z - 3 = 0" << endl
         << endl;

    double x = 0.0, y = 0.0, z = 0.0;
    double lambda = 0.0;
    double rho = 1.0;
    double rho_max = 1e6;
    double epsilon = 1e-6;

    cout << "Tham so:" << endl;
    cout << "  rho ban dau = " << rho << endl;
    cout << "  rho toi da   = " << rho_max << endl;
    cout << "  epsilon      = " << epsilon << endl
         << endl;

    cout << "Diem bat dau: x=" << x << ", y=" << y << ", z=" << z << endl;
    cout << "Gia tri ban dau: f=" << f(x, y, z) << ", h=" << h(x, y, z) << endl
         << endl;

    cout << "Vong | x         | y         | z         | f(x,y,z) | h(x,y,z) | lambda   | rho      " << endl;
    cout << "-----|-----------|-----------|-----------|----------|----------|----------|----------" << endl;

    int outer_iter = 0;
    int max_outer = 50;
    double h_prev = 1e10;

    while (outer_iter < max_outer)
    {

        solve_subproblem(x, y, z, lambda, rho);

        double h_val = h(x, y, z);
        double f_val = f(x, y, z);

        printf("%4d | %9.5f | %9.5f | %9.5f | %8.5f | %8.2e | %8.5f | %8.1f\n",
               outer_iter, x, y, z, f_val, h_val, lambda, rho);

        if (fabs(h_val) < epsilon)
        {
            cout << "\nDa hoi tu!" << endl;
            break;
        }

        lambda = lambda - rho * h_val;

        if (fabs(h_val) > 0.25 * fabs(h_prev))
        {

            rho = min(rho_max, 2.0 * rho);
        }

        h_prev = h_val;
        outer_iter++;
    }

    cout << "\n========== KET QUA ==========" << endl;
    cout << "Hoi tu sau " << outer_iter << " vong lap ngoai" << endl;
    cout << "Diem toi uu:" << endl;
    cout << "  x = " << x << endl;
    cout << "  y = " << y << endl;
    cout << "  z = " << z << endl;
    cout << "Gia tri ham muc tieu: f = " << f(x, y, z) << endl;
    cout << "Gia tri rang buoc: h = " << h(x, y, z) << endl;
    cout << "Nhan tu Lagrange: lambda = " << lambda << endl;

    cout << "\nKiem tra rang buoc:" << endl;
    cout << "  2x - y + z - 3 = " << (2 * x - y + z - 3) << endl;

    return 0;
}
