#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double f(double x, double y, double z)
{
    return 0.5 * log(x * x + y * y + z * z) + sin(x * y) + cos(y + z) / 5.0 + (x - z) * (x - z) / (1.0 + y * y);
}

double df_dx(double x, double y, double z)
{
    double r2 = x * x + y * y + z * z;
    return x / r2 + y * cos(x * y) + 2.0 * (x - z) / (1.0 + y * y);
}

double df_dy(double x, double y, double z)
{
    double r2 = x * x + y * y + z * z;
    return y / r2 + x * cos(x * y) - sin(y + z) / 5.0 - 2.0 * (x - z) * (x - z) * y / ((1.0 + y * y) * (1.0 + y * y));
}

double df_dz(double x, double y, double z)
{
    double r2 = x * x + y * y + z * z;
    return z / r2 - sin(y + z) / 5.0 - 2.0 * (x - z) / (1.0 + y * y);
}

int main()
{
    cout << fixed << setprecision(6);
    cout << "========== GRADIENT DESCENT METHOD ==========" << endl;
    cout << "Problem: Minimize f(x,y,z)" << endl
         << endl;

    // Tham so thuat toan
    double gamma = 0.02;      // Learning rate (buoc nhay, ty le hoc)
    double epsilon = 1e-5;    // Nguong sai so de dung (tolerance)

    cout << "Parameters:" << endl;
    cout << "  gamma (learning rate) = " << gamma << endl;
    cout << "  epsilon (tolerance)   = " << epsilon << endl
         << endl;

    double x = 1.0, y = 1.0, z = 1.0;

    cout << "Initial point: x=" << x << ", y=" << y << ", z=" << z << endl;
    cout << "Initial value: f=" << f(x, y, z) << endl
         << endl;

    int iteration = 0;
    int max_iterations = 10000;
    double norm_grad = 1e10;

    cout << "Iteration | x         | y         | z         | f(x,y,z) | ||grad|| " << endl;
    cout << "---------|-----------|-----------|-----------|----------|----------" << endl;

    while (iteration < max_iterations && norm_grad > epsilon)
    {

        double gx = df_dx(x, y, z);
        double gy = df_dy(x, y, z);
        double gz = df_dz(x, y, z);

        norm_grad = sqrt(gx * gx + gy * gy + gz * gz);

        if (iteration % 100 == 0)
        {
            printf("%9d | %9.5f | %9.5f | %9.5f | %8.5f | %8.2e\n",
                   iteration, x, y, z, f(x, y, z), norm_grad);
        }

        // Cap nhat bien theo huong nguoc gradient
        x = x - gamma * gx;
        y = y - gamma * gy;
        z = z - gamma * gz;

        iteration++;
    }

    double gx = df_dx(x, y, z);
    double gy = df_dy(x, y, z);
    double gz = df_dz(x, y, z);
    norm_grad = sqrt(gx * gx + gy * gy + gz * gz);

    printf("%9d | %9.5f | %9.5f | %9.5f | %8.5f | %8.2e\n",
           iteration, x, y, z, f(x, y, z), norm_grad);

    cout << "\n========== RESULTS ==========" << endl;
    cout << "Converged in " << iteration << " iterations" << endl;
    cout << "Final point:" << endl;
    cout << "  x = " << x << endl;
    cout << "  y = " << y << endl;
    cout << "  z = " << z << endl;
    cout << "Minimum value: " << f(x, y, z) << endl;
    cout << "||gradient|| = " << norm_grad << endl;

    return 0;
}
