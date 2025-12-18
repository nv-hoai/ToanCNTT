#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Ham muc tieu
double f(double x, double y, double z)
{
    return 0.5 * log(x * x + y * y + z * z) + sin(x * y) + cos(y + z) / 5.0 + (x - z) * (x - z) / (1.0 + y * y);
}

// Dao ham bac nhat
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

// Thuat toan Gradient Descent voi Momentum
// Tham so:
//   x, y, z: Bien can toi uu (truyen theo tham chieu)
//   gamma: Learning rate (buoc nhay, ty le hoc)
//   beta: He so momentum (quan tinh, thong thuong 0.9)
//   epsilon: Nguong sai so de dung (tolerance)
//   max_iterations: So vong lap toi da
// Tra ve: So vong lap thuc te da thuc hien
int momentumMethod(double& x, double& y, double& z, double gamma, double beta, double epsilon, int max_iterations)
{
    double vx = 0.0, vy = 0.0, vz = 0.0;
    int iteration = 0;
    double norm_grad = 1e10;

    cout << "Vong lap | x         | y         | z         | f(x,y,z) | ||grad|| " << endl;
    cout << "---------|-----------|-----------|-----------|----------|----------" << endl;

    while (iteration < max_iterations && norm_grad > epsilon)
    {
        // Tinh gradient
        double gx = df_dx(x, y, z);
        double gy = df_dy(x, y, z);
        double gz = df_dz(x, y, z);

        norm_grad = sqrt(gx * gx + gy * gy + gz * gz);

        if (iteration % 100 == 0)
        {
            printf("%9d | %9.5f | %9.5f | %9.5f | %8.5f | %8.2e\n",
                   iteration, x, y, z, f(x, y, z), norm_grad);
        }

        // Cap nhat voi momentum
        vx = beta * vx - gamma * gx;
        vy = beta * vy - gamma * gy;
        vz = beta * vz - gamma * gz;

        x += vx;
        y += vy;
        z += vz;

        iteration++;
    }

    // Hien thi ket qua cuoi cung
    double gx = df_dx(x, y, z);
    double gy = df_dy(x, y, z);
    double gz = df_dz(x, y, z);
    norm_grad = sqrt(gx * gx + gy * gy + gz * gz);

    printf("%9d | %9.5f | %9.5f | %9.5f | %8.5f | %8.2e\n",
           iteration, x, y, z, f(x, y, z), norm_grad);
    
    return iteration;
}

int main()
{
    cout << fixed << setprecision(6);
    cout << "========== GRADIENT DESCENT VOI MOMENTUM ==========" << endl;
    cout << "Bai toan: Tim cuc tieu f(x,y,z)" << endl << endl;

    // Du lieu dau vao
    double gamma = 0.02;
    double beta = 0.9;
    double epsilon = 1e-5;
    int max_iterations = 10000;

    cout << "Tham so:" << endl;
    cout << "  gamma (learning rate) = " << gamma << endl;
    cout << "  beta (momentum)       = " << beta << endl;
    cout << "  epsilon (sai so)      = " << epsilon << endl;
    cout << "  So vong lap toi da    = " << max_iterations << endl << endl;

    double x = 1.0, y = 1.0, z = 1.0;

    cout << "Diem bat dau: x=" << x << ", y=" << y << ", z=" << z << endl;
    cout << "Gia tri ban dau: f=" << f(x, y, z) << endl << endl;

    // Thuc hien thuat toan Momentum
    int iterations_done = momentumMethod(x, y, z, gamma, beta, epsilon, max_iterations);

    cout << "\n========== KET QUA ==========" << endl;
    cout << "So vong lap: " << iterations_done << endl;
    cout << "Diem hoi tu:" << endl;
    cout << "  x = " << x << endl;
    cout << "  y = " << y << endl;
    cout << "  z = " << z << endl;
    cout << "Gia tri cuc tieu: " << f(x, y, z) << endl;

    double gx = df_dx(x, y, z);
    double gy = df_dy(x, y, z);
    double gz = df_dz(x, y, z);
    double norm_grad = sqrt(gx * gx + gy * gy + gz * gz);
    cout << "||gradient|| = " << norm_grad << endl;

    return 0;
}
