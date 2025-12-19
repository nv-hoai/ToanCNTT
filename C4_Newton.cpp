#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

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

// Dao ham bac hai (Hessian matrix)
// Tham so:
//   x, y, z: Bien
//   H: Ma tran Hessian 3x3 (output, truyen theo tham chieu)
//      H[i][j] = d2f / (dxi * dxj)
void computeHessian(double x, double y, double z, vector<vector<double>>& H)
{
    double r2 = x * x + y * y + z * z;
    double r4 = r2 * r2;
    double yy1 = 1.0 + y * y;
    double yy2 = yy1 * yy1;

    // d2f/dx2
    H[0][0] = (y * y + z * z - x * x) / r4 - y * y * sin(x * y) + 2.0 / yy1;

    // d2f/dxdy
    H[0][1] = -2.0 * x * y / r4 + cos(x * y) - x * y * sin(x * y) - 4.0 * (x - z) * y / yy2;
    H[1][0] = H[0][1];

    // d2f/dxdz
    H[0][2] = -2.0 * x * z / r4 - 2.0 / yy1;
    H[2][0] = H[0][2];

    // d2f/dy2
    H[1][1] = (x * x + z * z - y * y) / r4 - x * x * sin(x * y) - cos(y + z) / 5.0 
              - 2.0 * (x - z) * (x - z) * (1.0 - 3.0 * y * y) / (yy2 * yy1);

    // d2f/dydz
    H[1][2] = -2.0 * y * z / r4 - cos(y + z) / 5.0 + 4.0 * (x - z) * y / yy2;
    H[2][1] = H[1][2];

    // d2f/dz2
    H[2][2] = (x * x + y * y - z * z) / r4 - cos(y + z) / 5.0 + 2.0 / yy1;
}

// Giai he phuong trinh tuyen tinh H * p = -g bang Gaussian elimination
// Tham so:
//   A: Ma tran he so (truyen theo tham chieu)
//   b: Vector ve phai
//   sol: Nghiem (output, truyen theo tham chieu)
void solveLinearSystem(vector<vector<double>>& A, vector<double>& b, vector<double>& sol)
{
    int n = A.size();
    
    // Tao ma tran mo rong
    vector<vector<double>> augmented(n, vector<double>(n + 1));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            augmented[i][j] = A[i][j];
        }
        augmented[i][n] = b[i];
    }

    // Khu Gauss
    for (int i = 0; i < n; i++)
    {
        // Tim pivot
        int maxRow = i;
        for (int k = i + 1; k < n; k++)
        {
            if (abs(augmented[k][i]) > abs(augmented[maxRow][i]))
            {
                maxRow = k;
            }
        }
        swap(augmented[i], augmented[maxRow]);

        // Kiem tra singular
        if (abs(augmented[i][i]) < 1e-12)
        {
            augmented[i][i] = 1e-12;
        }

        // Khu xuong
        for (int k = i + 1; k < n; k++)
        {
            double factor = augmented[k][i] / augmented[i][i];
            for (int j = i; j <= n; j++)
            {
                augmented[k][j] -= factor * augmented[i][j];
            }
        }
    }

    // Thay nguoc
    sol.resize(n);
    for (int i = n - 1; i >= 0; i--)
    {
        sol[i] = augmented[i][n];
        for (int j = i + 1; j < n; j++)
        {
            sol[i] -= augmented[i][j] * sol[j];
        }
        sol[i] /= augmented[i][i];
    }
}

// Thuat toan Newton
// Tham so:
//   x, y, z: Bien can toi uu (truyen theo tham chieu)
//   epsilon: Nguong sai so de dung (tolerance)
//   max_iterations: So vong lap toi da
// Tra ve: So vong lap thuc te da thuc hien
int newtonMethod(double& x, double& y, double& z, double epsilon, int max_iterations)
{
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

        if (iteration % 1 == 0)
        {
            printf("%9d | %9.5f | %9.5f | %9.5f | %8.5f | %8.2e\n",
                   iteration, x, y, z, f(x, y, z), norm_grad);
        }

        if (norm_grad < epsilon)
            break;

        // Tinh Hessian
        vector<vector<double>> H(3, vector<double>(3));
        computeHessian(x, y, z, H);

        // Giai he phuong trinh H * p = -g
        vector<double> neg_g = {-gx, -gy, -gz};

        vector<double> p(3);
        solveLinearSystem(H, neg_g, p);

        // Kiem tra xem huong p co giam ham khong (p^T * g < 0)
        double directional = p[0] * gx + p[1] * gy + p[2] * gz;
        
        // Neu khong phai descent direction, dung gradient descent
        if (directional >= 0)
        {
            p[0] = -gx;
            p[1] = -gy;
            p[2] = -gz;
            directional = -(gx * gx + gy * gy + gz * gz);
        }

        // Backtracking line search voi Armijo condition
        double alpha = 1.0;
        double f_old = f(x, y, z);
        double c1 = 1e-4;  // Armijo parameter
        double x_new, y_new, z_new;
        double f_new;

        for (int ls = 0; ls < 30; ls++)
        {
            x_new = x + alpha * p[0];
            y_new = y + alpha * p[1];
            z_new = z + alpha * p[2];

            f_new = f(x_new, y_new, z_new);

            // Armijo condition: f(x + alpha*p) <= f(x) + c1 * alpha * grad^T * p
            if (f_new <= f_old + c1 * alpha * directional)
            {
                break;
            }

            alpha *= 0.5;
        }

        // Cap nhat vi tri
        x = x_new;
        y = y_new;
        z = z_new;

        iteration++;
    }

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
    cout << "========== PHUONG PHAP NEWTON ==========" << endl;
    cout << "Bai toan: Tim cuc tieu f(x,y,z)" << endl << endl;

    // Du lieu dau vao
    double epsilon = 1e-5;
    int max_iterations = 100;

    cout << "Tham so:" << endl;
    cout << "  epsilon (sai so)      = " << epsilon << endl;
    cout << "  So vong lap toi da    = " << max_iterations << endl << endl;

    double x = 1.0, y = 1.0, z = 1.0;

    cout << "Diem bat dau: x=" << x << ", y=" << y << ", z=" << z << endl;
    cout << "Gia tri ban dau: f=" << f(x, y, z) << endl << endl;

    // Thuc hien thuat toan Newton
    int iterations_done = newtonMethod(x, y, z, epsilon, max_iterations);

    cout << "\n========== KET QUA ==========" << endl;
    cout << "So vong lap thuc te: " << iterations_done << endl;
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
